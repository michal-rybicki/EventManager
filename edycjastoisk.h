#ifndef EDYCJASTOISK_H
#define EDYCJASTOISK_H

#include <QDialog>
#include <QMessageBox>
#include "wystawy.h"
#include "tools.h"

namespace Ui {
class edycjastoisk;
}

class edycjastoisk : public QDialog
{
    Q_OBJECT

public:
    wystawa * starforce;
    bool TrybUsuwania;//kiedy prawdziwy blokuje rekacje na zmiany zawartosci
                      //comboboxa wyboru stoiska w celu jego bezpiecznej edycji
    int PoprzednieStoisko;//do pamietania ktore stoisko zapisac kiedy w comboboxie wybieramy inne do wczytania
    int DostepneMagazynyNr[5][200];//do synchronizacji indexow w comboboxach z nr magazynow
    int DostepneMagazynyLicznik[5][200];//do synchronizacji nr magazynow z indexami z comboboxach
    explicit edycjastoisk(QWidget *parent = 0);
    void ZmienStoisko(int nrStoiska);//edytuje stoisko w obiekcie starforce klasy wystawa
    void UstawMagazyny();//ustawia comboboxy wyborow magazynow i synchronizuje je z nr magazynow
    void UstawWybor();//ustawia pozycje w comboboxie wyboru stoiska
    void InformacjaTestowa(QString informacja);//do testowania
    bool TrybTestowania;
    void DodajBraki();
    ~edycjastoisk();

signals:
    void EdycjaStoiskZakonczona();
    void UsunWiersz();

public slots:
    void Ustaw(wystawa * starforce0);
    void UstawStoisko();//wczytuje stoisko wskazane przez combobox do pol formularza

private slots:
    void on_wybor_currentIndexChanged(int index);//wczytuje stoisko wskazane przez combobox do pol formularza
    void on_edycjastoisk_accepted();
    void on_edycjastoisk_rejected();
    void on_usun_clicked();

private:
    Ui::edycjastoisk *ui;
};

#endif // EDYCJASTOISK_H
