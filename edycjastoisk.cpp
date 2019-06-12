#include "edycjastoisk.h"
#include "ui_edycjastoisk.h"


edycjastoisk::edycjastoisk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edycjastoisk)
{
    TrybTestowania=true;
    ui->setupUi(this);
    this->setWindowTitle("Edytuj stoiska...");
    this->setFixedSize(503,319);//ustawianie stalego rozmiaru okna
    PoprzednieStoisko=0;
    TrybUsuwania=false;

    ui->jednstolow->addItem("szt.");
    ui->jednstolow->addItem("m2");
    ui->jednstolow->addItem("mb");
    ui->jednstolow->addItem("ławek");
    ui->jedngablot->addItem("szt.");
    ui->jedngablot->addItem("m2");
    ui->jednbarierslupkowych->addItem("szt.");
    ui->jednbarierslupkowych->addItem("mb");
    ui->jednbarierstalowych->addItem("szt.");
    ui->jednbarierstalowych->addItem("mb");
    ui->prad->addItem("Nie");
    ui->prad->addItem("Tak");


    QDoubleValidator * Straznik = new QDoubleValidator(0, 1000, 2, this);
    ui->Stoly->setValidator(Straznik);
    ui->Gabloty->setValidator(Straznik);
    ui->BarierkiSlupkowe->setValidator(Straznik);
    ui->BarierkiStalowe->setValidator(Straznik);
    ui->Krzesla->setValidator(Straznik);

}

edycjastoisk::~edycjastoisk()
{
    delete ui;
}

//----ustawianie comboboxow magazynow i stoisk oraz przechwytywanie ustawien wystawy----------------------------------------------------

void edycjastoisk::Ustaw(wystawa* starforce0)
{
    starforce=starforce0;
    UstawMagazyny();
    UstawWybor();

};

void edycjastoisk::UstawWybor()//ustawianie comboboxa stoisk
{
    ui->wybor->clear();
    for (int i=0; i<starforce->stoiska.count(); i++)
    {
        ui->wybor->addItem(starforce->stoiska[i].nazwa);
    };
}


//-----dodaje pole "brak" w comboboxach------------------------------------------------------------------------------------------

void edycjastoisk::DodajBraki()
{
    ui->magazynkrzesla->addItem("brak");
    ui->magazynbarierkistalowe->addItem("brak");
    ui->magazynbarierkislupkowe->addItem("brak");
    ui->magazyngabloty->addItem("brak");
    ui->magazynstoly->addItem("brak");
    ui->Stoly->setEnabled(true);
    ui->jednstolow->setEnabled(true);
    ui->magazynstoly->setEnabled(true);
    ui->dodatekpodgabloty->setEnabled(true);
    ui->Gabloty->setEnabled(true);
    ui->jedngablot->setEnabled(true);
    ui->magazyngabloty->setEnabled(true);
    ui->BarierkiSlupkowe->setEnabled(true);
    ui->jednbarierslupkowych->setEnabled(true);
    ui->magazynbarierkislupkowe->setEnabled(true);
    ui->BarierkiStalowe->setEnabled(true);
    ui->jednbarierstalowych->setEnabled(true);
    ui->magazynbarierkistalowe->setEnabled(true);
    ui->Krzesla->setEnabled(true);
    ui->magazynkrzesla->setEnabled(true);
};


