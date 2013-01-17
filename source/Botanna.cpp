#include "Botanna.h"
#include "Memory.h"
#include "Logic.h"
#include "AJAXChat.h"
#include <QList>
#include <QString>
#include <QRegExp>

// TODO: modificare userID con il nome utente (nella realtà l'utente vede il nome e non l'id) e quindi tutte le varie
// funzioni che lo usano, per il cambio nick bisogna collegarsi all'apposito segnale e fare il cambio nella lista utenti gdf.

Botanna::Botanna(QObject *parent) : QObject(parent)
{
    // Fetch stuff from memory
    memory = new Memory(this);
    name = memory->name();
    if (name.isEmpty()) {
        // stop
        // TODO
    }
    chatLocation = memory->chatLocation();
    if (chatLocation.isEmpty()) {
        // stop
        // TODO
    }
    // the channel can be empty
    chatChannel = memory->chatChannel();
    chatUsername = memory->chatUsername();
    if (chatUsername.isEmpty()) {
        // stop
        // TODO
    }
    chatPassword = memory->chatPassword();
    if (chatPassword.isEmpty()) {
        // stop
        // TODO
    }
    forumLoginUrl = memory->forumLoginUrl();
    if (forumLoginUrl.isEmpty()) {
        // nothing to do, can be empty
    }

    // setup the chat client
    chat = new AJAXChat(this);
    connect(chat, SIGNAL(newPublicMessage(QString&,QString)), this, SLOT(processPublicMessage(QString&,QString)));
    connect(chat, SIGNAL(userChangeNick(QString,QString)), this, SLOT(updateGDFUser(QString,QString)));
    connect(chat, SIGNAL(userLoggedOut(QString)), this, SLOT(gdfUserLeave(QString)));
    connect(chat, SIGNAL(userKicked(QString)), this, SLOT(gdfUserLeave(QString)));
    connect(chat, SIGNAL(userLeaveChannel(QString)), this, SLOT(gdfUserLeave(QString)));

    // Here we go, Anna puts his hands on the computer
    connectToChat();

    gdfStarted = false;
    currentGdfUser = 0;

    // Logic
    logic = new Logic(this);
    /*logic->processCommand("gdf", "start", QStringList(), "MightyBlue");
    logic->processCommand("gdf", "join", QStringList(), "MightyBlue");
    logic->processCommand("gdf", "join", QStringList(), "ddsx");
    logic->processCommand("gdf", "ordine", QStringList(), "MightyBlue");
    logic->processCommand("gdf", "msg", QStringList() << "oh" << "yeah" << "aa", "MightyBlue"); // valid message
    logic->processCommand("gdf", "msg", QStringList() << "lo" << "lo" << "lo" << "lo", "ddsx"); // too long message
    logic->processCommand("gdf", "riepilogo", QStringList(), "MightyBlue");
    exit(1);*/
}

Botanna::~Botanna()
{
    chat->quit();
}

Memory *Botanna::getMemory()
{
    return memory;
}

QString Botanna::getName()
{
    return name;
}

QString Botanna::getChatLocation()
{
    return chatLocation;
}

QString Botanna::getChatUsername()
{
    return chatUsername;
}

QString Botanna::getChatPassword()
{
    return chatPassword;
}

AJAXChat *Botanna::getChat()
{
    return chat;
}

void Botanna::connectToChat()
{
    if (chat != NULL && !chatLocation.isEmpty() && !chatUsername.isEmpty()) {
        // password can be empty
        chat->connectToServer(chatLocation, chatUsername, chatPassword, chatChannel, forumLoginUrl);
    }
}

void Botanna::joinPublic(bool connected)
{
    if (!connected) {
        // error
    } else {
        chat->join(chatChannel);
    }
}

