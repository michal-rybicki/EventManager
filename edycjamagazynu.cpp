#include "edycjamagazynu.h"
#include "ui_edycjamagazynu.h"


edycjamagazynu::edycjamagazynu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edycjamagazynu)
{
    //Ustawienia testow:
    TrybTestowania=true;

    //Ustawienia ogolne:
    ui->setupUi(this);
    PoprzedniMagazyn=1;
    TrybUsuwania=false;
    this->setWindowTitle("Edytuj magazyny...");
    this->setFixedSize(470,262);//ustawianie stalego rozmiaru okna

    //Zabezpieczenia wprowadzania wartosci:
    QDoubleValidator * Straznik = new QDoubleValidator(0, 1000, 2, this);
    ui->Stoly->setValidator(Straznik);
    ui->Gabloty->setValidator(Straznik);
    ui->BarierkiSlupkowe->setValidator(Straznik);
    ui->BarierkiStalowe->setValidator(Straznik);
    ui->Krzesla->setValidator(Straznik);

    QDoubleValidator * StraznikDodatni = new QDoubleValidator(0.1, 1000, 2, this);
    ui->DlStolu->setValidator(StraznikDodatni);
    ui->SzStolu->setValidator(StraznikDodatni);
    ui->DlGabloty->setValidator(StraznikDodatni);
    ui->SzGabloty->setValidator(StraznikDodatni);
    ui->DlBarierySlupkowej->setValidator(StraznikDodatni);
    ui->DlBarieryStalowej->setValidator(StraznikDodatni);
}

edycjamagazynu::~edycjamagazynu()
{
    delete ui;
}

//-----ustawianie wskaznika na wystawe i comboboxa wyboru magazynow------------------------------------------------------------------------------------

void edycjamagazynu::Ustaw(wystawa* starforce0)
{
    starforce=starforce0;
    UstawWybor();

};

//-----ustawianie comboboxa wyboru magazynow------------------------------------------------------------------------------------

void edycjamagazynu::UstawWybor()
{
    ui->wybor->clear();
    for (int i=1; i<starforce->zaplecze.count(); i++)
    {
        ui->wybor->addItem(starforce->zaplecze[i].nazwa);
    };
};

//-----wczytanie do formularza magazynu wybranego przez combobox----------------------------------------------------------------

void edycjamagazynu::UstawMagazyn()
{
    int i=ui->wybor->currentIndex()+1;
    PoprzedniMagazyn=ui->wybor->currentIndex()+1;

    //czyszczenie formularza

    ui->Usun->setEnabled(false);

    //wypelnianie formularza danymi magazynu
    ui->nazwa->setText(starforce->zaplecze[i].nazwa);
    ui->Stoly->setText(QString::number(starforce->zaplecze[i].sprzetNaStanie[0]->ilosc));
    ui->DlStolu->setText(QString::number(starforce->zaplecze[i].sprzetNaStanie[0]->dlugosc));
    ui->SzStolu->setText(QString::number(starforce->zaplecze[i].sprzetNaStanie[0]->szerokosc));
    ui->Gabloty->setText(QString::number(starforce->zaplecze[i].sprzetNaStanie[1]->ilosc));
    ui->DlGabloty->setText(QString::number(starforce->zaplecze[i].sprzetNaStanie[1]->dlugosc));
    ui->SzGabloty->setText(QString::number(starforce->zaplecze[i].sprzetNaStanie[1]->szerokosc));
    ui->BarierkiSlupkowe->setText(QString::number(starforce->zaplecze[i].sprzetNaStanie[2]->ilosc));
    ui->DlBarierySlupkowej->setText(QString::number(starforce->zaplecze[i].sprzetNaStanie[2]->dlugosc));
    ui->BarierkiStalowe->setText(QString::number(starforce->zaplecze[i].sprzetNaStanie[3]->ilosc));
    ui->DlBarieryStalowej->setText(QString::number(starforce->zaplecze[i].sprzetNaStanie[3]->dlugosc));
    ui->Krzesla->setText(QString::number(starforce->zaplecze[i].sprzetNaStanie[4]->ilosc));
    if (       starforce->zaplecze[i].stolyPobrane.iloscWSzt==0
            && starforce->zaplecze[i].gablotyPobrane.iloscWSzt==0
            && starforce->zaplecze[i].barierkiSlupkowePobrane.iloscWSzt==0
            && starforce->zaplecze[i].barierkiStalowePobrane.iloscWSzt==0
            && starforce->zaplecze[i].krzeslaPobrane.iloscWSzt==0)
        ui->Usun->setEnabled(true);
};

