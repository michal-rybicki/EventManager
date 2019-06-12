#include "wystawy.h"
#include "mainwindow.h"
#include "nowestoisko.h"
#include "nowymagazyn.h"
#include "edycjamagazynu.h"
#include "edycjastoisk.h"
#include "ui_mainwindow.h"
#include "edycjawystawy.h"
#include "historia.h"
#include "wystawazbazy.h"
#include <QtSql>


//wystawa starforce;

//-----tworzenie okna glownego----------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //ustawienia testow
    TrybTestowania=true;
    testySprawdzajStoiska=0;
    testySprawdzajStareMagazyny=0;


    //ustawienia ogolne
    starforce=new wystawa;
    TrybWyswietlania=0;//domyslnie wyswietla dane w jednostkach wybranych przez uzytkownika
    TrybWyswietlaniaMagazynow=0;//domyslnie wyswietla ilosc pobranych elementow z magazynu
    ui->setupUi(this);
    ui->wystawcy->setColumnCount(7);
    ui->mainToolBar->hide();//ukrywanie domyslnego paska narzedzi
    this->setFixedSize(860,630);//ustawianie stalego rozmiaru okna
    ui->zerezerwowane->setChecked(true);

    //zmienne odpowadajace za szerokosc kolumn
    int SzerokoscNazwy=300;
    int SzerokoscInnych=75;
    int SzerokoscBarier=110;

    //ustawianie szerokosci kolumn w tabeli stoisk
    ui->wystawcy->setColumnWidth(0,SzerokoscNazwy);
    for (int i=1; i<8; i++)
    ui->wystawcy->setColumnWidth(i,SzerokoscInnych);
    for (int i=3; i<5; i++)
    ui->wystawcy->setColumnWidth(i,SzerokoscBarier);

    //ustawianie szerokosci kolumn w tabeli magazynow
    ui->suma->setColumnWidth(0,SzerokoscNazwy);
    for (int i=1; i<7; i++)
    ui->suma->setColumnWidth(i,SzerokoscInnych);
    for (int i=3; i<5; i++)
    ui->suma->setColumnWidth(i,SzerokoscBarier);

    //tworzenie polaczenia z baza danych
    PolaczZBazaDanych();

    //poprawianie pustej bazy danych - do testow
    if (db.isOpen())
    {
        QSqlQuery query;
        query.exec("INSERT INTO typ_sprzetu (ID,nazwa) values (0,\"stol\");");
        query.exec("INSERT INTO typ_sprzetu (ID,nazwa) values (1,\"gablota\");");
        query.exec("INSERT INTO typ_sprzetu (ID,nazwa) values (2,\"bariera slupkowa\");");
        query.exec("INSERT INTO typ_sprzetu (ID,nazwa) values (3,\"bariera stalowa\");");
        query.exec("INSERT INTO typ_sprzetu (ID,nazwa) values (4,\"krzeslo\");");
        query.exec("INSERT INTO typ_sprzetu (ID,nazwa) values (5,\"prad\");");
        query.exec("INSERT INTO typ_sprzetu (ID,nazwa) values (6,\"stoly pod gabloty\");");
        query.exec("INSERT INTO wystawy (ID, nazwa) values (-999, \"puste\");");
        query.exec("INSERT INTO wystawy (ID, nazwa) values (-1000, \"usun\");");
        query.exec("INSERT INTO jednostki (ID,nazwa) values (0,\"szt.\");");
        query.exec("INSERT INTO jednostki (ID,nazwa) values (1,\"m2\");");
        query.exec("INSERT INTO jednostki (ID,nazwa) values (2,\"mb\");");
        query.exec("INSERT INTO jednostki (ID,nazwa) values (3,\"lawki szkolne\");");
        query.exec("INSERT INTO magazyny (ID, nazwa) values (-999, \"brak\");");
        query.exec("INSERT INTO zaplecze (magazyny_ID, wystawy_ID) values (-999, -999);");
        query.exec("INSERT INTO zaplecze_archiwum (magazyny_ID, wystawy_ID, edycja) values (-999, -999, \"2000-01-01\");");
    }

    NowyPlik();
}

//-----laczenie z baza danych----------------------------------------------
void MainWindow::PolaczZBazaDanych()
{

        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName("starforce");
        db.setUserName("root");
        db.setPassword("ncc1701E#");
        db.open();

};


//-----destruktor----------------------------------------------------------

MainWindow::~MainWindow()
{
    delete ui;
}

//----otwieranie formularza dodawania stoiska------------------------------------------------------------------

void MainWindow::on_pushButtonStoisko_clicked()
{
    Nowestoisko *ZapytanieOStoisko;
    ZapytanieOStoisko = new Nowestoisko(this);
    connect(ZapytanieOStoisko,SIGNAL(StworzoneStoisko(stoisko*)),this,SLOT(DodajStoisko(stoisko*)));
    connect(this,SIGNAL(AktualnaWystawaDoNowegoStoiska(wystawa*)),ZapytanieOStoisko,SLOT(UstawMagazyny(wystawa*)));
    emit AktualnaWystawaDoNowegoStoiska(starforce);
    ZapytanieOStoisko->exec();

}

//-----dodawanie stoiska-------------------------------------------------------------------------

void MainWindow::DodajStoisko(stoisko * N)
{
    starforce->dodaj_stoisko(N);
    DopiszStoisko(N);
    UaktualnijTabeleMagazynow();
    if (!ui->EdytujStoiska->isEnabled()) ui->EdytujStoiska->setEnabled(true);
}

//-----dopisywanie stoiska do tabeli-------------------------------------------------------------------------

void MainWindow::DopiszStoisko(stoisko * N)
{
    ui->wystawcy->setRowCount(ui->wystawcy->rowCount()+1);
    QTableWidgetItem *NowaNazwa;
    NowaNazwa=new QTableWidgetItem(N->nazwa);
    NowaNazwa->setFlags(NowaNazwa->flags() ^ Qt::ItemIsEditable);
    ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),0,NowaNazwa);

    if (TrybWyswietlania==0)
    {
        QTableWidgetItem *NoweStoly;
        if (N->stoly.typJU==0)
            NoweStoly=new QTableWidgetItem((QString::number(N->stoly.iloscWJU))+" szt.");
        else if (N->stoly.typJU==1)
            NoweStoly=new QTableWidgetItem((QString::number(N->stoly.iloscWJU))+" m2");
        else if (N->stoly.typJU==2)
            NoweStoly=new QTableWidgetItem((QString::number(N->stoly.iloscWJU))+" mb");
        else if (N->stoly.typJU==3)
            NoweStoly=new QTableWidgetItem((QString::number(N->stoly.iloscWJU))+" ławek");
        NoweStoly->setFlags(NoweStoly->flags() ^ Qt::ItemIsEditable);

        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),1,NoweStoly);

        QTableWidgetItem *NoweGabloty;
        if (N->gabloty.typJU==0)
                NoweGabloty=new QTableWidgetItem((QString::number(N->gabloty.iloscWJU))+" szt.");
        else if (N->gabloty.typJU==1)
            NoweGabloty=new QTableWidgetItem((QString::number(N->gabloty.iloscWJU))+" m2");
        NoweGabloty->setFlags(NoweGabloty->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),2,NoweGabloty);

        QTableWidgetItem *NoweBarierkiSlupkowe;
        if (N->barierkiSlupkowe.typJU==0)
            NoweBarierkiSlupkowe=new QTableWidgetItem((QString::number(N->barierkiSlupkowe.iloscWJU))+" szt.");
        else if (N->barierkiSlupkowe.typJU==1)
            NoweBarierkiSlupkowe=new QTableWidgetItem((QString::number(N->barierkiSlupkowe.iloscWJU))+" mb");
        NoweBarierkiSlupkowe->setFlags(NoweBarierkiSlupkowe->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),3,NoweBarierkiSlupkowe);

        QTableWidgetItem *NoweBarierkiStalowe;
        if (N->barierkiStalowe.typJU==0)
            NoweBarierkiStalowe=new QTableWidgetItem((QString::number(N->barierkiStalowe.iloscWJU))+" szt.");
        else if (N->barierkiStalowe.typJU==1)
            NoweBarierkiStalowe=new QTableWidgetItem((QString::number(N->barierkiStalowe.iloscWJU))+" mb");
        NoweBarierkiStalowe->setFlags(NoweBarierkiStalowe->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),4,NoweBarierkiStalowe);

        QTableWidgetItem *NoweKrzesla;
        NoweKrzesla=new QTableWidgetItem((QString::number(N->krzesla.iloscWJU))+" szt.");
        NoweKrzesla->setFlags(NoweKrzesla->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),5,NoweKrzesla);

        QTableWidgetItem *NoweZapotrzebowaniePrad;
        if (N->zapotrzebowaniePrad==0)
            NoweZapotrzebowaniePrad=new QTableWidgetItem("NIE");
        else if (N->zapotrzebowaniePrad==1)
        NoweZapotrzebowaniePrad=new QTableWidgetItem("TAK");
        NoweZapotrzebowaniePrad->setFlags(NoweZapotrzebowaniePrad->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),6,NoweZapotrzebowaniePrad);
    }
    else
    {
        QTableWidgetItem *NoweStoly;
        NoweStoly=new QTableWidgetItem((QString::number(N->stoly.iloscWSzt))+" szt.");
        NoweStoly->setFlags(NoweStoly->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),1,NoweStoly);

        QTableWidgetItem *NoweGabloty;
        NoweGabloty=new QTableWidgetItem((QString::number(N->gabloty.iloscWSzt))+" szt.");
        NoweGabloty->setFlags(NoweGabloty->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),2,NoweGabloty);

        QTableWidgetItem *NoweBarierkiSlupkowe;
        NoweBarierkiSlupkowe=new QTableWidgetItem((QString::number(N->barierkiSlupkowe.iloscWSzt))+" szt.");
        NoweBarierkiSlupkowe->setFlags(NoweBarierkiSlupkowe->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),3,NoweBarierkiSlupkowe);

        QTableWidgetItem *NoweBarierkiStalowe;
        NoweBarierkiStalowe=new QTableWidgetItem((QString::number(N->barierkiStalowe.iloscWSzt))+" szt.");
        NoweBarierkiStalowe->setFlags(NoweBarierkiStalowe->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),4,NoweBarierkiStalowe);

        QTableWidgetItem *NoweKrzesla;
        NoweKrzesla=new QTableWidgetItem((QString::number(N->krzesla.iloscWSzt))+" szt.");
        NoweKrzesla->setFlags(NoweKrzesla->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),5,NoweKrzesla);

        QTableWidgetItem *NoweZapotrzebowaniePrad;
        if (N->zapotrzebowaniePrad==0)
            NoweZapotrzebowaniePrad=new QTableWidgetItem("NIE");
        else if (N->zapotrzebowaniePrad==1)
        NoweZapotrzebowaniePrad=new QTableWidgetItem("TAK");
        NoweZapotrzebowaniePrad->setFlags(NoweZapotrzebowaniePrad->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),6,NoweZapotrzebowaniePrad);
    }
}