void edycjastoisk::UstawMagazyny()//ustawianie comboboxow magazynow
{
    //DodajBraki();
    int LicznikMagazynow[5];
    int s;
    for (int i=0;i<5;i++)
    {
        LicznikMagazynow[i]=0;
        for (int j=0; j<200; j++)
        {
            DostepneMagazynyNr[i][j]=0;
            DostepneMagazynyLicznik[i][j]=0;
        };
    };

    for (int i=0;i<starforce->zaplecze.count();i++)
    {
        s=0;
        if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            ui->magazynstoly->addItem(starforce->zaplecze[i].nazwa);
            DostepneMagazynyNr[s][LicznikMagazynow[s]]=i;
            DostepneMagazynyLicznik[s][i]=LicznikMagazynow[s];
            LicznikMagazynow[s]++;
            ui->Stoly->setEnabled(true);
            ui->jednstolow->setEnabled(true);
            ui->magazynstoly->setEnabled(true);
            ui->dodatekpodgabloty->setEnabled(true);
        };
        s++;
        if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            ui->magazyngabloty->addItem(starforce->zaplecze[i].nazwa);
            DostepneMagazynyNr[s][LicznikMagazynow[s]]=i;
            DostepneMagazynyLicznik[s][i]=LicznikMagazynow[s];;
            LicznikMagazynow[s]++;
            ui->Gabloty->setEnabled(true);
            ui->jedngablot->setEnabled(true);
            ui->magazyngabloty->setEnabled(true);
        };
        s++;
        if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            ui->magazynbarierkislupkowe->addItem(starforce->zaplecze[i].nazwa);
            DostepneMagazynyNr[s][LicznikMagazynow[s]]=i;
            DostepneMagazynyLicznik[s][i]=LicznikMagazynow[s];
            LicznikMagazynow[s]++;
            ui->BarierkiSlupkowe->setEnabled(true);
            ui->jednbarierslupkowych->setEnabled(true);
            ui->magazynbarierkislupkowe->setEnabled(true);
        };
        s++;
        if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            ui->magazynbarierkistalowe->addItem(starforce->zaplecze[i].nazwa);
            DostepneMagazynyNr[s][LicznikMagazynow[s]]=i;
            DostepneMagazynyLicznik[s][i]=LicznikMagazynow[s];
            LicznikMagazynow[s]++;
            ui->BarierkiStalowe->setEnabled(true);
            ui->jednbarierstalowych->setEnabled(true);
            ui->magazynbarierkistalowe->setEnabled(true);
        };
        s++;
        if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            ui->magazynkrzesla->addItem(starforce->zaplecze[i].nazwa);
            DostepneMagazynyNr[s][LicznikMagazynow[s]]=i;
            DostepneMagazynyLicznik[s][i]=LicznikMagazynow[s];
            LicznikMagazynow[s]++;
            ui->Krzesla->setEnabled(true);
            ui->magazynkrzesla->setEnabled(true);
        };
    };
};

//----wczytywanie stoiska wybranego przez combobox do pol formularza--------------------------------------------

void edycjastoisk::UstawStoisko()
{

    if (ui->wybor->currentIndex()<0 || ui->wybor->currentIndex()>(starforce->stoiska.count())) return;
    int i=ui->wybor->currentIndex();
    PoprzednieStoisko=ui->wybor->currentIndex();

    ui->nazwa->clear();
    ui->Stoly->clear();
    ui->Gabloty->clear();
    ui->BarierkiSlupkowe->clear();
    ui->BarierkiStalowe->clear();
    ui->Krzesla->clear();

    ui->nazwa->setText(starforce->stoiska[i].nazwa);
    ui->Stoly->setText(QString::number(starforce->stoiska[i].stoly.iloscWJU));
    ui->jednstolow->setCurrentIndex(starforce->stoiska[i].stoly.typJU);
    if (ui->magazynstoly->count()>0 && (starforce->stoiska[i].stoly.nrMagazynu)!=100000) ui->magazynstoly->setCurrentIndex(DostepneMagazynyLicznik[0][starforce->stoiska[i].stoly.nrMagazynu]);
    ui->Gabloty->setText(QString::number(starforce->stoiska[i].gabloty.iloscWJU));
    ui->jedngablot->setCurrentIndex(starforce->stoiska[i].gabloty.typJU);
    if (ui->magazyngabloty->count()>0 && starforce->stoiska[i].gabloty.nrMagazynu!=100000) ui->magazyngabloty->setCurrentIndex(DostepneMagazynyLicznik[1][starforce->stoiska[i].gabloty.nrMagazynu]);
    ui->BarierkiSlupkowe->setText(QString::number(starforce->stoiska[i].barierkiSlupkowe.iloscWJU));
    ui->jednbarierslupkowych->setCurrentIndex(starforce->stoiska[i].barierkiSlupkowe.typJU);
    if (ui->magazynbarierkislupkowe->count()>0 && starforce->stoiska[i].barierkiSlupkowe.nrMagazynu!=100000) ui->magazynbarierkislupkowe->setCurrentIndex(DostepneMagazynyLicznik[2][starforce->stoiska[i].barierkiSlupkowe.nrMagazynu]);
    ui->BarierkiStalowe->setText(QString::number(starforce->stoiska[i].barierkiStalowe.iloscWJU));
    ui->jednbarierstalowych->setCurrentIndex(starforce->stoiska[i].barierkiStalowe.typJU);
    if (ui->magazynbarierkistalowe->count()>0 && starforce->stoiska[i].barierkiStalowe.nrMagazynu!=100000) ui->magazynbarierkistalowe->setCurrentIndex(DostepneMagazynyLicznik[3][starforce->stoiska[i].barierkiStalowe.nrMagazynu]);
    ui->Krzesla->setText(QString::number(starforce->stoiska[i].krzesla.iloscWJU));
    if (ui->magazynkrzesla->count()>0 && starforce->stoiska[i].krzesla.nrMagazynu!=100000) ui->magazynkrzesla->setCurrentIndex(DostepneMagazynyLicznik[4][starforce->stoiska[i].krzesla.nrMagazynu]);
    ui->prad->setCurrentIndex(starforce->stoiska[i].zapotrzebowaniePrad);
    if (starforce->stoiska[i].doliczStolyPodGabloty==1)
        ui->dodatekpodgabloty->setChecked(true);
    else
        ui->dodatekpodgabloty->setChecked(false);
};

