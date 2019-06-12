#ifndef EDYCJAMAGAZYNU_H
#define EDYCJAMAGAZYNU_H

#include <QDialog>
#include <QMessageBox>
#include "wystawy.h"
#include <tools.h>

namespace Ui {
class edycjamagazynu;
}

class edycjamagazynu : public QDialog
{
    Q_OBJECT

public:
    explicit edycjamagazynu(QWidget *parent = 0);
    ~edycjamagazynu();
    wystawa * starforce;//lokalny wskaznik na wystawe
    bool TrybUsuwania;//blokuje dzialanie comboboxa na czas usuwania magazynu z wystawy
    int PoprzedniMagazyn;//do pamietania ktory magazyn zapisac kiedy w comboboxie wybieramy inny do wczytania
    void ZmienMagazyn(int NrMagazynu);//edytuje magazyn w obiekcie starforce klasy wystawa

    //Do testow:
    void InformacjaTestowa(QString informacja);//do testow
    bool TrybTestowania;//do testow

signals:
    void EdycjaMagazynowZakonczona();
    void UsunWiersz();

public slots:
    void Ustaw(wystawa* starforce0);//przekazuje wskaznik do wystawy okna glownego i ustawia combobox wyboru magazynu
    void UstawWybor();//ustawianie comboboxa wyboru magazynow
    void UstawMagazyn();//wczytuje magazyn wybrany w comboboxie do pol formularza

private slots:
    void on_wybor_currentIndexChanged(int index);//wczytuje magazyn wskazany przez combobox do pol formularza
    void on_edycjamagazynu_accepted();
    void on_edycjamagazynu_rejected();

    void on_Usun_clicked();

private:
    Ui::edycjamagazynu *ui;
};

#endif // EDYCJAMAGAZYNU_H
