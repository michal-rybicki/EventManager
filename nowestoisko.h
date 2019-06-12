#ifndef NOWESTOISKO_H
#define NOWESTOISKO_H

#include <QDialog>
#include "wystawy.h"
#include "tools.h"

namespace Ui {
class Nowestoisko;
}

class Nowestoisko : public QDialog
{
    Q_OBJECT

public:
    explicit Nowestoisko(QWidget *parent = 0);
    ~Nowestoisko();
    int DostepneMagazyny[5][200];//to synchronizacji comboboxow z nr magazynow
    void DodajBraki();
signals:
    void StworzoneStoisko(stoisko*);

public slots:
    void UstawMagazyny(wystawa* starforce);//ustawia comboboxy wyborow magazynow i synchronizuje je z nr magazynow

private slots:
    void on_Nowestoisko_accepted();

private:
    Ui::Nowestoisko *ui;
};

#endif // NOWESTOISKO_H
