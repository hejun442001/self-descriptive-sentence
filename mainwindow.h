#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "generator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void generateRequest(QString);

private slots:
    void on_genButton_clicked();

public slots:
    void resultFeedback(QString text);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
