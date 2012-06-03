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

/** @return my name. */
QString Memory::name()
{
    return memoryDocument->documentElement().firstChildElement("name").text();
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
