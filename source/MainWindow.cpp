#include "MainWindow.h"
#include "Botanna.h"
#include <QtGui>
#include "Memory.h"
#include "Chat.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    botanna = new Botanna(this);

    nickLabel = new QLabel(tr("Nickname: "));
    nickLineEdit = new QLineEdit(botanna->getMemory()->name());
    changeNickButton = new QPushButton(tr("Cambia"));
    disconnectConnectButton = new QPushButton(tr("Disconnetti"));
    connect(disconnectConnectButton, SIGNAL(clicked()), this, SLOT(disconnectConnect()));
    chatOptionsLayout = new QHBoxLayout;
    chatOptionsLayout->addWidget(nickLabel);
    chatOptionsLayout->addWidget(nickLineEdit);
    chatOptionsLayout->addWidget(changeNickButton);
    chatOptionsLayout->addStretch();
    chatOptionsLayout->addWidget(disconnectConnectButton);

    chatMessages = new QTextEdit;
    chatMessages->setReadOnly(true);
    chatMessages->setAcceptRichText(false);

    chatWriteLineEdit = new QLineEdit;
    chatWriteButton = new QPushButton(tr("Invia"));
    connect(chatWriteButton, SIGNAL(clicked()), this, SLOT(writeToChat()));
    chatWriteLayout = new QHBoxLayout;
    chatWriteLayout->addWidget(chatWriteLineEdit);
    chatWriteLayout->addWidget(chatWriteButton);

    centralLayout = new QVBoxLayout;
    centralLayout->addLayout(chatOptionsLayout);
    centralLayout->addWidget(chatMessages);
    centralLayout->addLayout(chatWriteLayout);
    QWidget *central = new QWidget();
    central->setLayout(centralLayout);
    setCentralWidget(central);
}

MainWindow::~MainWindow()
{
    botanna->deleteLater();
}

void MainWindow::disconnectConnect()
{
    if (botanna->getChat()->isConnected()) {
        // disconnect
        botanna->getChat()->quit();
    } else {
        // connect
        botanna->deleteLater();
        botanna = new Botanna(this);
    }
}

void MainWindow::writeToChat()
{
    if (!chatWriteLineEdit->text().isEmpty()) {
        botanna->getChat()->write(chatWriteLineEdit->text());
        chatWriteLineEdit->clear();
    }
}