void Botanna::processPublicMessage(QString &message, const QString &userName)
{
    if (userName == name) {
        // skip my messages
        return;
    }

    // replace the message with the alias in memory, if exists
    QString aliasMessage = memory->alias(message);
    if (!aliasMessage.isEmpty()) {
        message = aliasMessage;
    }

    if (message.startsWith("!")) { // the message is a command
        QStringList messageParts = message.split(" ");
        if (messageParts.length() < 2) {
            // too few parts
            return;
        }
        const QString &commandType = messageParts.at(0).mid(1); // also removes the starting "!" character
        const QString &command = messageParts.at(1);
        QStringList parameters = messageParts.mid(2, -1); // from third element to end of list

        // execute logic for command processing
        //logic->processCommand(commandType, command, parameters, userName);

        if (commandType == "gdf") {
            gdfExecuteCommand(command, parameters, userName);
        }
    } else {
        // normal message
    }
}

/** User con eventuali parentesi tolte */
void Botanna::gdfExecuteCommand(const QString &command, const QStringList &parameters, const QString &userName)
{
    if (command == "start") {
        gdfStart();
    } else if (command == "stop") {
        gdfStop();
    } else if (command == "recover") {
        gdfRecover();
    } else if (command == "join") {
        gdfUserJoin(userName);
    } else if (command == "leave") {
        // if a second argument is not specified, remove the author of the message
        QString userToRemove = parameters.isEmpty() ? userName : parameters.at(0);
        gdfUserLeave(userToRemove);
    } else if (command == "rounds") {
        if (parameters.isEmpty()) {
            // informa sul numero di round del gdf corrente
            gdfWriteRoundsNum();
        } else {
            // imposta il numero di round del gdf corrente
            int roundsNum = parameters.at(0).toInt();
            gdfSetRoundsNum(roundsNum);
        }
    } else if (command == "undo") {
        // rimuove l'ultimo messaggio
        gdfUndo();
    } else if (command == "riepilogo") {
        // riepiloga la frase del GDF corrente
        gdfRiepilogo();
    } else if (command == "ordine") {
        // lista utenti
        gdfOrdine();
    } else if (command == "help") {
        QString helpAbout = parameters.isEmpty() ? "" : parameters.at(0);
        gdfHelp(helpAbout);
    } else if (command == "msg") {
        gdfAddWords(parameters, userName);
    } else if (command == "skip") {
        gdfSkipUser();
    }
}

void Botanna::gdfStart()
{
    if (!gdfStarted) {
        chat->write(tr("Che GDF sia! Per unirti al gioco scrivi !gdf join"));
        gdfStarted = true;
        gdfUsers.clear();
        currentGdfUser = 0;
        currentGdfPhrase.clear();
        gdfRoundsNum = 0;
        gdfRoundsCompleted = 0;
    } else {
        chat->write(tr("C'è già un gdf in corso."));
    }
}

void Botanna::gdfStop()
{
    if (!gdfStarted) {
        chat->write(tr("Il GDF non è attivo, non c'è niente da fermare."));
    } else {
        chat->write(tr("GDF fermato."));
        gdfStarted = false;
    }
}

/** Permette di continuare un GDF che è stato fermato incidentalmente. */
void Botanna::gdfRecover()
{
    if (gdfStarted) {
        // the GDF is active, nothing to do
        chat->write(tr("Il GDF è ancora attivo, dovresti recuperare la testa piuttosto -.-"));
        return;
    }
    if (currentGdfPhrase.isEmpty() && gdfUsers.isEmpty()) {
        chat->write(tr("Il GDF è vuoto, non c'è niente da recuperare."));
        return;
    }

    gdfStarted = true;
    chat->write(tr("GDF recuperato."));
}

void Botanna::gdfUserJoin(const QString &userName)
{
    if (!gdfStarted) { return; }

    // si assicura che l'utente non ci sia già e individua la posizione del nuovo utente
    for (int i=0; i<gdfUsers.length(); i++) {
        if (gdfUsers.at(i) == userName) {
            chat->write(userName + tr(": fai già parte del GDF..."));
            return;
        }
    }

    gdfUsers.append(userName);
    chat->write(userName + tr(" sei dentro!"));
}

