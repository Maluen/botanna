#ifndef MEMORY_H
#define MEMORY_H

/**
  \class Memory
  \brief An abstract memory.

  Subclass it to define a memory based on a specific format (xml, sql, plain text, etc.)
*/

#include <QObject>

class QDomDocument;

class Memory : QObject
{
public:
    Memory(QObject *parent=0);
    QString name();
    QString chatLocation();
    QString chatUsername();
    QString chatPassword();
    QString chatInfo(const QString &infoType);
    QString forumLoginUrl();
    QString forumUsername();
    QString forumPassword();
    QString forumInfo(const QString &infoType);
    QString info(const QString &infoCategory, const QString &infoType);
    QString alias(const QString &text);

private:
    QDomDocument *memoryDocument;
};

#endif // MEMORY_H
