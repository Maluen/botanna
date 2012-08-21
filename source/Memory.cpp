#include "Memory.h"
#include <QDomDocument>
#include <QFile>
#include <QString>
#include <QDomNodeList>
#include <QRegExp>
#include <QDomElement>

const QString memoryDocumentFileName = "memory.xml";

Memory::Memory(QObject *parent) : QObject(parent)
{
    QFile memoryDocumentFile(memoryDocumentFileName);
    if (!memoryDocumentFile.open(QIODevice::ReadOnly)) {
        qFatal(tr("Impossibile aprire %1").arg(memoryDocumentFileName).toAscii());
        return;
    }
    memoryDocument = new QDomDocument;
    if (!memoryDocument->setContent(memoryDocumentFile.readAll())) {
        qFatal(tr("File xml della memoria non valido.").toAscii());
        return;
    }

    if (name().isEmpty()) {
        qFatal(tr("Inserire il nome del bot nel file xml!").toAscii());
        return;
    }
}

/** @return my name or an empty string if there isn't one. */
QString Memory::name()
{
    QDomElement el = memoryDocument->documentElement().firstChildElement("name");
    return el.isNull() ? "" : el.text();
}

/** @return the location of the chat where I like to talk.
    @return an empty string if there is no location. */
QString Memory::chatLocation()
{
    return chatInfo("location");
}

/** @return the username of the account on the chat where I like to talk.
    @return an empty string if there is no username. */
QString Memory::chatUsername()
{
    return chatInfo("username");
}

/** @return the password of the account on the chat where I like to talk.
    @return an empty string if there is no password. */
QString Memory::chatPassword()
{
    return chatInfo("password");
}

/**
  Handy function for getting a specific chat info, e.g. location, username, password, etc.
  @param infoType the info to get
  @return the info or NULL if it doesn't exist
*/
QString Memory::chatInfo(const QString &infoType)
{
    return info("chat", infoType);
}

/** @return the parametric url used to login in the forum.
    Parameter syntax is %Parameter%, e.g. %Username%, %Password% **/
QString Memory::forumLoginUrl()
{
    return forumInfo("loginUrl");
}

/** @return the username used in the forum **/
QString Memory::forumUsername()
{
    return forumInfo("username");
}

/** @return the password used in the forum **/
QString Memory::forumPassword()
{
    return forumInfo("password");
}

/**
  Handy function for getting a specific forum info, e.g. loginUrl, username, password, etc.
  @param infoType the info to get
  @return the info or NULL if it doesn't exist
*/
QString Memory::forumInfo(const QString &infoType)
{
    return info("forum", infoType);
}

/**
  Handy function for getting a specific info,
  @param infoCategory the category of the info to get, e.g. chat, forum
  @param infoType the info to get, e.g. username, password, etc.
  @return the info or NULL if it doesn't exist
*/
QString Memory::info(const QString &infoCategory, const QString &infoType)
{
    QString info = "";

    QDomElement el = memoryDocument->documentElement().firstChildElement(infoCategory);
    if (!el.isNull()) {
        el = el.firstChildElement(infoType);
        info = el.text();
    }

    return info;
}

/** @return the alias that match with the specified text, empty if doesn't exist. */
QString Memory::alias(const QString &text)
{
    QRegExp regExp;
    regExp.setMinimal(true);

    QString textAlias;
    QDomNodeList aliasList = memoryDocument->documentElement().elementsByTagName("alias");
    for (int i=0; i<aliasList.count(); i++) {
        QDomElement element = aliasList.at(i).toElement();
        QString pattern = element.firstChildElement("pattern").text();
        if (pattern.isEmpty()) { continue; }
        pattern.replace("%botname%", name());
        regExp.setPattern(pattern);
        if (regExp.exactMatch(text)) {
            // alias found
            textAlias = element.firstChildElement("value").text();
        }
    }
    return textAlias;
}
