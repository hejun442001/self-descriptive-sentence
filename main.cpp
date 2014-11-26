#include "mainwindow.h"
#include <QApplication>
#include "generator.h"
#include <QObject>
#include <QTextCodec>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    MainWindow w;
    Generator g;
    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    QObject::connect(&w, &MainWindow::generateRequest,
                     &g, &Generator::generate);
    QObject::connect(&g, &Generator::resultFeedback,
                     &w, &MainWindow::resultReceived);

    w.show();

    return a.exec();
}