//----zmiana aktywnej pozycji w comboboxie stoisk---------------------------------------------------------------------

void edycjastoisk::on_wybor_currentIndexChanged(int index)
{
    if (!TrybUsuwania)
    {
        if (PoprzednieStoisko!=ui->wybor->currentIndex()) ZmienStoisko(PoprzednieStoisko);
        UstawStoisko();
    }
}

//----edysja stoiska w wystawie-----------------------------------------------------------------------------------------

void edycjastoisk::ZmienStoisko(int nrStoiska)
{
    if (nrStoiska<0 || nrStoiska>=starforce->stoiska.count()) return;

    starforce->stoiska[nrStoiska].nazwa=ui->nazwa->text();
    starforce->stoiska[nrStoiska].stoly.iloscWJU=tools::toDouble(ui->Stoly->text());
    starforce->stoiska[nrStoiska].stoly.typJU=ui->jednstolow->currentIndex();
    if (ui->magazynstoly->count()>1)
    {
        if (starforce->stoiska[nrStoiska].stoly.nrMagazynu!=100000)
            starforce->stoiska[nrStoiska].stoly.nrStaregoMagazynu=starforce->stoiska[nrStoiska].stoly.nrMagazynu;
        else
            starforce->stoiska[nrStoiska].stoly.nrStaregoMagazynu=DostepneMagazynyNr[0][ui->magazynstoly->currentIndex()];
        starforce->stoiska[nrStoiska].stoly.nrMagazynu=DostepneMagazynyNr[0][ui->magazynstoly->currentIndex()];
    };
    starforce->stoiska[nrStoiska].gabloty.iloscWJU=tools::toDouble(ui->Gabloty->text());
    starforce->stoiska[nrStoiska].gabloty.typJU=ui->jedngablot->currentIndex();
    if (ui->magazyngabloty->count()>1)
    {
        if (starforce->stoiska[nrStoiska].gabloty.nrMagazynu!=100000)
            starforce->stoiska[nrStoiska].gabloty.nrStaregoMagazynu=starforce->stoiska[nrStoiska].gabloty.nrMagazynu;
        else
            starforce->stoiska[nrStoiska].gabloty.nrStaregoMagazynu=DostepneMagazynyNr[1][ui->magazyngabloty->currentIndex()];
        starforce->stoiska[nrStoiska].gabloty.nrMagazynu=DostepneMagazynyNr[1][ui->magazyngabloty->currentIndex()];
    };
    starforce->stoiska[nrStoiska].barierkiSlupkowe.iloscWJU=tools::toDouble(ui->BarierkiSlupkowe->text());
    starforce->stoiska[nrStoiska].barierkiSlupkowe.typJU=ui->jednbarierslupkowych->currentIndex();
    if (ui->magazynbarierkislupkowe->count()>1)
    {
        if (starforce->stoiska[nrStoiska].barierkiSlupkowe.nrMagazynu!=100000)
            starforce->stoiska[nrStoiska].barierkiSlupkowe.nrStaregoMagazynu=starforce->stoiska[nrStoiska].barierkiSlupkowe.nrMagazynu;
        else
            starforce->stoiska[nrStoiska].barierkiSlupkowe.nrStaregoMagazynu=DostepneMagazynyNr[2][ui->magazynbarierkislupkowe->currentIndex()];
        starforce->stoiska[nrStoiska].barierkiSlupkowe.nrMagazynu=DostepneMagazynyNr[2][ui->magazynbarierkislupkowe->currentIndex()];
    };
    starforce->stoiska[nrStoiska].barierkiStalowe.iloscWJU=tools::toDouble(ui->BarierkiStalowe->text());
    starforce->stoiska[nrStoiska].barierkiStalowe.typJU=ui->jednbarierstalowych->currentIndex();
    if (ui->magazynbarierkistalowe->count()>1)
    {
        if (starforce->stoiska[nrStoiska].barierkiStalowe.nrMagazynu!=100000)
            starforce->stoiska[nrStoiska].barierkiStalowe.nrStaregoMagazynu=starforce->stoiska[nrStoiska].barierkiStalowe.nrMagazynu;
        else
            starforce->stoiska[nrStoiska].barierkiStalowe.nrStaregoMagazynu=DostepneMagazynyNr[3][ui->magazynbarierkistalowe->currentIndex()];
        starforce->stoiska[nrStoiska].barierkiStalowe.nrMagazynu=DostepneMagazynyNr[3][ui->magazynbarierkistalowe->currentIndex()];
    };
    starforce->stoiska[nrStoiska].krzesla.iloscWJU=ui->Krzesla->text().toInt();
    if (ui->magazynkrzesla->count()>1)
    {
        if (starforce->stoiska[nrStoiska].krzesla.nrMagazynu!=100000)
            starforce->stoiska[nrStoiska].krzesla.nrStaregoMagazynu=starforce->stoiska[nrStoiska].krzesla.nrMagazynu;
        else
            starforce->stoiska[nrStoiska].krzesla.nrStaregoMagazynu=DostepneMagazynyNr[4][ui->magazynkrzesla->currentIndex()];
        starforce->stoiska[nrStoiska].krzesla.nrMagazynu=DostepneMagazynyNr[4][ui->magazynkrzesla->currentIndex()];
    };
    starforce->stoiska[nrStoiska].zapotrzebowaniePrad=ui->prad->currentIndex();
    if (ui->dodatekpodgabloty->isChecked())
        starforce->stoiska[nrStoiska].doliczStolyPodGabloty=1;
    else
        starforce->stoiska[nrStoiska].doliczStolyPodGabloty=0;
    starforce->przelicz_stoisko(starforce->stoiska[nrStoiska]);
};