//----otwieranie formularza dodawania magazynu------------------------------------------------------------------

void MainWindow::on_pushButtonSprzet_clicked()
{
    nowymagazyn *ZapytanieOMagazyn;
    ZapytanieOMagazyn = new nowymagazyn(this);
    connect(ZapytanieOMagazyn,SIGNAL(StworzonyMagazyn(magazyn*)),this,SLOT(DodajMagazyn(magazyn*)));
    ZapytanieOMagazyn->exec();
};

//-----dodawanie magazynu-------------------------------------------------------------------------------------

void MainWindow::DodajMagazyn(magazyn * N)
{
    starforce->dodaj_magazyn(N);
    if (!ui->EdytujMagazyny->isEnabled()) ui->EdytujMagazyny->setEnabled(true);
    DopiszMagazyn(N);
};

void MainWindow::DopiszMagazyn(magazyn * N)
{
    ui->suma->setRowCount(ui->suma->rowCount()+1);
    QTableWidgetItem *NowaNazwa;
    NowaNazwa=new QTableWidgetItem(N->nazwa);
    NowaNazwa->setFlags(NowaNazwa->flags() ^ Qt::ItemIsEditable);
    ui->suma->setItem(((ui->suma->rowCount())-1),0,NowaNazwa);
    UaktualnijTabeleMagazynow();

};

//-----wybor trybu wyswietlania magazynow-----------------------------------------------------------------------------

void MainWindow::on_zerezerwowane_clicked()
{
    TrybWyswietlaniaMagazynow=0;
    UaktualnijTabeleMagazynow();
}

void MainWindow::on_wolne_clicked()
{
    TrybWyswietlaniaMagazynow=1;
    UaktualnijTabeleMagazynow();
}

void MainWindow::on_wsumie_clicked()
{
    TrybWyswietlaniaMagazynow=2;
    UaktualnijTabeleMagazynow();
}

//-----uaktualnianie sumy w tabeli-----------------------------------------------------------------------------

void MainWindow::UaktualnijTabeleMagazynow()
{
    if (starforce->zaplecze.count()>1)
    {
        ui->EdytujMagazyny->setEnabled(true);
    }
    else
    {
        ui->EdytujMagazyny->setEnabled(false);
    };

    int s=0;
    for (int i=1; i<starforce->zaplecze.count();i++)
    {
        s=0;
        QTableWidgetItem *NowaNazwa;
         NowaNazwa=new QTableWidgetItem(((starforce->zaplecze[i].nazwa)));
         ui->suma->setItem(i-1,0,NowaNazwa);
        //if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            QTableWidgetItem *NoweStoly;
            if (TrybWyswietlaniaMagazynow==0)
                NoweStoly=new QTableWidgetItem((QString::number(starforce->zaplecze[i].stolyPobrane.iloscWSzt))+" szt.");
            else if (TrybWyswietlaniaMagazynow==1)
                NoweStoly=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[0]->ilosc-starforce->zaplecze[i].stolyPobrane.iloscWSzt))+" szt.");
            else if (TrybWyswietlaniaMagazynow==2)
                NoweStoly=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[0]->ilosc))+" szt.");
            NoweStoly->setFlags(NoweStoly->flags() ^ Qt::ItemIsEditable);
            ui->suma->setItem(i-1,1,NoweStoly);
        };
        s++;
        //if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            QTableWidgetItem *NoweGabloty;
            if (TrybWyswietlaniaMagazynow==0)
                NoweGabloty=new QTableWidgetItem((QString::number(starforce->zaplecze[i].gablotyPobrane.iloscWSzt))+" szt.");
            else if (TrybWyswietlaniaMagazynow==1)
                NoweGabloty=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[1]->ilosc-starforce->zaplecze[i].gablotyPobrane.iloscWSzt))+" szt.");
            else if (TrybWyswietlaniaMagazynow==2)
                NoweGabloty=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[1]->ilosc))+" szt.");
            NoweGabloty->setFlags(NoweGabloty->flags() ^ Qt::ItemIsEditable);
            ui->suma->setItem(i-1,2,NoweGabloty);
        };
        s++;
        //if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            QTableWidgetItem *NoweBarierkiSlupkowe;
            if (TrybWyswietlaniaMagazynow==0)
                NoweBarierkiSlupkowe=new QTableWidgetItem((QString::number(starforce->zaplecze[i].barierkiSlupkowePobrane.iloscWSzt))+" szt.");
            else if (TrybWyswietlaniaMagazynow==1)
                NoweBarierkiSlupkowe=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[2]->ilosc-starforce->zaplecze[i].barierkiSlupkowePobrane.iloscWSzt))+" szt.");
            else if (TrybWyswietlaniaMagazynow==2)
                NoweBarierkiSlupkowe=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[2]->ilosc))+" szt.");
            NoweBarierkiSlupkowe->setFlags(NoweBarierkiSlupkowe->flags() ^ Qt::ItemIsEditable);
            ui->suma->setItem(i-1,3,NoweBarierkiSlupkowe);
        };
        s++;
        //if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            QTableWidgetItem *NoweBarierkiStalowe;
            if (TrybWyswietlaniaMagazynow==0)
                NoweBarierkiStalowe=new QTableWidgetItem((QString::number(starforce->zaplecze[i].barierkiStalowePobrane.iloscWSzt))+" szt.");
            else if (TrybWyswietlaniaMagazynow==1)
                NoweBarierkiStalowe=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[3]->ilosc-starforce->zaplecze[i].barierkiStalowePobrane.iloscWSzt))+" szt.");
            else if (TrybWyswietlaniaMagazynow==2)
                NoweBarierkiStalowe=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[3]->ilosc))+" szt.");
            NoweBarierkiStalowe->setFlags(NoweBarierkiStalowe->flags() ^ Qt::ItemIsEditable);
            ui->suma->setItem(i-1,4,NoweBarierkiStalowe);
        };
        s++;
        //if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            QTableWidgetItem *NoweKrzesla;
            if (TrybWyswietlaniaMagazynow==0)
                NoweKrzesla=new QTableWidgetItem((QString::number(starforce->zaplecze[i].krzeslaPobrane.iloscWSzt))+" szt.");
            else if (TrybWyswietlaniaMagazynow==1)
                NoweKrzesla=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[4]->ilosc-starforce->zaplecze[i].krzeslaPobrane.iloscWSzt))+" szt.");
            else if (TrybWyswietlaniaMagazynow==2)
                NoweKrzesla=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[4]->ilosc))+" szt.");
            NoweKrzesla->setFlags(NoweKrzesla->flags() ^ Qt::ItemIsEditable);
            ui->suma->setItem(i-1,5,NoweKrzesla);
        };
    };

    Koloruj();
};

