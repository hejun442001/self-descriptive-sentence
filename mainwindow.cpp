#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gen(new Generater)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gen;
}

void MainWindow::on_genButton_clicked()
{
    QString text=gen->generate(ui->inputText->toPlainText());
    ui->outputText->setText(text);
}