//----reakcja na ok i anuluj--------------------------------------------------------------------------------------------

void edycjastoisk::on_edycjastoisk_accepted()
{
    ZmienStoisko(ui->wybor->currentIndex());
    emit EdycjaStoiskZakonczona();
}

void edycjastoisk::on_edycjastoisk_rejected()
{
    emit EdycjaStoiskZakonczona();
}

//----usuwanie stoiska---------------------------------------------------------------------------------------------

void edycjastoisk::on_usun_clicked()
{
    TrybUsuwania=true;//wylacza rekacje na zmany w comboboxie wyboru stoisk zeby umozliwic jego edycje
    starforce->usun_stoisko(ui->wybor->currentIndex());//usuwa stoisko z wystawy
    if (starforce->stoiska.count()>0)
    {
        PoprzednieStoisko=0;//blokuje edycje stoiska o numerze usuwanego stoiska po przejsciu do stoiska nr 0
        UstawWybor();//ustawia combobox wyboru stoisk
        emit UsunWiersz();//usuwa wiersz w tabeli stoisk
        TrybUsuwania=false;//przywraca rekacje na zmany w comboboxie wyboru stoisk
        UstawStoisko();
    }
    else
    {
        emit UsunWiersz();//usuwa wiersz w tabeli stoisk
        this->close();
    }
}

//-----TESTY----------------------------------------------------------------------------------------------------

void edycjastoisk::InformacjaTestowa(QString informacja)
{
    QMessageBox msgBox;
    msgBox.setText(informacja);
    msgBox.exec();
};