//-----uaktualnianie listy stoisk w tabeli-----------------------------------------------------------------------------

void MainWindow::UaktualnijTabeleStoisk()
{
    if (starforce->stoiska.count()>0)
    {
        ui->EdytujStoiska->setEnabled(true);
    }
    else
    {
        ui->EdytujStoiska->setEnabled(false);
    };

    for (int i=0; i<starforce->stoiska.count(); i++)
    {
        ui->wystawcy->item(i,0)->setText(starforce->stoiska[i].nazwa);
        if (TrybWyswietlania==0)
        {
            if (starforce->stoiska[i].stoly.typJU==0)
                ui->wystawcy->item(i,1)->setText((QString::number(starforce->stoiska[i].stoly.iloscWJU))+" szt.");
            else if (starforce->stoiska[i].stoly.typJU==1)
                ui->wystawcy->item(i,1)->setText((QString::number(starforce->stoiska[i].stoly.iloscWJU))+" m2");
            else if (starforce->stoiska[i].stoly.typJU==2)
                ui->wystawcy->item(i,1)->setText((QString::number(starforce->stoiska[i].stoly.iloscWJU))+" mb");
            else if (starforce->stoiska[i].stoly.typJU==3)
                ui->wystawcy->item(i,1)->setText((QString::number(starforce->stoiska[i].stoly.iloscWJU))+" ławek");

            if (starforce->stoiska[i].gabloty.typJU==0)
                ui->wystawcy->item(i,2)->setText((QString::number(starforce->stoiska[i].gabloty.iloscWJU))+" szt.");
            else if (starforce->stoiska[i].gabloty.typJU==1)
                ui->wystawcy->item(i,2)->setText((QString::number(starforce->stoiska[i].gabloty.iloscWJU))+" m2");

            if (starforce->stoiska[i].barierkiSlupkowe.typJU==0)
                ui->wystawcy->item(i,3)->setText((QString::number(starforce->stoiska[i].barierkiSlupkowe.iloscWJU))+" szt.");
            else if (starforce->stoiska[i].barierkiSlupkowe.typJU==1)
                ui->wystawcy->item(i,3)->setText((QString::number(starforce->stoiska[i].barierkiSlupkowe.iloscWJU))+" mb");

            if (starforce->stoiska[i].barierkiStalowe.typJU==0)
                ui->wystawcy->item(i,4)->setText((QString::number(starforce->stoiska[i].barierkiStalowe.iloscWJU))+" szt.");
            else if (starforce->stoiska[i].barierkiStalowe.typJU==1)
                ui->wystawcy->item(i,4)->setText((QString::number(starforce->stoiska[i].barierkiStalowe.iloscWJU))+" mb");
        }
        else
        {
                ui->wystawcy->item(i,1)->setText((QString::number(starforce->stoiska[i].stoly.iloscWSzt))+" szt.");
                ui->wystawcy->item(i,2)->setText((QString::number(starforce->stoiska[i].gabloty.iloscWSzt))+" szt.");
                ui->wystawcy->item(i,3)->setText((QString::number(starforce->stoiska[i].barierkiSlupkowe.iloscWSzt))+" szt.");
                ui->wystawcy->item(i,4)->setText((QString::number(starforce->stoiska[i].barierkiStalowe.iloscWSzt))+" szt.");
        };
        ui->wystawcy->item(i,5)->setText((QString::number(starforce->stoiska[i].krzesla.iloscWJU))+" szt.");

        if (starforce->stoiska[i].zapotrzebowaniePrad==0)
            ui->wystawcy->item(i,6)->setText("NIE");
        else if (starforce->stoiska[i].zapotrzebowaniePrad==1)
            ui->wystawcy->item(i,6)->setText("TAK");
    };
};

//-----kolorowanie tabel------------------------------------------------------------------------------------------

void MainWindow::Koloruj()
{
    QColor zielony;
    QColor czerwony;
    QColor zolty;
    zielony.setRgb(200,250,200);
    czerwony.setRgb(250,200,200);
    zolty.setRgb(250,250,160);

    for (int i=0; i < starforce->zaplecze.count()-1; i++)
    {
        for (int k=1; k<6; k++)
        {
            ui->suma->item(i,k)->setBackground(zielony);
        };
    };

    for (int i=0; i < starforce->stoiska.count(); i++)
    {
        for (int k=1; k<6; k++)
        {
            ui->wystawcy->item(i,k)->setBackground(zielony);
        };
    };

    for (int i=1;i<starforce->zaplecze.count();i++)
    {
        if (starforce->zaplecze[i].stolyPobrane.iloscWSzt > starforce->zaplecze[i].sprzetNaStanie[0]->ilosc)
        {
            ui->suma->item(i-1,1)->setBackground(czerwony);
            for (int k=0; k < starforce->stoiska.count(); k++)
            {
                if (starforce->stoiska[k].stoly.nrMagazynu==i) ui->wystawcy->item(k,1)->setBackground(czerwony);

            };
        };
        if (starforce->zaplecze[i].gablotyPobrane.iloscWSzt > starforce->zaplecze[i].sprzetNaStanie[1]->ilosc)
        {
            ui->suma->item(i-1,2)->setBackground(czerwony);
            for (int k=0; k < starforce->stoiska.count(); k++)
            {
                if (starforce->stoiska[k].gabloty.nrMagazynu==i) ui->wystawcy->item(k,2)->setBackground(czerwony);

            };
        };
        if (starforce->zaplecze[i].barierkiSlupkowePobrane.iloscWSzt > starforce->zaplecze[i].sprzetNaStanie[2]->ilosc)
        {
            ui->suma->item(i-1,3)->setBackground(czerwony);
            for (int k=0; k < starforce->stoiska.count(); k++)
            {
                if (starforce->stoiska[k].barierkiSlupkowe.nrMagazynu==i) ui->wystawcy->item(k,3)->setBackground(czerwony);

            };
        };
        if (starforce->zaplecze[i].barierkiStalowePobrane.iloscWSzt > starforce->zaplecze[i].sprzetNaStanie[3]->ilosc)
        {
            ui->suma->item(i-1,4)->setBackground(czerwony);
            for (int k=0; k < starforce->stoiska.count(); k++)
            {
                if (starforce->stoiska[k].barierkiStalowe.nrMagazynu==i) ui->wystawcy->item(k,4)->setBackground(czerwony);

            };
        };
        if (starforce->zaplecze[i].krzeslaPobrane.iloscWSzt > starforce->zaplecze[i].sprzetNaStanie[4]->ilosc)
        {
            ui->suma->item(i-1,5)->setBackground(czerwony);
            for (int k=0; k < starforce->stoiska.count(); k++)
            {
                if (starforce->stoiska[k].krzesla.nrMagazynu==i) ui->wystawcy->item(k,5)->setBackground(czerwony);
            };
        };
    }
    for (int i=0; i < starforce->stoiska.count(); i++)
    {

            if (starforce->stoiska[i].krzesla.nrMagazynu==0 && starforce->stoiska[i].krzesla.iloscWJU>0) ui->wystawcy->item(i,5)->setBackground(zolty);
            if (starforce->stoiska[i].barierkiStalowe.nrMagazynu==0 && starforce->stoiska[i].barierkiStalowe.iloscWJU>0) ui->wystawcy->item(i,4)->setBackground(zolty);
            if (starforce->stoiska[i].barierkiSlupkowe.nrMagazynu==0 && starforce->stoiska[i].barierkiSlupkowe.iloscWJU>0) ui->wystawcy->item(i,3)->setBackground(zolty);
            if (starforce->stoiska[i].gabloty.nrMagazynu==0 && starforce->stoiska[i].gabloty.iloscWJU>0) ui->wystawcy->item(i,2)->setBackground(zolty);
            if (starforce->stoiska[i].stoly.nrMagazynu==0 && starforce->stoiska[i].stoly.iloscWJU>0) ui->wystawcy->item(i,1)->setBackground(zolty);
    };
}

