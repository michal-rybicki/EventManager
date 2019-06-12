#ifndef HISTORIA_H
#define HISTORIA_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include "wystawy.h"
#include <QListWidgetItem>
#include <QListWidget>
#include <QTableWidgetItem>

namespace Ui {
class historia;
}

class historia : public QDialog
{
    Q_OBJECT


public:
    wystawa * starforce;
    int index[100];
    QString nazwa[100];
    int ID[100];
    int ID_wystawy;
    QString data;
    wystawa * aktualny_starforce;
    int TrybWyswietlania;//0 - ilosci w tabeli stoisk wyswietlane w jednostkach podanych przez uzytkowanika
                         //1 - ilosci w tabeli stoisk wyswietlane w szt.
    int TrybWyswietlaniaMagazynow; //0 - ilosc pobrana z magazynu
                                   //1 - ilosc pozostala w magazynie
                                   //2 - ilosc w sumie dostepna w magazynie
    explicit historia(QWidget *parent = nullptr);
    void WczytajZBazy(int ID_wystawy, QString data);
    void DopiszStoisko(stoisko * N);//dopisuje stoisko do tabeli
    void DopiszMagazyn(magazyn * N);//dopisuje magazyn do tabeli
    void uaktualnijComboboxEdycji(); //uaktualnia combox edycji
    void ustawWystawy(); //ustawia combobox wystaw
    ~historia();

signals:
    void PobieranieZakonczone();
    void StworzoneStoisko(stoisko*);
    void StworzonyMagazyn(magazyn*);
    void UsunWiersz();
    void UsunWierszMagazynow();

public slots:
    void Ustaw(wystawa * starforce0);
    void DodajStoisko(stoisko * N);//dodaje stoisko do wystawy i dopisuje do tabeli
    void DodajMagazyn(magazyn * N);//dodaje magazyn do wystawy i dopisuje do tabeli
    void UaktualnijTabeleMagazynow();//uaktualnia tabele magazynow
    void InformacjaTestowa(QString informacja);
private slots:
    void on_edycja_currentIndexChanged(int index);

    void on_wystawy_currentIndexChanged(int index);

    void on_historia_accepted();

private:
    Ui::historia *ui;
};

#endif // HISTORIA_H
