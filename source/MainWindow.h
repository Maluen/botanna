#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
    This window is basically just a viewer of the BOT computer, with the ability to "stole its chat"
    and perform some ations like change its nickname, disconnect, send messages, etc
*/

#include <QtGui/QMainWindow>

class Botanna;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QHBoxLayout;
class QVBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void changeNick();
    void disconnectConnect();
    void writeToChat();

private:
    QLabel *nickLabel;
    QLineEdit *nickLineEdit;
    QPushButton *changeNickButton;
    QPushButton *disconnectConnectButton;
    QHBoxLayout *chatOptionsLayout;
    QTextEdit *chatMessages;
    QLineEdit *chatWriteLineEdit;
    QPushButton *chatWriteButton;
    QHBoxLayout *chatWriteLayout;
    QVBoxLayout *centralLayout;
    Botanna *botanna;
};

#endif // MAINWINDOW_H
