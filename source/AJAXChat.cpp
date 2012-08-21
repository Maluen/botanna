#include "AJAXChat.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDomDocument>
#include <QTextEdit>
#include <QTimer>

const int _readInterval = 1000;

AJAXChat::AJAXChat(QObject *parent) : QObject(parent)
{
    net = new QNetworkAccessManager(this);
    _maxWriteLength = 1040;
    lastMsgId = 0;

    readTimer = new QTimer(this);
    readTimer->setInterval(_readInterval);
    connect(readTimer, SIGNAL(timeout()), this, SLOT(read()));
}

void AJAXChat::connectToServer(const QString &server, const QString &userName, const QString &password,
                               const QString &channel, const QString &forumLoginUrl)
{
    _server = server;

    // setup request parameters
    QUrl params;
    params.addQueryItem("login", "login");
    params.addQueryItem("redirect", server);
    params.addQueryItem("username", userName);
    params.addQueryItem("password", password);
    params.addQueryItem("channelName", channel);
    params.addQueryItem("lang", "en");
    params.addQueryItem("submit", "Login");

    // if the forum login url is set, then login to the forum and then go to the chat,
    // otherwise, directly go to the chat
    QString connectUrl = forumLoginUrl.isEmpty() ? server : forumLoginUrl;

    QUrl url(connectUrl);
    QNetworkRequest request(url);
    // content type is needed or you will get runtime debug warnings. This kind of header is needed or
    // the chat will just asks the bot to logout
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = net->post(request, params.encodedQuery());
    connect(reply, SIGNAL(finished()), this, SLOT(connectFinished()));
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

/** Starts to read on connected. */
void AJAXChat::connectFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(QObject::sender());

    if (reply->errorString().isEmpty() || reply->errorString() == "Unknown error") {
        emit connectFinished(true);
        readTimer->start();
    } else {
        qDebug(QString("Connect error: " + reply->errorString()).toUtf8());
        emit connectFinished(false);
        readTimer->stop();
    }
}

void AJAXChat::quit()
{
    QUrl url(_server);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = net->post(request, "logout=true");
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));

    readTimer->stop();
}

/** The chat is connected to the server if the read timer is active. */
bool AJAXChat::isConnected()
{
    return readTimer->isActive();
}

void AJAXChat::join(const QString &roomName)
{

}

void AJAXChat::read(const QString &roomName)
{
    QNetworkReply *reply = net->get(QNetworkRequest(QUrl(_server + "?ajax=true")));
    connect(reply, SIGNAL(finished()), this, SLOT(readFinished()));
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

void AJAXChat::readFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(QObject::sender());

    QByteArray data = reply->readAll();
    QDomDocument dataDocument;
    if (!dataDocument.setContent(data)) {
        qDebug("Error on setting data document.");
        return;
    }

    // Update users
    QDomElement usersElement = dataDocument.documentElement().firstChildElement("users");
    //qDebug(dataDocument.toString().toUtf8());
    if (usersElement.isNull()) {
        qDebug("No messages.");
        return;
    }
    QDomNodeList usersNodeList = usersElement.elementsByTagName("user");
    users.clear();
    for (uint i=0; i<usersNodeList.length(); i++) {
        QDomElement user = usersNodeList.at(i).toElement();
        users.insert(user.attribute("userID").toInt(), user);
    }

    // Get messages
    QDomElement messagesElement = dataDocument.documentElement().firstChildElement("messages");
    if (messagesElement.isNull()) {
        qDebug("No messages.");
        return;
    }
    QDomNodeList messagesNodeList = messagesElement.elementsByTagName("message");
    // send only the new messages
    qDebug(QString("Reading new messages... (tot: %1)").arg(QString::number(messagesNodeList.length())).toUtf8());
    for (uint i=0; i<messagesNodeList.length(); i++) {
        QDomElement message = messagesNodeList.item(i).toElement();
        int msgId = message.attribute("id").toInt();
        if (msgId > lastMsgId) {
            qDebug(QString("%1) msgID: %2 lastID: %3").arg(QString::number(i), QString::number(msgId), QString::number((lastMsgId))).toUtf8());
            emitChatData(message);
        }
    }
    // last entry is the last message
    int newLastMsgId = messagesNodeList.item(messagesNodeList.length()-1).toElement().attribute("id").toInt();
    if (newLastMsgId > lastMsgId) {
        // bug fix: sometimes the xml contains only older messages, so this check prevents to update the lastMsgId
        // with an old ID that will lead in the next reads to process the same messages again.
        lastMsgId = newLastMsgId;
    }
    qDebug(QString("New lastMsgId: %1").arg(QString::number(lastMsgId)).toUtf8());
}

void AJAXChat::write(const QString &msg, const QString &roomName)
{
    /*
    QString url = _server + "?ajax=true&text=" + QUrl::toPercentEncoding(msg);
    QNetworkReply *reply = net->get(QNetworkRequest(QUrl::fromEncoded(url.toUtf8())));
    */
    QUrl url(_server + "?ajax=true");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = net->post(request, "text=" + QUrl::toPercentEncoding(msg));
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

int AJAXChat::maxWriteLength()
{
    return _maxWriteLength;
}

void AJAXChat::changeNick(const QString &newNick)
{
    if (!newNick.isEmpty()) {
        write("/nick " + newNick);
    }
}

QDomElement AJAXChat::getUser(const int &userID)
{
    return users.value(userID);
}

QDomElement AJAXChat::getUser(const QString &userName)
{
    QHash<int, QDomElement>::const_iterator i;
    for (i = users.constBegin(); i != users.constEnd(); i++) {
        QDomElement user = i.value();
        if (user.text() == userName) { return user; }
    }

    return QDomElement();
}

/** Get a message node element, extrapolate its type (public msg, private msg, channelEnter, etc.) and emit
    the specific signal */
void AJAXChat::emitChatData(const QDomElement &message)
{
    QString messageText = message.firstChildElement("text").text();
    // decode html entities from the message text and simplify it
    QTextEdit t;
    t.setHtml(messageText);
    messageText = t.toPlainText().simplified();

    if (messageText.startsWith("/")) {
        QStringList messageParts = messageText.split(" ");
        if (messageText.startsWith("/privmsg")) {
            const int userID = message.attribute("userID").toInt();
            emit newPrivateMessage(messageParts.at(1), getUser(userID).text());
        } else if (messageText.startsWith("/login")) {
            emit userLoggedIn(messageParts.at(1));
        } else if (messageText.startsWith("/logout")) {
            emit userLoggedOut(messageParts.at(1));
            qDebug("Logged out: " + getUser(messageParts.at(1)).text().toUtf8());
        } else if (messageText.startsWith("/channelEnter")) {
            emit userJoinChannel(messageParts.at(1));
        } else if (messageText.startsWith("/channelLeave")) {
            emit userLeaveChannel(messageParts.at(1));
        } else if (messageText.startsWith("/kick")) {
            emit userKicked(messageParts.at(1));
        } else if (messageText.startsWith("/nick")) {
            emit userChangeNick(messageParts.at(1), messageParts.at(2));
        }
    } else {
        const int userID = message.attribute("userID").toInt();
        emit newPublicMessage(messageText, getUser(userID).text());
    }
}