//----edycja magazynu w wystawie--------------------------------------------------------------------------------------------

void edycjamagazynu::ZmienMagazyn(int NrMagazynu)
{

    double wymiar1=0, wymiar2=0;
    starforce->zaplecze[NrMagazynu].nazwa=ui->nazwa->text();
    starforce->zaplecze[NrMagazynu].sprzetNaStanie[0]->ilosc=ui->Stoly->text().toInt();
    wymiar1=tools::toDouble(ui->DlStolu->text());
    wymiar2=tools::toDouble(ui->SzStolu->text());
    starforce->zaplecze[NrMagazynu].sprzetNaStanie[0]->ustawWymiary(wymiar1,wymiar2);
    starforce->zaplecze[NrMagazynu].sprzetNaStanie[1]->ilosc=ui->Gabloty->text().toInt();
    wymiar1=tools::toDouble(ui->DlGabloty->text());
    wymiar2=tools::toDouble(ui->SzGabloty->text());
    starforce->zaplecze[NrMagazynu].sprzetNaStanie[1]->ustawWymiary(wymiar1,wymiar2);
    starforce->zaplecze[NrMagazynu].sprzetNaStanie[2]->ilosc=ui->BarierkiSlupkowe->text().toInt();
    starforce->zaplecze[NrMagazynu].sprzetNaStanie[2]->dlugosc=tools::toDouble(ui->DlBarierySlupkowej->text());
    starforce->zaplecze[NrMagazynu].sprzetNaStanie[3]->ilosc=ui->BarierkiStalowe->text().toInt();
    starforce->zaplecze[NrMagazynu].sprzetNaStanie[3]->dlugosc=tools::toDouble(ui->DlBarieryStalowej->text());
    starforce->zaplecze[NrMagazynu].sprzetNaStanie[4]->ilosc=ui->Krzesla->text().toInt();
};

//-----reakcja na zmiane wyboru w comboboxie--------------------------------------------------------------------

void edycjamagazynu::on_wybor_currentIndexChanged(int index)
{
   if (!TrybUsuwania)
   {
       if (PoprzedniMagazyn!=ui->wybor->currentIndex()+1) ZmienMagazyn(PoprzedniMagazyn);
       if (ui->wybor->currentIndex()!=(-1)) UstawMagazyn();
   };
}

//----reakcja na ok i anuluj---------------------------------------------------------------------------------------

void edycjamagazynu::on_edycjamagazynu_accepted()
{
    ZmienMagazyn(ui->wybor->currentIndex()+1);
    emit EdycjaMagazynowZakonczona();
};

void edycjamagazynu::on_edycjamagazynu_rejected()
{
    emit EdycjaMagazynowZakonczona();
}

//---usuwanie magazynu-----------------------------------------------------------------------------------------------

void edycjamagazynu::on_Usun_clicked()
{
    TrybUsuwania=true;//wylacza rekacje na zmany w comboboxie wyboru magazynow zeby umozliwic jego bezpieczna edycje
    starforce->usun_magazyn(ui->wybor->currentIndex()+1);
    if (starforce->zaplecze.count()>1)
    {
        UstawWybor();
        emit UsunWiersz();
        TrybUsuwania=false;
        UstawMagazyn();
    }
    else
    {
        emit UsunWiersz();
        this->close();
    };

}

//-----TESTY----------------------------------------------------------------------------------------------------

void edycjamagazynu::InformacjaTestowa(QString informacja)
{
    QMessageBox msgBox;
    msgBox.setText(informacja);
    msgBox.exec();
};

