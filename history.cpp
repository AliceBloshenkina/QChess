#include "history.h"
#include "ui_history.h"

History::History(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::History)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./Player.db");

    history = new QSqlQuery(db);
    history->exec("CREATE TABLE History(NICK1 TEXT, NICK2 TEXT, WINNER TEXT);");
    model = new QSqlTableModel(this, db);
    model->setTable("History");
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);}

void History::closeEvent(QCloseEvent *event)
{
    emit closed();
    event->accept();
}

History::~History()
{
    delete ui;
}
