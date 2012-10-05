#ifndef LOGIC_H
#define LOGIC_H

#include <QScriptEngine>
#include <QScriptProgram>
#include <QStringList>

class QString;
class Botanna;

class Logic
{
public:
    Logic(Botanna *botanna);
    void processCommand(const QString &commandType, const QString &command,
                        const QStringList &parameters, const QString &userName);

private:
    Botanna *io;
    QScriptEngine engine;
    QList<QScriptValue> processCommandObjects;
};

#endif // LOGIC_H
