#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include"gamewindow.h"
#include"history.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_2_clicked()
{
    History *window = new History();
    connect(window, &History::closed, this, &MainWindow::onWindowClosed);
    hide();
    window->show();
}


void MainWindow::on_pushButton_clicked()
{
    window = new Dialog();
    hide();
    window->show();
}

void MainWindow::onWindowClosed()
{
    show();
}
