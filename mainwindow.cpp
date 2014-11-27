#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_genButton_clicked()
{
    ui->genButton->setText("生成中……");
    ui->genButton->setEnabled(false);
    emit generateRequest(ui->inputEdit->text());
}

void MainWindow::resultReceived(QString text)
{
    ui->outputText->setText(text);
    ui->genButton->setText("生成");
    ui->genButton->setEnabled(true);
}
