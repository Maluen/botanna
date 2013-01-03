(function(botanna, chat) {
    this.botanna = botanna;
    this.chat = chat;
    // GDF properties
    this.gdfStarted = false;
    this.gdfUsers = [];
    this.currentGdfUser = undefined;
    this.currentGdfPhrase = [];
    
    // Fake constructor
    this.initialize = function() {
    }
    
    /** @param parameters array of strings with the command parameters */
    this.processCommand = function(commandType, command, parameters, userName) {  
        if (commandType != "gdf") { return; }
        
        if (command == "start") {
            return this.gdfStart();
        } else if (command == "stop") {
            return this.gdfStop();
        } else if (command == "recover") {
            return this.gdfRecover();
        } else if (command == "join") {
            return this.gdfUserJoin(userName);
        } else if (command == "leave") {
            // if a second argument is not specified, remove the author of the message
            var userToRemove = (parameters.length == 0) ? userName : parameters[0];
            return this.gdfUserLeave(userToRemove);
        } else if (command == "undo") {
            // rimuove l'ultimo messaggio
            return this.gdfUndo();
        } else if (command == "riepilogo") {
            // riepiloga la frase del GDF corrente
            return this.gdfRiepilogo();
        } else if (command == "ordine") {
            // lista utenti
            return this.gdfOrdine();
        } else if (command == "help") {
            var helpAbout = (parameters.length == 0) ? "" : parameters[0];
            return this.gdfHelp(helpAbout);
        } else if (command == "msg") {
            return this.gdfAddWords(parameters, userName);
        } else if (command == "skip") {
            return this.gdfSkipUser();
        }
    }
    
    this.gdfStart = function() {
        if (!this.gdfStarted) {
            this.chat.write("Che GDF sia! Per unirti al gioco scrivi !gdf join");
            this.gdfStarted = true;
            this.gdfUsers.length = 0; // clears the array
            this.currentGdfUser = 0;
            this.currentGdfPhrase.length = 0; // clears the array
        } else {
            this.chat.write("C'è già un gdf in corso.");
        }
    }
    
    this.gdfStop = function() {
        if (!this.gdfStarted) {
            this.chat.write("Il GDF non è attivo, non c'è niente da fermare.");
        } else {
            this.chat.write("GDF fermato.");
            this.gdfStarted = false;
        }
    }
    
    this.gdfRecover = function() {
        if (this.gdfStarted) {
            // the GDF is active, nothing to do
            this.chat.write("Il GDF è ancora attivo, dovresti recuperare la testa piuttosto -.-");
            return;
        }
        if (this.currentGdfPhrase.length == 0 && this.gdfUsers.length == 0) {
            this.chat.write("Il GDF è vuoto, non c'è niente da recuperare.");
            return;
        }

        this.gdfStarted = true;
        this.chat.write("GDF recuperato.");
    }
    
    this.gdfUserJoin = function(userName) {
        if (!this.gdfStarted) { return; }

        // si assicura che l'utente non ci sia già e individua la posizione del nuovo utente
        for (var i=0,l=this.gdfUsers.length; i<l; i++) {
            if (this.gdfUsers[i] == userName) {
                this.chat.write(userName + ": fai già parte del GDF...");
                return;
            }
        }

        this.gdfUsers.push(userName);
        this.chat.write(userName + " sei dentro!");
    }
    
    this.gdfUserLeave = function(userToRemove) {
        if (!this.gdfStarted) { return; }

        for (var i=0,l=this.gdfUsers.length; i<l; i++) {
            if (this.gdfUsers[i] == userToRemove) {
                this.gdfUsers.splice(i, 1); // remove the element at position i from the array, leaving no holes
                
                if (i == this.currentGdfUser) {
                    // l'utente che era di turno ha lasciato
                    this.stepNextGdfUser();
                } else if (i < this.currentGdfUser) {
                    this.currentGdfUser--;
                }
                this.chat.write(userToRemove + " non fa più parte del GDF.");
                return;
            }
        }
        this.chat.write("Non posso rimuovere " + userToRemove + "... non fa parte del GDF...");
    }
    
    this.gdfUndo = function() {
        if (!this.gdfStarted) { return; }
        
        if (this.currentGdfPhrase.length == 0) {
            this.chat.write("Non c'è niente da rimuovere. Banned!");
            return;
        }

        this.currentGdfPhrase.splice(this.currentGdfPhrase.length-1, 1); // remove the last element from the array
        this.stepPreviousGdfUser();
        this.chat.write("Ultimo messaggio rimosso.");
    }
    
    this.gdfRiepilogo = function() {
        if (this.currentGdfPhrase.length == 0) {
            this.chat.write("Non c'è niente da riepilogare...");
            return;
        }

        var phrase = "";
        var maxWriteLength = this.chat.maxWriteLength();
        for (var i=0,l=this.currentGdfPhrase.length; i<l; i++) {
            if (phrase.length + this.currentGdfPhrase[i].length > maxWriteLength) {
                // message is too long => cut it
                this.chat.write(phrase);
                phrase = "";
            }
            phrase += this.currentGdfPhrase[i] + " ";
        }
        this.chat.write(phrase);
    }
    
    this.gdfOrdine = function() {
        if (!this.gdfStarted) { return; }

        if (this.gdfUsers.length == 0) {
            this.chat.write("Doh! Non vedo utenti!");
        } else {
            var toSend = "";
            for (var i=0,l=this.gdfUsers.length; i<l; i++) {
                var userName = this.gdfUsers[i];
                if (this.currentGdfUser == i) {
                    // utente corrente
                    toSend += "[b]" + userName + "[/b]";
                } else {
                    toSend += userName;
                }
                if (i != this.gdfUsers.length - 1) {
                    toSend += ", ";
                }
            }
            this.chat.write(toSend);
        }
    }
    
    this.gdfHelp = function(command) {
        if (!command) {
            this.chat.write("!gdf help seguito da uno dei seguenti comandi:" +
                           " start, stop, recover, join, leave, msg, undo, skip, riepilogo, ordine, help");
        } else if (command == "start") {
            this.chat.write("Avvia il GDF.");
        } else if (command == "stop") {
            this.chat.write("Ferma il GDF. Richiede votazione.");
        } else if (command == "recover") {
            this.chat.write("Permette di recuperare un GDF che è stato fermato incidentalmente.");
        } else if (command == "join") {
            this.chat.write("Con join ti unisci al GDF in corso.");
        } else if (command == "leave") {
            this.chat.write("Con leave \"utente\" (senza apici) rimuovi l'utente dal GDF corrente, senza parametri" +
                           " rimuove te.");
        } else if (command == "msg") {
            this.chat.write("Quando è il tuo turno puoi scrivere nel GDF con !gdf msg MESSAGGIO." +
                           " Attenzione: puoi inserire al massimo due parole.");
        } else if (command == "undo") {
            this.chat.write("Annulla l'ultimo messaggio inserito.");
        } else if (command == "skip") {
            this.chat.write("Salta un turno e passa quindi al prossimo utente del GDF secondo l'ordine.");
        } else if (command == "riepilogo") {
            this.chat.write("Mostra il contenuto, anche parziale, dell'ultimo GDF.");
        } else if (command == "ordine") {
            this.chat.write("Mostra l'ordine degli utenti nel GDF. La (x) a sinistra indica che l'utente " +
                           " è quello che deve scrivere il prossimo messaggio.");
        } else if (command == "help") {
            this.chat.write("Secondo te? Chiedere aiuto sull'aiuto mostra evidenti segnali di niubbiagine...");
        }
    }
    
    this.gdfAddWords = function(words, userName) {
        if (!this.gdfStarted || this.gdfUsers.length == 0 
            || userName != this.gdfUsers[this.currentGdfUser] || words.length == 0)
        {
            return userName;
        }

        var maxWordsLength = 3;
        if (words.length > maxWordsLength) {
            this.chat.write("Troppe parole " + userName + ". Puoi scriverne al massimo " + maxWordsLength);
            return;
        }

        this.currentGdfPhrase.push(words.join(" ").toUpperCase());
        this.stepNextGdfUser();
        this.chat.write("Bene, vai " + "[b]"+this.gdfUsers[this.currentGdfUser]+"[/b]");
    }
    
    this.gdfSkipUser = function() {
        if (!this.gdfStarted || this.gdfUsers.length == 0) { return; }

        this.stepNextGdfUser();
        this.chat.write("OK, vai " + "[b]" + this.gdfUsers[this.currentGdfUser] + "[/b]");
    }
    
    this.stepNextGdfUser = function() {
        this.currentGdfUser++;
        if (this.currentGdfUser >= this.gdfUsers.length) {
            this.currentGdfUser = 0;
        }
    }
    
    this.stepPreviousGdfUser = function() {
        this.currentGdfUser--;
        if (this.currentGdfUser < 0) {
            if (this.gdfUsers.length == 0) {
                this.currentGdfUser = 0;
            } else {
                this.currentGdfUser = this.gdfUsers.length - 1;
            }
        }
    }
    
    this.initialize(); // manually calls the fake constructor
})
