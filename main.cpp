#include "mainwindow.h"
#include <QApplication>
#include "generator.h"
#include <QObject>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    MainWindow w;
    Generator g;

    QObject::connect(&w, &MainWindow::generateRequest,
                     &g, &Generator::generate);
    QObject::connect(&a, &QApplication::aboutToQuit,
                     &g, &QThread::quit);

    g.start();
    w.show();

    return a.exec();
}
