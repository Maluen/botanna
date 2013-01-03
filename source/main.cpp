#include <QtGui/QApplication>
#include "MainWindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Sets "UTF-8" as the codec used by QString and tr() to convert to and from const char *
    QTextCodec *UTF8Codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(UTF8Codec);
    QTextCodec::setCodecForTr(UTF8Codec);

    MainWindow w;
    w.show();

    return a.exec();
}