//-----usuwanie wiersza z tabeli stoisk------------------------------------------------------------------------

void MainWindow::UsunWierszStoisk()
{
    ui->wystawcy->removeRow(ui->wystawcy->rowCount()-1);
    UaktualnijTabeleStoisk();
    UaktualnijTabeleMagazynow();
    if (TrybTestowania) TestStoisk();//do testow

}

//-----usuwanie wiersza z tabeli magazynow------------------------------------------------------------------------

void MainWindow::UsunWierszMagazynow()
{
    ui->suma->removeRow(ui->suma->rowCount()-1);
    UaktualnijTabeleMagazynow();
}

//-----uaktualnianie listy magazynow---------------------------------------------------------------------------------

void MainWindow::UaktualnijMagazyny()
{
    if (TrybTestowania) TestStarychMagazynow();//do testow
    starforce->przelicz_wszystko();
    UaktualnijTabeleMagazynow();
};

//-----uaktualnianie listy stoisk-------------------------------------------------------------------------------------

void MainWindow::UaktualnijStoiska()
{
    if (TrybTestowania) TestStarychMagazynow();//do testow
    starforce->przelicz_wszystko();
    UaktualnijTabeleStoisk();
    UaktualnijTabeleMagazynow();
};

//----otwieranie formularza edycji magazynow------------------------------------------------------------------

void MainWindow::on_EdytujMagazyny_clicked()
{
    edycjamagazynu * ListaMagazynow;
    ListaMagazynow = new edycjamagazynu(this);
    connect(this,SIGNAL(AktualnaWystawaDoMagazynow(wystawa*)),ListaMagazynow,SLOT(Ustaw(wystawa*)));
    connect(ListaMagazynow,SIGNAL(EdycjaMagazynowZakonczona()),this,SLOT(UaktualnijMagazyny()));
    connect(ListaMagazynow,SIGNAL(UsunWiersz()),this,SLOT(UsunWierszMagazynow()));
    emit AktualnaWystawaDoMagazynow(starforce);
    ListaMagazynow->exec();
};

//----otwieranie formularza edycji stoisk------------------------------------------------------------------

void MainWindow::on_EdytujStoiska_clicked()
{
    if (TrybTestowania) TestStarychMagazynow();//do testow
    edycjastoisk * ListaStoisk;
    ListaStoisk = new edycjastoisk(this);
    connect(this,SIGNAL(AktualnaWystawaDoStoisk(wystawa*)),ListaStoisk,SLOT(Ustaw(wystawa*)));
    connect(ListaStoisk,SIGNAL(EdycjaStoiskZakonczona()),this,SLOT(UaktualnijStoiska()));
    connect(ListaStoisk,SIGNAL(UsunWiersz()),this,SLOT(UsunWierszStoisk()));
    emit AktualnaWystawaDoStoisk(starforce);
    ListaStoisk->exec();
};

//-----otwieranie pliku-----------------------------------------------------------------------------------

