#ifndef CHAT_H
#define CHAT_H

/**
  \class Chats
  \brief An abstract class for a Chat client.

  Subclass it to define a class for a specific protocol (irc, msn, ajax, etc)
*/

#include <QObject>
#include <QString>
#include <QList>
#include <QDomElement>
#include <QStringList>
#include <QHash>

class QNetworkAccessManager;
class QTimer;

class Chat : public QObject
{
    Q_OBJECT

public:
    Chat(QObject *parent=0);
    void connectToServer(const QString &server, const QString &userName, const QString &password, const QString &channel=tr("Pubblico"));
    void quit();
    bool isConnected();
    void join(const QString &roomName);
    void leave(const QString &roomName);
    void write(const QString &msg, const QString &roomName="");
    int maxWriteLength();
    QDomElement getUser(const int &userID);
    QDomElement getUser(const QString &userName);
    void emitChatData(const QDomElement &message);
public slots:
    void connectFinished();
private slots:
    void read(const QString &roomName=""); // reads periodically by using the readTimer
    void readFinished();

signals:
    void connectFinished(bool status);
    void joinFinished(bool status);
    void newPublicMessage(QString &message, const QString &userName);
    void newPrivateMessage(const QString &message, const QString &userName);
    void userLoggedIn(const QString &userName);
    void userLoggedOut(const QString &userName);
    void userJoinChannel(const QString &userName);
    void userLeaveChannel(const QString &userName);
    void userKicked(const QString &userName);
    void userChangeNick(const QString &oldUserName, const QString &newUserName);

private:
    QNetworkAccessManager *net;
    QString _server;
    QTimer *readTimer;
    int _maxWriteLength; // maximum number of chars allowed in a single message
    int lastMsgId;
    QHash<int, QDomElement> users;
};

#endif // CHAT_H