void Botanna::gdfWriteRoundsNum()
{
    if (!gdfStarted) { return; }

    if (gdfRoundsNum == 0) {
        chat->write(tr("Questo GDF non ha limiti sul numero di round!"));
    } else {
        chat->write(tr("Il numero di round di questo GDF è pari a ") + QString::number(gdfRoundsNum)
                    + tr(", di cui ") + QString::number(gdfRoundsCompleted) + tr(" già completati/o."));
    }
}

void Botanna::gdfSetRoundsNum(int roundsNum)
{
    if (!gdfStarted) { return; }

    if (roundsNum <= gdfRoundsCompleted) {
        chat->write(tr("Inserire un numero di round maggiore di ")
                    + QString::number(gdfRoundsCompleted)
                    + tr(" (i round già completati)"));
        return;
    }

    gdfRoundsNum = roundsNum;
    chat->write(tr("Numero di round del GDF corrente impostati a ")
                + QString::number(roundsNum));
}

void Botanna::gdfUserLeave(const QString &userToRemove)
{
    if (!gdfStarted) { return; } 

    for (int i=0; i<gdfUsers.length(); i++) {
        if (gdfUsers.at(i) == userToRemove) {
            gdfUsers.removeAt(i);
            if (i == currentGdfUser) {
                // l'utente che era di turno ha lasciato
                stepNextGdfUser();
            } else if (i < currentGdfUser) {
                currentGdfUser--;
            }
            chat->write(userToRemove + tr(" non fa più parte del GDF."));
            return;
        }
    }
    //chat->write(tr("Non posso rimuovere ") + userToRemove + tr("... non fa parte del GDF..."));
}

/** Remove the last message */
void Botanna::gdfUndo()
{
    if (!gdfStarted) { return; }
    if (currentGdfPhrase.isEmpty()) {
        chat->write(tr("Non c'è niente da rimuovere. Banned!"));
        return;
    }

    currentGdfPhrase.removeLast();
    stepPreviousGdfUser();
    chat->write(tr("Ultimo messaggio rimosso."));
}

void Botanna::gdfRiepilogo()
{
    if (currentGdfPhrase.isEmpty()) {
        chat->write("Non c'è niente da riepilogare...");
        return;
    }

    QString phrase;
    int maxWriteLength = chat->maxWriteLength();
    for (int i=0; i<currentGdfPhrase.length(); i++) {
        if (phrase.length() + currentGdfPhrase.at(i).length() > maxWriteLength) {
            // message is too long => cut it
            chat->write(phrase);
            phrase = "";
        }
        phrase.append(currentGdfPhrase.at(i) + " ");
    }
    chat->write(phrase);
}

void Botanna::gdfOrdine()
{
    if (!gdfStarted) { return; }

    if (gdfUsers.isEmpty()) {
        chat->write(tr("Doh! Non vedo utenti!"));
    } else {
        QString toSend;
        for (int i=0; i<gdfUsers.length(); i++) {
            QString userName = gdfUsers.at(i);
            if (currentGdfUser == i) {
                // utente corrente
                toSend += "[b]" + userName + "[/b]";
            } else {
                toSend += userName;
            }
            if (i != gdfUsers.length() - 1) {
                toSend += ", ";
            }
        }
        chat->write(toSend);
    }
}

