#include "Logic.h"
#include "Botanna.h"
#include "AJAXChat.h"
#include <QScriptValue>
#include <QDir>
#include <QFile>
#include <QDebug>

Logic::Logic(Botanna *botanna)
{
    io = botanna;

    // Create javascript objects pointing to the c++ bot objects
    QScriptValue botannaScriptValue = engine.newQObject(botanna);
    QScriptValue chatScriptValue = engine.newQObject(botanna->getChat());

    // Retrieve the "process command" javascript objects
    QString scriptsDirectory = "logic/process-command";
    QDir dir(scriptsDirectory);
    QStringList fileNames = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for (int i=0; i<fileNames.length(); i++) {
        QString fileName = fileNames.at(i);

        QFile file(scriptsDirectory + "/" + fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Unable to open " + fileName;
            continue;
        }

        QString sourceCode = file.readAll();
        // source code is like "(function() { ... })" (javascript "class")
        QScriptValue objectClass = engine.evaluate(sourceCode);
        QScriptValue object = objectClass.construct(QScriptValueList() << botannaScriptValue << chatScriptValue);

        processCommandObjects.append(object);
    }
}

void Logic::processCommand(const QString &commandType, const QString &command,
                           const QStringList &parameters, const QString &userName)
{
    qDebug("Logic: processing command");

    // Scripts function arguments
    QScriptValueList args;
    args.append(commandType);
    args.append(command);
    args.append(qScriptValueFromSequence(&engine, parameters)); // javascript array from container (stringlist)
    args.append(userName);

    // Call all the script functions
    for (int i=0; i<processCommandObjects.size(); i++) {
        QScriptValue object = processCommandObjects.at(i);
        // the object has a processCommand property (that it's a function)

        QScriptValue processCommandFunction = object.property("processCommand");
        // processCommandFunction code is like
        // "this.processCommand = function(commandType, command, parameters, userName) { ... }"
        QScriptValue r = processCommandFunction.call(object, args);
        if (engine.hasUncaughtException()) {
            qDebug(engine.uncaughtException().toString().toUtf8());
        }

        qDebug(r.toString().toUtf8());
    }
}
