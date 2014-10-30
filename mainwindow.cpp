#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gen(new Generator)
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
    ui->genButton->setText("生成中……");
    ui->genButton->setEnabled(false);
    QString text=gen->generate(ui->inputText->toPlainText());
    ui->outputText->setText(text);
    ui->genButton->setText("生成");
    ui->genButton->setEnabled(true);
}
