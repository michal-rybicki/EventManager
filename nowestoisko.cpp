#include "nowestoisko.h"
#include "ui_nowestoisko.h"

Nowestoisko::Nowestoisko(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Nowestoisko)
{
    ui->setupUi(this);
    this->setWindowTitle("Nowe stoisko...");
    this->setFixedSize(508,281);//ustawianie stalego rozmiaru okna

    //Ustawienia jednostek do wyboru:

    //   0      1         2           3            4
    //   stol   gablota   bar. slu.   bar. stal.   krzesla
    //0  szt.   szt.      szt.        szt.         szt.
    //1  m2     m2        mb          mb
    //2  mb
    //3  lawka szkolna

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

    //Zabezpieczenia wprowadzania wartosci:
    QDoubleValidator * Straznik = new QDoubleValidator(0, 1000, 2, this);
    ui->Stoly->setValidator(Straznik);
    ui->Gabloty->setValidator(Straznik);
    ui->BarierkiSlupkowe->setValidator(Straznik);
    ui->BarierkiStalowe->setValidator(Straznik);
    ui->Krzesla->setValidator(Straznik);
};


//-----ustawia dostepne magazyny w comoboboxach----------------------------------------------------------------------

void Nowestoisko::UstawMagazyny(wystawa* starforce)
{
    //DodajBraki();
    int LicznikMagazynow[5];
    int s;
    for (int i=0;i<5;i++)
    {
        LicznikMagazynow[i]=0;
        for (int j=0; j<200; j++)
        {
            DostepneMagazyny[i][j]=0;
        };
    };
    for (int i=0;i<starforce->zaplecze.count();i++)
    {
        s=0;
        if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            ui->magazynstoly->addItem(starforce->zaplecze[i].nazwa);
            DostepneMagazyny[s][LicznikMagazynow[s]]=i;
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
            DostepneMagazyny[s][LicznikMagazynow[s]]=i;
            LicznikMagazynow[s]++;
            ui->Gabloty->setEnabled(true);
            ui->jedngablot->setEnabled(true);
            ui->magazyngabloty->setEnabled(true);
        };
        s++;
        if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            ui->magazynbarierkislupkowe->addItem(starforce->zaplecze[i].nazwa);
            DostepneMagazyny[s][LicznikMagazynow[s]]=i;
            LicznikMagazynow[s]++;
            ui->BarierkiSlupkowe->setEnabled(true);
            ui->jednbarierslupkowych->setEnabled(true);
            ui->magazynbarierkislupkowe->setEnabled(true);
        };
        s++;
        if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            ui->magazynbarierkistalowe->addItem(starforce->zaplecze[i].nazwa);
            DostepneMagazyny[s][LicznikMagazynow[s]]=i;
            LicznikMagazynow[s]++;
            ui->BarierkiStalowe->setEnabled(true);
            ui->jednbarierstalowych->setEnabled(true);
            ui->magazynbarierkistalowe->setEnabled(true);
        };
        s++;
        if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            ui->magazynkrzesla->addItem(starforce->zaplecze[i].nazwa);
            DostepneMagazyny[s][LicznikMagazynow[s]]=i;
            LicznikMagazynow[s]++;
            ui->Krzesla->setEnabled(true);
            ui->magazynkrzesla->setEnabled(true);
        };
    };
};

Nowestoisko::~Nowestoisko()
{
    delete ui;
}

//-----dodaje pole "brak" w comboboxach------------------------------------------------------------------------------------------

void Nowestoisko::DodajBraki()
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

//-----tworzy nowe stoisko i emituje je do okna glownego----------------------------------------------------------------------

void Nowestoisko::on_Nowestoisko_accepted()
{
    stoisko * N;
    N = new stoisko;
    N->nazwa=ui->nazwa->text();
    N->stoly.iloscWJU=tools::toDouble(ui->Stoly->text());
    N->stoly.typJU=ui->jednstolow->currentIndex();
    if (ui->magazynstoly->count()>0) N->stoly.nrMagazynu=DostepneMagazyny[0][ui->magazynstoly->currentIndex()];
    if (ui->magazynstoly->count()>0) N->stoly.nrStaregoMagazynu=DostepneMagazyny[0][ui->magazynstoly->currentIndex()];
    N->gabloty.iloscWJU=tools::toDouble(ui->Gabloty->text());
    N->gabloty.typJU=ui->jedngablot->currentIndex();
    if (ui->magazyngabloty->count()>0) N->gabloty.nrMagazynu=DostepneMagazyny[1][ui->magazyngabloty->currentIndex()];
    if (ui->magazyngabloty->count()>0) N->gabloty.nrStaregoMagazynu=DostepneMagazyny[1][ui->magazyngabloty->currentIndex()];
    N->barierkiSlupkowe.iloscWJU=tools::toDouble(ui->BarierkiSlupkowe->text());
    N->barierkiSlupkowe.typJU=ui->jednbarierslupkowych->currentIndex();
    if (ui->magazynbarierkislupkowe->count()>0) N->barierkiSlupkowe.nrMagazynu=DostepneMagazyny[2][ui->magazynbarierkislupkowe->currentIndex()];
    if (ui->magazynbarierkislupkowe->count()>0) N->barierkiSlupkowe.nrStaregoMagazynu=DostepneMagazyny[2][ui->magazynbarierkislupkowe->currentIndex()];
    N->barierkiStalowe.iloscWJU=tools::toDouble(ui->BarierkiStalowe->text());
    N->barierkiStalowe.typJU=ui->jednbarierstalowych->currentIndex();
    if (ui->magazynbarierkistalowe->count()>0) N->barierkiStalowe.nrMagazynu=DostepneMagazyny[3][ui->magazynbarierkistalowe->currentIndex()];
    if (ui->magazynbarierkistalowe->count()>0) N->barierkiStalowe.nrStaregoMagazynu=DostepneMagazyny[3][ui->magazynbarierkistalowe->currentIndex()];
    N->krzesla.iloscWJU=ui->Krzesla->text().toInt();
    if (ui->magazynkrzesla->count()>0) N->krzesla.nrMagazynu=DostepneMagazyny[4][ui->magazynkrzesla->currentIndex()];
    if (ui->magazynkrzesla->count()>0) N->krzesla.nrStaregoMagazynu=DostepneMagazyny[4][ui->magazynkrzesla->currentIndex()];
    N->zapotrzebowaniePrad=ui->prad->currentIndex();
    if (ui->dodatekpodgabloty->isChecked())
        N->doliczStolyPodGabloty=1;
    else
        N->doliczStolyPodGabloty=0;
    emit StworzoneStoisko(N);
}
