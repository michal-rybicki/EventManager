#ifndef WYSTAWAZBAZY_H
#define WYSTAWAZBAZY_H

#include <QDialog>
#include <QtSql>
#include "wystawy.h"

namespace Ui {
class wystawazbazy;
}

class wystawazbazy : public QDialog
{
    Q_OBJECT

public:
    QStringList nazwy;
    explicit wystawazbazy(QWidget *parent = nullptr);
    ~wystawazbazy();

signals:
    void wybranoWystawe(int ID);

private slots:
    void on_wystawazbazy_accepted();

private:
    Ui::wystawazbazy *ui;
};

#endif // WYSTAWAZBAZY_H