void Botanna::gdfHelp(const QString &command)
{
    if (command.isEmpty()) {
        chat->write(tr("!gdf help seguito da uno dei seguenti comandi:"
                       " start, stop, recover, join, rounds, leave, msg, undo, skip, riepilogo, ordine, help"));
    } else if (command == "start") {
        chat->write(tr("Avvia il GDF."));
    } else if (command == "stop") {
        chat->write(tr("Ferma il GDF. Richiede votazione."));
    } else if (command == "recover") {
        chat->write(tr("Permette di recuperare un GDF che è stato fermato incidentalmente."));
    } else if (command == "join") {
        chat->write(tr("Con join ti unisci al GDF in corso."));
    } else if (command == "rounds") {
        chat->write(tr("Informa sul numero di round del GDF corrente se chiamato senza argomenti, "
                       "altrimenti imposta il numero di round."));
    } else if (command == "leave") {
        chat->write(tr("Con leave \"utente\" (senza apici) rimuovi l'utente dal GDF corrente, senza parametri"
                       " rimuove te."));
    } else if (command == "msg") {
        chat->write(tr("Quando è il tuo turno puoi scrivere nel GDF con !gdf msg MESSAGGIO."
                       " Attenzione: puoi inserire al massimo due parole."));
    } else if (command == "undo") {
        chat->write(tr("Annulla l'ultimo messaggio inserito."));
    } else if (command == "skip") {
        chat->write(tr("Salta un turno e passa quindi al prossimo utente del GDF secondo l'ordine."));
    } else if (command == "riepilogo") {
        chat->write(tr("Mostra il contenuto, anche parziale, dell'ultimo GDF."));
    } else if (command == "ordine") {
        chat->write(tr("Mostra l'ordine degli utenti nel GDF. La (x) a sinistra indica che l'utente "
                       " è quello che deve scrivere il prossimo messaggio."));
    } else if (command == "help") {
        chat->write(tr("Secondo te? Chiedere aiuto sull'aiuto mostra evidenti segnali di niubbiagine..."));
    }
}

void Botanna::gdfAddWords(const QStringList &words, const QString &userName)
{
    if (!gdfStarted || gdfUsers.length() == 0 || userName != gdfUsers.at(currentGdfUser) || words.isEmpty()) {
        return;
    }

    // controlla se non sono già stati completati tutti i round
    if (gdfRoundsNum != 0 && gdfRoundsCompleted == gdfRoundsNum) {
        chat->write(tr("Non puoi! Tutti i round sono già stati completati."));
        return;
    }

    if (words.length() > 3) {
        chat->write(tr("Troppe parole ") + userName + tr(". Puoi scriverne al massimo 2"));
        return;
    }

    currentGdfPhrase.append(words.join(" ").toUpper());

    // controlla se è stato completato un round
    if (gdfRoundsNum != 0 && gdfUsers.last() == userName) {
        gdfRoundsCompleted++;
        if (gdfRoundsCompleted == gdfRoundsNum) {
            // tutti i round sono stati completati, questo è l'ultimo messaggio del gdf!
            chat->write(tr("Bene ") + "[b]" + gdfUsers.at(currentGdfUser) + "[/b], round finiti!");
            gdfStop();
            return;
        }
    }

    stepNextGdfUser();
    chat->write(tr("Bene, vai ") + "[b]" + gdfUsers.at(currentGdfUser) + "[/b]");

    // all'inizio di un nuovo round, scrive un messaggio con il numero di round rimasti
    if (gdfRoundsNum != 0 && gdfUsers.last() == userName) {
        int roundsLeft = gdfRoundsNum-gdfRoundsCompleted;
        if (roundsLeft > 1) {
            chat->write(tr("Mancano ancora ")
                        + QString::number(roundsLeft)
                        +  tr(" round!"));
        } else {
            chat->write(tr("Ultimo round!!"));
        }
    }
}

/** Salta un turno e passa quindi al prossimo utente del GDF secondo l'ordine. */
void Botanna::gdfSkipUser()
{
    if (!gdfStarted || gdfUsers.isEmpty()) { return; }

    stepNextGdfUser();
    chat->write(tr("OK, vai ") + "[b]" + gdfUsers.at(currentGdfUser) + "[/b]");
}

void Botanna::stepNextGdfUser()
{
    currentGdfUser++;
    if (currentGdfUser >= gdfUsers.length()) {
        currentGdfUser = 0;
    }
}

void Botanna::stepPreviousGdfUser()
{
    currentGdfUser--;
    if (currentGdfUser < 0) {
        if (gdfUsers.length() == 0) {
            currentGdfUser = 0;
        } else {
            currentGdfUser = gdfUsers.length() - 1;
        }
    }
}

/** An user has changed its username => update it on the gdf users list */
void Botanna::updateGDFUser(const QString &oldUserName, const QString &newUserName)
{
    for (int i=0; i<gdfUsers.length(); i++) {
        if (gdfUsers.at(i) == oldUserName) {
            gdfUsers[i] = newUserName;
            return;
        }
    }
}
