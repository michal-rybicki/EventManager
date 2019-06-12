#ifndef NOWYMAGAZYN_H
#define NOWYMAGAZYN_H

#include <QDialog>
#include <wystawy.h>
#include <tools.h>
#include <QDoubleValidator>

namespace Ui {
class nowymagazyn;
}

class nowymagazyn : public QDialog
{
    Q_OBJECT

public:
    explicit nowymagazyn(QWidget *parent = 0);
    ~nowymagazyn();

signals:
    void StworzonyMagazyn(magazyn*);

private slots:
    void on_nowymagazyn_accepted();

private:
    Ui::nowymagazyn *ui;
};

#endif // NOWYMAGAZYN_H
