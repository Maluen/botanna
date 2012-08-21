#ifndef BOTANNA_H
#define BOTANNA_H

#include <QObject>
#include <QDomElement>
#include <QStringList>

class AJAXChat;
class QTimer;
class Memory;

class Botanna : public QObject
{
    Q_OBJECT

public:
    Botanna(QObject *parent = 0);
    ~Botanna();
    Memory *getMemory();
    QString getName();
    QString getChatLocation();
    QString getChatUsername();
    QString getChatPassword();
    AJAXChat *getChat();
    void connectToChat();
private:
    void gdfExecuteCommand(const QString &command, const QStringList &parameters, const QString &userName);
    void gdfHelp(const QString &command);
    void gdfStart();
    void gdfStop();
    void gdfRecover();
    void gdfUserJoin(const QString &userName);
    void gdfOrdine();
    void gdfAddWords(const QStringList &words, const QString &userName);
    void gdfUndo();
    void gdfSkipUser();
    void gdfRiepilogo();
    void stepNextGdfUser();
    void stepPreviousGdfUser();
private slots:
    void joinPublic(bool connected);
    void processPublicMessage(QString &message, const QString &userName);
    void gdfUserLeave(const QString &userToRemove);
    void updateGDFUser(const QString &oldUserName, const QString &newUserName);

private:
    Memory *memory;
    QString name;
    QString chatLocation;
    QString chatUsername;
    QString chatPassword;
    QString forumLoginUrl;
    QString forumUsername;
    QString forumPassword;
    AJAXChat *chat;
    bool gdfStarted;
    QList<QString> gdfUsers; // the usernames
    int currentGdfUser;
    QStringList currentGdfPhrase;
};

#endif // BOTANNA_H
