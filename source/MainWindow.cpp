#include "MainWindow.h"
#include "Botanna.h"
#include <QtGui>
#include "Memory.h"
#include "AJAXChat.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    botanna = new Botanna(this);

    nickLabel = new QLabel(tr("Nickname: "));
    nickLineEdit = new QLineEdit(botanna->getMemory()->chatUsername());
    changeNickButton = new QPushButton(tr("Cambia"));
    connect(changeNickButton, SIGNAL(clicked()), this, SLOT(changeNick()));

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

void MainWindow::changeNick()
{
    QString newNick = nickLineEdit->text();

    if (!newNick.isEmpty()) {
        // change nick
        botanna->getChat()->changeNick(newNick);
    }
}

void MainWindow::disconnectConnect()
{
    if (botanna->getChat()->isConnected()) {
        // disconnect
        botanna->getChat()->quit();
        disconnectConnectButton->setText(tr("Connetti"));
    } else {
        // connect
        botanna->connectToChat();
        disconnectConnectButton->setText(tr("Disconnetti"));
    }
}

void MainWindow::writeToChat()
{
    if (!chatWriteLineEdit->text().isEmpty()) {
        botanna->getChat()->write(chatWriteLineEdit->text());
        chatWriteLineEdit->clear();
    }
}
