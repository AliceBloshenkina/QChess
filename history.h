#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>
#include <QSqlDatabase>
#include <QCloseEvent>
#include <QSqlQuery>
#include <QSqlTableModel>
namespace Ui {
class History;
}

class History : public QWidget
{
    Q_OBJECT
signals:
    void closed();
protected:
    void closeEvent(QCloseEvent *event) override;

public:
    explicit History(QWidget *parent = nullptr);
    ~History();

private:
    QSqlDatabase db;
    QSqlQuery *history;
    QSqlTableModel *model;
    Ui::History *ui;
};

#endif // HISTORY_H
