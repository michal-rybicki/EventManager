#include "wystawazbazy.h"
#include "ui_wystawazbazy.h"

wystawazbazy::wystawazbazy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wystawazbazy)
{
    ui->setupUi(this);
    this->setWindowTitle("Wybierz wystawe...");
    QSqlQuery query;
    QSqlQuery query2;

    QString nazwa;
    //QList<QString> nazwy;
    query.exec("select nazwa, problemy from wystawy where ID>-999");
    while (query.next())
    {
        nazwy.append(query.value(0).toString());
        if (query.value(1).toInt()>0)
        {
            ui->wystawy->addItem(query.value(0).toString()+" (Problemy: "+query.value(1).toString()+")");
        }
        else
        {
            ui->wystawy->addItem(query.value(0).toString());
        }
    }
}

wystawazbazy::~wystawazbazy()
{
    delete ui;
}

void wystawazbazy::on_wystawazbazy_accepted()
{
    QSqlQuery query;
    QString nazwa=nazwy[ui->wystawy->currentIndex()];
    query.prepare("select ID from wystawy where nazwa like :nazwa and ID>-999");
    query.bindValue(":nazwa",nazwa);
    query.exec();
    query.first();
    int ID_wystawy=query.value(0).toInt();
    emit wybranoWystawe(ID_wystawy);
}
