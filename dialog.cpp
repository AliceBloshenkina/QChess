#include "dialog.h"
#include "ui_dialog.h"
#include<QMessageBox>
#include <QTimer>
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    QString name_1 = ui->lineEdit->text();
    QString name_2 = ui->lineEdit_2->text();
    if(name_1.isEmpty() && !name_2.isEmpty())
    {
        QMessageBox::warning(this,"Авторизация" , "Не заполнили ник 1 игрока!!!");
    } else if(!name_1.isEmpty() && name_2.isEmpty())
    {
        QMessageBox::warning(this,"Авторизация" , "Не заполнили ник 2 игрока!!!");
    } else if(name_1.isEmpty() && name_2.isEmpty()){
        QMessageBox::warning(this,"Авторизация" , "Не заполнили ники игроков!!!");
    } else if(name_1 == name_2){
        QMessageBox::warning(this,"Авторизация" , "Совпадают ники игроков!!!");
    }else {
        hide();
        window = new GameWindow();
        window->setPlayerNamesAndRatings(name_1, name_2);
        window->show();
    }
}

