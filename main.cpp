#include "mainwindow.h"
#include <QApplication>
#include "generator.h"
#include <QObject>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Generator g;
    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    QObject::connect(&w, &MainWindow::generateRequest,
                     &g, &Generator::generate);
    QObject::connect(&g, &Generator::resultFeedback,
                     &w, &MainWindow::resultReceived);

    g.start();
    w.show();
    int ret = a.exec();

    g.quit();
    g.wait();
    return ret;
}