void MainWindow::on_actionOtw_rz_triggered()
{

    QString sciezkaDoPliku = QFileDialog::getOpenFileName(this,tr("Otwórz..."), "/home/", tr("Pliki event (*.event)"));
    plik.setFileName(sciezkaDoPliku);
    if(!plik.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    ui->suma->setRowCount(0);//czyszczenie tabel
    ui->wystawcy->setRowCount(0);//czyszczenie tabel
    ZaladujPlik();//wczytywanie danych z pliku do wystawy i generowanie tabel
    UaktualnijStoiska();//przeliczanie wystawy i uaktualnianie tabel
    plik.close();
    ui->actionZapisz->setEnabled(true);
    this->setWindowTitle(sciezkaDoPliku);

};

//-----czytanie z pliku-----------------------------------------------------------------------------------

void MainWindow::ZaladujPlik()
{
    QString wpis;
    char znak;
    delete starforce;
    wpis=plik.readLine();
    wpis.chop(1);
    starforce= new wystawa(wpis);
    wpis=plik.readLine();
    starforce->ID=(wpis).toInt();
    wpis=plik.readLine();
    starforce->data.setDate(wpis.mid(0,4).toInt(),wpis.mid(5,2).toInt(),wpis.mid(8,2).toInt());
    znak=(plik.readLine())[0];
    while (znak!='K')
    {
        if (znak=='S')
        {
            stoisko *N;
            N = new stoisko;
            wpis=plik.readLine();
            wpis.chop(1);
            /*01*/N->nazwa=wpis;
            wpis=plik.readLine();
            /*02*/N->ID=(wpis).toInt();
            wpis=plik.readLine();
            /*03*/N->stoly.iloscWJU=((wpis).toDouble());
            wpis=plik.readLine();
            /*04*/N->stoly.typJU=(wpis).toInt();
            wpis=plik.readLine();
            /*05*/N->stoly.nrMagazynu=(wpis).toInt();
            N->stoly.nrStaregoMagazynu=N->stoly.nrMagazynu;
            wpis=plik.readLine();
            /*06*/N->gabloty.iloscWJU=(wpis).toDouble();
            wpis=plik.readLine();
            /*07*/N->gabloty.typJU=(wpis).toInt();
            wpis=plik.readLine();
            /*08*/N->gabloty.nrMagazynu=(wpis).toInt();
            N->gabloty.nrStaregoMagazynu=N->gabloty.nrMagazynu;
            wpis=plik.readLine();
            /*09*/N->barierkiSlupkowe.iloscWJU=(wpis).toDouble();
            wpis=plik.readLine();
            /*10*/N->barierkiSlupkowe.typJU=(wpis).toInt();
            wpis=plik.readLine();
            /*11*/N->barierkiSlupkowe.nrMagazynu=(wpis).toInt();
            N->barierkiSlupkowe.nrStaregoMagazynu=N->barierkiSlupkowe.nrMagazynu;
            wpis=plik.readLine();
            /*12*/N->barierkiStalowe.iloscWJU=(wpis).toDouble();
            wpis=plik.readLine();
            /*13*/N->barierkiStalowe.typJU=(wpis).toInt();
            wpis=plik.readLine();
            /*14*/N->barierkiStalowe.nrMagazynu=(wpis).toInt();
            N->barierkiStalowe.nrStaregoMagazynu=N->barierkiStalowe.nrMagazynu;
            wpis=plik.readLine();
            /*15*/N->krzesla.iloscWJU=(wpis).toDouble();
            wpis=plik.readLine();
            /*16*/N->krzesla.nrMagazynu=(wpis).toInt();
            N->krzesla.nrStaregoMagazynu=N->krzesla.nrMagazynu;
            wpis=plik.readLine();
            /*17*/N->zapotrzebowaniePrad=(wpis).toInt();
            wpis=plik.readLine();
            /*18*/N->doliczStolyPodGabloty=(wpis).toInt();
            starforce->stoiska.append(*N);
            DopiszStoisko(N);
        }
        else
        {
            magazyn *N;
            N= new magazyn;
            wpis=plik.readLine();
            wpis.chop(1);
            /*01*/N->nazwa=wpis;
            wpis=plik.readLine();
            /*02*/N->ID=wpis.toInt();
            wpis=plik.readLine();
            /*03*/ N->sprzetNaStanie[0]->ilosc=wpis.toInt();
            wpis=plik.readLine();
            /*04*/N->sprzetNaStanie[0]->dlugosc=wpis.toDouble();
            wpis=plik.readLine();
            /*05*/N->sprzetNaStanie[0]->szerokosc=wpis.toDouble();
            wpis=plik.readLine();
            /*06*/N->sprzetNaStanie[1]->ilosc=wpis.toInt();
            wpis=plik.readLine();
            /*07*/N->sprzetNaStanie[1]->dlugosc=wpis.toDouble();
            wpis=plik.readLine();
            /*08*/N->sprzetNaStanie[1]->szerokosc=wpis.toDouble();
            wpis=plik.readLine();
            /*09*/N->sprzetNaStanie[2]->ilosc=wpis.toInt();
            wpis=plik.readLine();
            /*10*/N->sprzetNaStanie[2]->dlugosc=wpis.toDouble();
            wpis=plik.readLine();
            /*11*/N->sprzetNaStanie[3]->ilosc=wpis.toInt();
            wpis=plik.readLine();
            /*12*/N->sprzetNaStanie[3]->dlugosc=wpis.toDouble();
            wpis=plik.readLine();
            /*13*/N->sprzetNaStanie[4]->ilosc=wpis.toInt();
            starforce->zaplecze.append(*N);
            DopiszMagazyn(N);
        }
    znak=(plik.readLine())[0];
    }
};


//-----pobieranie danych o wystawie z bazy danych-------------------------------------------------------------------

void MainWindow::on_actionWczytaj_z_bazy_danych_triggered()
{

    if (!db.isOpen()) PolaczZBazaDanych();
    if (db.isOpen())
    {
        //uaktualnianie pola o dacie ostatniej wizyty w bazie
        QSqlQuery query1;
        QDate dataWizyty;
        dataWizyty.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        query1.prepare("UPDATE wystawy SET ostatnia_wizyta=:aktualna_data");
        query1.bindValue(":aktualna_data",QString::number(dataWizyty.year())+"-"+QString::number(dataWizyty.month())+"-"+QString::number(dataWizyty.day()));
        query1.exec();

        //QSqlQuery query;
        //QStringList nazwy;
        //QString nazwa;
        //query.exec("select nazwa from wystawy where ID>-999");
        //while (query.next())
        //{
        //    nazwy.append(query.value(0).toString());
        //}
        //QInputDialog *wybor= new QInputDialog();
        //wybor->setFixedSize(400,200);
        //nazwa=wybor->getItem(0,"Wybierz wystawe","Nazwa: " ,nazwy,0,false);
        //query.prepare("select ID from wystawy where nazwa like :nazwa and ID>-999");
        //query.bindValue(":nazwa",nazwa);
        //query.exec();
        //query.first();
        //int ID_wystawy=query.value(0).toInt();
        wystawazbazy * wybor;
        wybor=new wystawazbazy();
        connect(wybor,SIGNAL(wybranoWystawe(int)),this,SLOT(wybranoWystawe(int)));
        wybor->exec();


    }
    else
    {
        InformacjaTestowa("Brak polaczenia z baza danych!");
    }


};

void MainWindow::wybranoWystawe(int ID_wystawy)
{
    ui->suma->setRowCount(0);//czyszczenie tabel
    ui->wystawcy->setRowCount(0);//czyszczenie tabel
    WczytajZBazy(ID_wystawy);//wczytywanie danych z bazy danych do wystawy i generowanie tabel
    UaktualnijStoiska();//przeliczanie wystawy i uaktualnianie tabel
    this->setWindowTitle("Baza Danych -> "+starforce->nazwa);
}

void MainWindow::WczytajZBazy(int ID_wystawy)
{
    //nazwa wystawy, ID i data
    QString wpis;
    QSqlQuery Query;
    Query.prepare("select nazwa from wystawy where ID=:ID_wystawy");
    Query.bindValue(":ID_wystawy",ID_wystawy);
    Query.exec();
    Query.first();
    delete starforce;
    wpis=Query.value(0).toString();
    starforce= new wystawa(wpis);
    starforce->ID=ID_wystawy;
    Query.prepare("select termin from wystawy where ID=:ID_wystawy");
    Query.bindValue(":ID_wystawy",ID_wystawy);
    Query.exec();
    Query.first();
    wpis=Query.value(0).toString();
    starforce->data.setDate(wpis.mid(0,4).toInt(),wpis.mid(5,2).toInt(),wpis.mid(8,2).toInt());
    if (starforce->data<QDate::currentDate())
        {
            starforce->data=QDate::currentDate();
            starforce->data=starforce->data.addMonths(1);
            Query.prepare("DELETE FROM zapotrzebowanie WHERE wystawy_ID=:ID;");
            Query.bindValue(":ID",QString::number(starforce->ID));
            Query.exec();
            Query.prepare("DELETE FROM zaplecze WHERE wystawy_ID=:ID;");
            Query.bindValue(":ID",QString::number(starforce->ID));
            Query.exec();
        }

    //wczytywanie magazynow
    Query.prepare("select * from zaplecze where wystawy_ID=:ID_wystawy");
    Query.bindValue(":ID_wystawy",ID_wystawy);
    Query.exec();
    while(Query.next())
    {
        magazyn *N;
        N= new magazyn;
        wpis=Query.value(1).toString();
        /*02*/N->ID=wpis.toInt();
        QSqlQuery query1;
        query1.exec("SELECT nazwa FROM magazyny WHERE ID="+QString::number( N->ID )+";");
        query1.first();
        /*01*/N->nazwa=query1.value(0).toString();
        wpis=Query.value(2).toString();
        /*03*/ N->sprzetNaStanie[0]->ilosc=wpis.toInt();
        wpis=Query.value(8).toString();
        /*04*/N->sprzetNaStanie[0]->dlugosc=wpis.toDouble();
        wpis=Query.value(9).toString();
        /*05*/N->sprzetNaStanie[0]->szerokosc=wpis.toDouble();
        wpis=Query.value(3).toString();
        /*06*/N->sprzetNaStanie[1]->ilosc=wpis.toInt();
        wpis=Query.value(10).toString();
        /*07*/N->sprzetNaStanie[1]->dlugosc=wpis.toDouble();
        wpis=Query.value(9).toString();
        /*08*/N->sprzetNaStanie[1]->szerokosc=wpis.toDouble();
        wpis=Query.value(5).toString();
        /*09*/N->sprzetNaStanie[2]->ilosc=wpis.toInt();
        wpis=Query.value(12).toString();
        /*10*/N->sprzetNaStanie[2]->dlugosc=wpis.toDouble();
        wpis=Query.value(4).toString();
        /*11*/N->sprzetNaStanie[3]->ilosc=wpis.toInt();
        wpis=Query.value(11).toString();
        /*12*/N->sprzetNaStanie[3]->dlugosc=wpis.toDouble();
        wpis=Query.value(6).toString();
        /*13*/N->sprzetNaStanie[4]->ilosc=wpis.toInt();
        starforce->zaplecze.append(*N);
        DopiszMagazyn(N);
    }

    //wczytywanie stoisk
    Query.prepare("select distinct wystawcy_ID from zapotrzebowanie where wystawy_ID=:ID_wystawy");
    Query.bindValue(":ID_wystawy",ID_wystawy);
    Query.exec();
    QList<int> ID_stoiska;
    while(Query.next())
    {
        ID_stoiska.append(Query.value(0).toInt());
    }


    for (int i=0; i<ID_stoiska.length(); i++)
    {
        Query.prepare("select nazwa from wystawcy where ID=:ID_wystawcy");
        Query.bindValue(":ID_wystawcy",ID_stoiska[i]);
        Query.exec();
        Query.first();
        QString nowa_nazwa=Query.value(0).toString();
        Query.prepare("select * from zapotrzebowanie where wystawy_ID=:ID_wystawy and wystawcy_ID=:ID_wystawcy");
        Query.bindValue(":ID_wystawy",ID_wystawy);
        Query.bindValue(":ID_wystawcy",ID_stoiska[i]);
        Query.exec();
        stoisko *N;
        N = new stoisko;
        /*01*/N->nazwa=nowa_nazwa;
        /*02*/N->ID=ID_stoiska[i];
        while(Query.next())
        {
            if (Query.value(2).toInt()==0)
            {
                wpis=Query.value(3).toString();
                /*03*/N->stoly.iloscWJU=((wpis).toDouble());
                wpis=Query.value(5).toString();
                /*04*/N->stoly.typJU=(wpis).toInt();
                int ID_magazynu=Query.value(6).toInt();
                /*05*/N->stoly.nrMagazynu=starforce->nrMagazynu(ID_magazynu);
                N->stoly.nrStaregoMagazynu=N->stoly.nrMagazynu;
            }
            else if (Query.value(2).toInt()==1)
            {
                wpis=Query.value(3).toString();
                /*06*/N->gabloty.iloscWJU=((wpis).toDouble());
                wpis=Query.value(5).toString();
                /*07*/N->gabloty.typJU=(wpis).toInt();
                int ID_magazynu=Query.value(6).toInt();
                /*08*/N->gabloty.nrMagazynu=starforce->nrMagazynu(ID_magazynu);
                N->gabloty.nrStaregoMagazynu=N->gabloty.nrMagazynu;
            }
            else if (Query.value(2).toInt()==2)
            {
                wpis=Query.value(3).toString();
                /*09*/N->barierkiSlupkowe.iloscWJU=((wpis).toDouble());
                wpis=Query.value(5).toString();
                /*10*/N->barierkiSlupkowe.typJU=(wpis).toInt();
                int ID_magazynu=Query.value(6).toInt();
                /*11*/N->barierkiSlupkowe.nrMagazynu=starforce->nrMagazynu(ID_magazynu);
                N->barierkiSlupkowe.nrStaregoMagazynu=N->barierkiSlupkowe.nrMagazynu;
            }
            else if (Query.value(2).toInt()==3)
            {
                wpis=Query.value(3).toString();
                /*12*/N->barierkiStalowe.iloscWJU=((wpis).toDouble());
                wpis=Query.value(5).toString();
                /*13*/N->barierkiStalowe.typJU=(wpis).toInt();
                int ID_magazynu=Query.value(6).toInt();
                /*14*/N->barierkiStalowe.nrMagazynu=starforce->nrMagazynu(ID_magazynu);
                N->barierkiStalowe.nrStaregoMagazynu=N->barierkiStalowe.nrMagazynu;
            }
            else if (Query.value(2).toInt()==4)
            {
                wpis=Query.value(3).toString();
                /*15*/N->krzesla.iloscWJU=((wpis).toDouble());
                int ID_magazynu=Query.value(6).toInt();
                /*16*/N->krzesla.nrMagazynu=starforce->nrMagazynu(ID_magazynu);
                N->krzesla.nrStaregoMagazynu=N->krzesla.nrMagazynu;
            }
            else if (Query.value(2).toInt()==5)
            {
                wpis=Query.value(3).toString();
                /*17*/N->zapotrzebowaniePrad=((wpis).toInt());
            }
            else if (Query.value(2).toInt()==6)
            {
                wpis=Query.value(3).toString();
                /*18*/N->doliczStolyPodGabloty=((wpis).toInt());
            }
        }
        starforce->stoiska.append(*N);
        DopiszStoisko(N);
    }


};

//-----wybieranie pliku do zapisu-----------------------------------------------------------------------------------

void MainWindow::on_actionZapisz_triggered()
{
    plik.open(QIODevice::WriteOnly | QIODevice::Text);
    ZapiszDoPliku();
    plik.close();

};

void MainWindow::on_actionZapisz_jako_triggered()
{
    QString sciezkaDoPliku;

    if(sciezkaDoPliku.isEmpty())
        sciezkaDoPliku = QFileDialog::getSaveFileName(this,tr("Zapisz plik jako..."),tr("/home/"),tr("Pliki event (*.event)"));

    if(sciezkaDoPliku.isEmpty())
        return;

    plik.setFileName(sciezkaDoPliku);
    plik.open(QIODevice::WriteOnly | QIODevice::Text);
    ZapiszDoPliku();
    plik.close();
    ui->actionZapisz->setEnabled(true);
    this->setWindowTitle(sciezkaDoPliku);
};

//-----zapisywanie pliku------------------------------------------------------------------------------------------

void MainWindow::ZapiszDoPliku()
{
    QTextStream zapis(&plik);
    zapis << starforce->nazwa << endl;
    zapis << starforce->ID << endl;
    zapis << QString::number(starforce->data.year())+"-"+QString::number(starforce->data.month())+"-"+QString::number(starforce->data.day()) << endl;
    for (int i=1; i<starforce->zaplecze.count(); i++)
    {
        zapis << "M" << endl;
        zapis << starforce->zaplecze[i].nazwa << endl;
        zapis << starforce->zaplecze[i].ID << endl;
        zapis << starforce->zaplecze[i].sprzetNaStanie[0]->ilosc << endl;
        zapis << starforce->zaplecze[i].sprzetNaStanie[0]->dlugosc << endl;
        zapis << starforce->zaplecze[i].sprzetNaStanie[0]->szerokosc << endl;
        zapis << starforce->zaplecze[i].sprzetNaStanie[1]->ilosc << endl;
        zapis << starforce->zaplecze[i].sprzetNaStanie[1]->dlugosc << endl;
        zapis << starforce->zaplecze[i].sprzetNaStanie[1]->szerokosc << endl;
        zapis << starforce->zaplecze[i].sprzetNaStanie[2]->ilosc << endl;
        zapis << starforce->zaplecze[i].sprzetNaStanie[2]->dlugosc << endl;
        zapis << starforce->zaplecze[i].sprzetNaStanie[3]->ilosc << endl;
        zapis << starforce->zaplecze[i].sprzetNaStanie[3]->dlugosc << endl;
        zapis << starforce->zaplecze[i].sprzetNaStanie[4]->ilosc << endl;
     }
     for (int i=0; i<starforce->stoiska.count(); i++)
     {
         zapis << "S" << endl;
         zapis << starforce->stoiska[i].nazwa<< endl;
         zapis << starforce->stoiska[i].ID << endl;
         zapis << starforce->stoiska[i].stoly.iloscWJU<< endl;
         zapis << starforce->stoiska[i].stoly.typJU<< endl;
         zapis << starforce->stoiska[i].stoly.nrMagazynu << endl;
         zapis << starforce->stoiska[i].gabloty.iloscWJU<< endl;
         zapis << starforce->stoiska[i].gabloty.typJU<< endl;
         zapis << starforce->stoiska[i].gabloty.nrMagazynu << endl;
         zapis << starforce->stoiska[i].barierkiSlupkowe.iloscWJU<< endl;
         zapis << starforce->stoiska[i].barierkiSlupkowe.typJU<< endl;
         zapis << starforce->stoiska[i].barierkiSlupkowe.nrMagazynu << endl;
         zapis << starforce->stoiska[i].barierkiStalowe.iloscWJU<< endl;
         zapis << starforce->stoiska[i].barierkiStalowe.typJU<< endl;
         zapis << starforce->stoiska[i].barierkiStalowe.nrMagazynu << endl;
         zapis << starforce->stoiska[i].krzesla.iloscWJU<< endl;
         zapis << starforce->stoiska[i].krzesla.nrMagazynu << endl;
         zapis << starforce->stoiska[i].zapotrzebowaniePrad<< endl;
         zapis << starforce->stoiska[i].doliczStolyPodGabloty<< endl;
     }
     zapis << "K" << endl;
};

//-----zapisywanie do bazy danych-----------------------------------------------------------------------------
void MainWindow::on_actionZapisz_do_bazy_danych_triggered()
{
    if (!db.isOpen()) PolaczZBazaDanych();
    if (db.isOpen())
    {
        ZapiszDoBazy();
    }
    else
    {
        InformacjaTestowa("Brak polaczenia z baza danych!");
    }

};

void MainWindow::ZapiszDoBazy()
{

    //dodawanie wystawy do bazy jezeli nie ma lub uaktualnianie ID wystawy
    if (starforce->ID==0)
    {

        QSqlQuery query;
        query.prepare("select count(*) from wystawy where nazwa like :nazwa");
        query.bindValue(":nazwa",starforce->nazwa);
        query.exec();
        query.first();
        if (query.value(0).toInt()==0)
        {
            query.exec("select max(id) from wystawy");
            query.first();
            starforce->ID=query.value(0).toInt()+1;
            query.exec("INSERT INTO wystawy (nazwa, ID) values (\"" + starforce->nazwa + "\", " + QString::number(starforce->ID) +")");
        }
        else
        {
            query.prepare("select ID from wystawy where nazwa like :nazwa");
            query.bindValue(":nazwa",starforce->nazwa);
            query.exec();
            query.first();
            starforce->ID=query.value(0).toInt();
        }

    }
    //ukatualnianie nazwy okna glownego
    this->setWindowTitle("Baza Danych -> "+starforce->nazwa);

    //uaktualnianie nazwy wystawy i daty
    QSqlQuery query;
    query.prepare("UPDATE wystawy SET termin=:data, nazwa=:nazwa WHERE ID=:ID_wyst" );
    query.bindValue(":ID_wyst",QString::number(starforce->ID));
    query.bindValue(":nazwa",starforce->nazwa);
    query.bindValue(":data",QString::number(starforce->data.year())+"-"+QString::number(starforce->data.month())+"-"+QString::number(starforce->data.day()));
    query.exec();

    //dodawanie wystawcow do bazy jezeli nie ma
    for (int i=0; i<starforce->stoiska.count(); i++)
    {
        QSqlQuery query;
        query.exec("select count(ID) from wystawcy where nazwa like + \""+ starforce->stoiska[i].nazwa +"\"");
        query.first();
        if (query.value(0).toInt()==0)
        {
            query.exec("select max(ID) from wystawcy");
            query.first();
            starforce->stoiska[i].ID=query.value(0).toInt()+1;
            query.exec("INSERT INTO wystawcy (nazwa, ID) values (\"" + starforce->stoiska[i].nazwa  + "\", " + QString::number(starforce->stoiska[i].ID) +")");
        }
        else
        {
             query.exec("select ID from wystawcy where nazwa like + \""+ starforce->stoiska[i].nazwa +"\"");
             query.first();
             starforce->stoiska[i].ID=query.value(0).toInt();
        }
    }

    //dodawanie magazynow do bazy lub uaktualnianie magazynow w bazie
    for (int i=1; i<starforce->zaplecze.count(); i++)
    {
        QSqlQuery query;
        QSqlQuery insupd;
        QSqlQuery query2;
        QSqlQuery query3;

        //dodawanie nazw magazynow do bazy oraz uaktalnianie ID magazynow
        query.prepare("select count(*) from magazyny where nazwa like :nazwa");
        query.bindValue(":nazwa",starforce->zaplecze[i].nazwa);
        query.exec();
        query.first();
        if (query.value(0).toInt()==0)
        {
            query2.exec("select max(ID) from magazyny");
            query2.first();
            starforce->zaplecze[i].ID=query2.value(0).toInt()+1;
            query2.prepare("INSERT INTO magazyny (ID, nazwa) values (:ID, :nazwa)");
            query2.bindValue(":nazwa",starforce->zaplecze[i].nazwa);
            query2.bindValue(":ID",QString::number(starforce->zaplecze[i].ID));
            query2.exec();
        }
        else
        {
            query2.prepare("select * from magazyny where nazwa like :nazwa");
            query2.bindValue(":nazwa",starforce->zaplecze[i].nazwa);
            query2.exec();
            query2.first();
            starforce->zaplecze[i].ID=query2.value(0).toInt();
        }

        query3.exec("select count(*) from zaplecze where magazyny_ID="+ QString::number(starforce->zaplecze[i].ID) +" and wystawy_ID=" + QString::number(starforce->ID));
        query3.first();
        if (query3.value(0).toInt()==0)
        {
            insupd.prepare("INSERT INTO zaplecze (magazyny_ID, wystawy_ID, ilosc_stolow, ilosc_gablot, ilosc_barier_stalowych, ilosc_barier_slupkowych, ilosc_krzesel, szerokosc_stolu, dlugosc_stolu, szerokosc_gabloty, dlugosc_gabloty, dlugosc_bariery_stalowej, dlugosc_bariery_slupkowej)"
                          "values (:ID, :ID_wyst, :IS, :IG, :IBST, :IBSL, :IK, :SST, :DST, :SGB, :DGB, :DBST, :DBSL)");

            insupd.bindValue(":ID_wyst",QString::number(starforce->ID));
            insupd.bindValue(":ID",QString::number(starforce->zaplecze[i].ID));
        }
        else
        {
            insupd.prepare("UPDATE zaplecze SET ilosc_stolow=:IS, ilosc_gablot=:IG, ilosc_barier_stalowych=:IBST, ilosc_barier_slupkowych=:IBSL, ilosc_krzesel=:IK, szerokosc_stolu=:SST, dlugosc_stolu=:DST, szerokosc_gabloty=:SGB, dlugosc_gabloty=:DGB, dlugosc_bariery_stalowej=:DBST, dlugosc_bariery_slupkowej=:DBSL "
                          "where magazyny_ID="+ QString::number(starforce->zaplecze[i].ID) +" and wystawy_ID="+ QString::number(starforce->ID));
        }

            insupd.bindValue(":IS",QString::number(starforce->zaplecze[i].sprzetNaStanie[0]->ilosc));
            insupd.bindValue(":DST",QString::number(starforce->zaplecze[i].sprzetNaStanie[0]->dlugosc));
            insupd.bindValue(":SST",QString::number(starforce->zaplecze[i].sprzetNaStanie[0]->szerokosc));
            insupd.bindValue(":IG",QString::number(starforce->zaplecze[i].sprzetNaStanie[1]->ilosc));
            insupd.bindValue(":DGB",QString::number(starforce->zaplecze[i].sprzetNaStanie[1]->dlugosc));
            insupd.bindValue(":SGB",QString::number(starforce->zaplecze[i].sprzetNaStanie[1]->szerokosc));
            insupd.bindValue(":IBSL",QString::number(starforce->zaplecze[i].sprzetNaStanie[2]->ilosc));
            insupd.bindValue(":DBSL",QString::number(starforce->zaplecze[i].sprzetNaStanie[2]->dlugosc));
            insupd.bindValue(":IBST",QString::number(starforce->zaplecze[i].sprzetNaStanie[3]->ilosc));
            insupd.bindValue(":DBST",QString::number(starforce->zaplecze[i].sprzetNaStanie[3]->dlugosc));
            insupd.bindValue(":IK",QString::number(starforce->zaplecze[i].sprzetNaStanie[4]->ilosc));

            insupd.exec();


    }

    //dodawanie wartosci ilosci pobranych z magazynow
    for (int i=1; i<starforce->zaplecze.count(); i++)
    {
        QSqlQuery query;
        query.prepare("UPDATE zaplecze SET pobrane_stoly=:pobrane_stoly, pobrane_gabloty=:pobrane_gabloty, pobrane_barierki_slupkowe=:pobrane_barierki_slupkowe,  pobrane_barierki_stalowe=:pobrane_barierki_stalowe, pobrane_krzesla=:pobrane_krzesla "
                     "where magazyny_ID="+ QString::number(starforce->zaplecze[i].ID) +" and wystawy_ID="+ QString::number(starforce->ID));
        query.bindValue(":pobrane_stoly", QString::number(starforce->zaplecze[i].stolyPobrane.iloscWSzt));
        query.bindValue(":pobrane_gabloty", QString::number(starforce->zaplecze[i].gablotyPobrane.iloscWSzt));
        query.bindValue(":pobrane_barierki_slupkowe", QString::number(starforce->zaplecze[i].barierkiSlupkowePobrane.iloscWSzt));
        query.bindValue(":pobrane_barierki_stalowe", QString::number(starforce->zaplecze[i].barierkiStalowePobrane.iloscWSzt));
        query.bindValue(":pobrane_krzesla", QString::number(starforce->zaplecze[i].krzeslaPobrane.iloscWSzt));
        query.exec();
    }


    //dodawanie zapotrzebowania stoisk do bazy lub uaktualnianie zapotrzebowania w bazie
    for (int i=0; i<starforce->stoiska.count(); i++)
    {
        for (int j=0; j<=6; j++)
        {
            QSqlQuery query;
            QSqlQuery insupd;
            query.prepare("select count(*) from zapotrzebowanie where wystawy_ID=:ID_wystawy and wystawcy_ID=:ID_wystawcy and typ_sprzetu_ID=:typ_sprzetu");
            query.bindValue(":ID_wystawy", QString::number(starforce->ID));
            query.bindValue(":ID_wystawcy", QString::number(starforce->stoiska[i].ID));
            query.bindValue(":typ_sprzetu", QString::number(j));
            query.exec();
            query.first();
            if (query.value(0).toInt()==0)
            {
                insupd.prepare("INSERT INTO zapotrzebowanie (wystawy_ID, wystawcy_ID, typ_sprzetu_ID, ilosc_podana, ilosc_przeliczona, jednostki_ID, zaplecze_magazyny_ID)"
                               "VALUES (:ID_wystawy, :ID_wystawcy, :typ_sprzetu, :ilosc_podana, :ilosc_przeliczona, :jedn_uzyt, :ID_magazynu)");
                insupd.bindValue(":ID_wystawy",QString::number(starforce->ID));
                insupd.bindValue(":ID_wystawcy",QString::number(starforce->stoiska[i].ID));
                insupd.bindValue(":typ_sprzetu", QString::number(j));
            }
            else
            {
                insupd.prepare("UPDATE zapotrzebowanie SET ilosc_podana=:ilosc_podana, ilosc_przeliczona=:ilosc_przeliczona, jednostki_ID=:jedn_uzyt, zaplecze_magazyny_ID=:ID_magazynu "
                               "where wystawy_ID=" + QString::number(starforce->ID) +" and wystawcy_ID=" + QString::number(starforce->stoiska[i].ID) + " and typ_sprzetu_ID=" + QString::number(j));
            }
            if (j==0)
            {
                insupd.bindValue(":ilosc_podana",QString::number(starforce->stoiska[i].stoly.iloscWJU));
                insupd.bindValue(":ilosc_przeliczona",QString::number(starforce->stoiska[i].stoly.iloscWSzt));
                insupd.bindValue(":jedn_uzyt",QString::number(starforce->stoiska[i].stoly.typJU));
                insupd.bindValue(":ID_magazynu",QString::number(starforce->zaplecze[starforce->stoiska[i].stoly.nrMagazynu].ID));
            }
            else if (j==1)
            {
                insupd.bindValue(":ilosc_podana",QString::number(starforce->stoiska[i].gabloty.iloscWJU));
                insupd.bindValue(":ilosc_przeliczona",QString::number(starforce->stoiska[i].gabloty.iloscWSzt));
                insupd.bindValue(":jedn_uzyt",QString::number(starforce->stoiska[i].gabloty.typJU));
                insupd.bindValue(":ID_magazynu",QString::number(starforce->zaplecze[starforce->stoiska[i].gabloty.nrMagazynu].ID));
            }
            else if (j==2)
            {
                insupd.bindValue(":ilosc_podana",QString::number(starforce->stoiska[i].barierkiSlupkowe.iloscWJU));
                insupd.bindValue(":ilosc_przeliczona",QString::number(starforce->stoiska[i].barierkiSlupkowe.iloscWSzt));
                insupd.bindValue(":jedn_uzyt",QString::number(starforce->stoiska[i].barierkiSlupkowe.typJU));
                insupd.bindValue(":ID_magazynu",QString::number(starforce->zaplecze[starforce->stoiska[i].barierkiSlupkowe.nrMagazynu].ID));
            }
            else if (j==3)
            {
                insupd.bindValue(":ilosc_podana",QString::number(starforce->stoiska[i].barierkiStalowe.iloscWJU));
                insupd.bindValue(":ilosc_przeliczona",QString::number(starforce->stoiska[i].barierkiStalowe.iloscWSzt));
                insupd.bindValue(":jedn_uzyt",QString::number(starforce->stoiska[i].barierkiStalowe.typJU));
                insupd.bindValue(":ID_magazynu",QString::number(starforce->zaplecze[starforce->stoiska[i].barierkiStalowe.nrMagazynu].ID));
            }
            else if (j==4)
            {
                insupd.bindValue(":ilosc_podana",QString::number(starforce->stoiska[i].krzesla.iloscWJU));
                insupd.bindValue(":ilosc_przeliczona",QString::number(starforce->stoiska[i].krzesla.iloscWSzt));
                insupd.bindValue(":jedn_uzyt",QString::number(starforce->stoiska[i].krzesla.typJU));
                insupd.bindValue(":ID_magazynu",QString::number(starforce->zaplecze[starforce->stoiska[i].krzesla.nrMagazynu].ID));
            }
            else if (j==5)
            {
                insupd.bindValue(":ilosc_podana",QString::number(starforce->stoiska[i].zapotrzebowaniePrad));
                insupd.bindValue(":ilosc_przeliczona","0");
                insupd.bindValue(":jedn_uzyt","0");
                insupd.bindValue(":ID_magazynu","-999");
            }
            else if (j==6)
            {
                insupd.bindValue(":ilosc_podana",QString::number(starforce->stoiska[i].doliczStolyPodGabloty));
                insupd.bindValue(":ilosc_przeliczona","0");
                insupd.bindValue(":jedn_uzyt","0");
                insupd.bindValue(":ID_magazynu","-999");
            }
            insupd.exec();

        }
    }

    QSqlQuery query1;
    QDate dataWizyty;
    dataWizyty.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
    query1.prepare("UPDATE wystawy SET ostatnia_wizyta=:aktualna_data");
    query1.bindValue(":aktualna_data",QString::number(dataWizyty.year())+"-"+QString::number(dataWizyty.month())+"-"+QString::number(dataWizyty.day()));
    query1.exec();

};

//-----zmiana trybu wyswietlania jednostki uzytkownika/szt.----------------------------------------------------------

void MainWindow::on_Przelicz_stateChanged(int arg1)
{
    TrybWyswietlania=arg1;
    UaktualnijTabeleStoisk();
}


//-----nowy plik----------------------------------------------------------------------------------------------

void MainWindow::on_actionNowy_triggered()
{
    NowyPlik();
}

void MainWindow::NowyPlik()
{
    delete starforce;//czyszczenie wystawy
    starforce=new wystawa;//tworzenie nowej wystawy
    ui->suma->setRowCount(0);//czyszczenie tabel
    ui->wystawcy->setRowCount(0);//czyszczenie tabel
    ui->EdytujStoiska->setEnabled(false);//wygaszanie przyciskow edycji
    ui->EdytujMagazyny->setEnabled(false);//wygaszanie przyciskow edycji
    ui->actionZapisz->setEnabled(false);//wygaszanie przycisku "Zapisz"
    this->setWindowTitle("Nowa wystawa");
}

//-----zamykanie programu-----------------------------------------------------------------------------------------

void MainWindow::on_actionZamknij_triggered()
{
    ZamknijProgram();
}

void MainWindow::ZamknijProgram()
{
    this->close();
}

//------otwieranie okna wlasciwosci wystawy--------------------------------------------------------------------------

void MainWindow::on_actionW_a_ciwo_ci_wystawy_triggered()
{
    edycjawystawy * wlasciwosciWystawy;
    wlasciwosciWystawy= new edycjawystawy(this);
    connect(this,SIGNAL(AktualnaWystawaDoEdycji(wystawa*)),wlasciwosciWystawy,SLOT(Ustaw(wystawa*)));
    emit AktualnaWystawaDoEdycji(starforce);
    wlasciwosciWystawy->exec();

};

//------otwieranie okna historii------------------------------------------------------------------------------------

void MainWindow::on_actionPobierz_dane_z_innej_wystawy_triggered()
{
    if (!db.isOpen()) PolaczZBazaDanych();
    if (db.isOpen())
    {
        historia * pobieranieZHistorii;
        pobieranieZHistorii = new historia(this);
        pobieranieZHistorii->ustawWystawy();
        connect(this,SIGNAL(AktualnaWystawaDoHistorii(wystawa*)),pobieranieZHistorii,SLOT(Ustaw(wystawa*)));
        connect(pobieranieZHistorii,SIGNAL(PobieranieZakonczone()),this,SLOT(UaktualnijStoiska()));
        connect(pobieranieZHistorii,SIGNAL(StworzoneStoisko(stoisko*)),this,SLOT(DodajStoisko(stoisko*)));
        connect(pobieranieZHistorii,SIGNAL(StworzonyMagazyn(magazyn*)),this,SLOT(DodajMagazyn(magazyn*)));
        connect(pobieranieZHistorii,SIGNAL(UsunWiersz()),this,SLOT(UsunWierszStoisk()));
        connect(pobieranieZHistorii,SIGNAL(UsunWierszMagazynow()),this,SLOT(UsunWierszMagazynow()));
        emit AktualnaWystawaDoHistorii(starforce);
        pobieranieZHistorii->exec();
    }
    else
    {
        InformacjaTestowa("Brak polaczenia z baza danych!");
    }
}



//-----TESTY-------------------------------------------------------------------------------------------------

void MainWindow::TestStoisk()
{
if (testySprawdzajStoiska==1)
{
    QMessageBox msgBox;
    QString informacja;
    for (int i=0;i<starforce->stoiska.count();i++)
    {
        informacja=informacja + " " + starforce->stoiska[i].nazwa + ",";
    }
    msgBox.setText(informacja);
    msgBox.exec();
};
};
void MainWindow::TestStarychMagazynow()
{
if (testySprawdzajStareMagazyny==1)
{
    QMessageBox msgBox;
    QString informacja;
    for (int i=0;i<starforce->stoiska.count();i++)
    {
        informacja=informacja + " " + starforce->stoiska[i].nazwa + ": " + QString::number(starforce->stoiska[i].stoly.nrStaregoMagazynu) + ", " +QString::number(starforce->stoiska[i].gabloty.nrStaregoMagazynu) + ", "+QString::number(starforce->stoiska[i].barierkiSlupkowe.nrStaregoMagazynu) + ", "+QString::number(starforce->stoiska[i].barierkiStalowe.nrStaregoMagazynu) + ", "+QString::number(starforce->stoiska[i].krzesla.nrStaregoMagazynu) + "::: " + '\n';
    }
    msgBox.setText(informacja);
    msgBox.exec();
};
};

void MainWindow::InformacjaTestowa(QString informacja)
{
    QMessageBox msgBox;
    msgBox.setText(informacja);
    msgBox.exec();
};






