#include "historia.h"
#include "ui_historia.h"

historia::historia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::historia)
{
    ui->setupUi(this);

    ui->wystawcy->setColumnCount(8);

    this->setFixedSize(930,630);//ustawianie stalego rozmiaru okna
    this->setWindowTitle("Wybierz dane do skopiowania...");
    TrybWyswietlaniaMagazynow=2;
    TrybWyswietlania=0;

    //zmienne odpowadajace za szerokosc kolumn
    int SzerokoscNazwy=300;
    int SzerokoscInnych=75;
    int SzerokoscBarier=110;
    int SzerokoscPierwszej=10;

    //ustawianie szerokosci kolumn w tabeli stoisk
    ui->wystawcy->setColumnWidth(0,SzerokoscPierwszej);
    ui->wystawcy->setColumnWidth(1,SzerokoscNazwy);
    for (int i=2; i<9; i++)
    ui->wystawcy->setColumnWidth(i,SzerokoscInnych);
    for (int i=4; i<6; i++)
    ui->wystawcy->setColumnWidth(i,SzerokoscBarier);

    //ustawianie szerokosci kolumn w tabeli magazynow
    ui->suma->setColumnWidth(0,SzerokoscPierwszej);
    ui->suma->setColumnWidth(1,SzerokoscNazwy);
    for (int i=2; i<8; i++)
    ui->suma->setColumnWidth(i,SzerokoscInnych);
    for (int i=4; i<6; i++)
    ui->suma->setColumnWidth(i,SzerokoscBarier);

    starforce=new wystawa("Nowa wystawa");
}

historia::~historia()
{
    delete ui;
}

void historia::Ustaw(wystawa* starforce0)
{
    aktualny_starforce=starforce0;

};
//-----ustawianie comboboxa wystaw----------------------------------------------------------------------------

void historia::ustawWystawy()
{
    QSqlQuery Query;
    QList<QString> lista_wystaw;
    Query.prepare("SELECT distinct wystawy_ID FROM zapotrzebowanie_archiwum");
    Query.exec();
    //int i=0;
    while(Query.next())
    {
        QSqlQuery Query2;
        Query2.prepare("SELECT nazwa FROM wystawy where ID=:ID");
        Query2.bindValue(":ID",Query.value(0).toString());
        //ID[i]=Query.value(0).toInt();
        Query2.exec();
        Query2.first();
        QString wpis=Query2.value(0).toString();
        lista_wystaw.append(wpis);
        //ui->wystawa->addItem(wpis);
        //nazwa[i]=wpis;
        //i++;

    }
    ui->wystawy->addItems(lista_wystaw);

}

//-----wczytywanie archiwalnej wystawy--------------------------------------------------------------------------------

void historia::WczytajZBazy(int ID_wystawy, QString data)
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
    //starforce->data=data;


    //wczytywanie magazynow
    Query.prepare("select * from zaplecze_archiwum where wystawy_ID=:ID_wystawy and edycja=:data");
    Query.bindValue(":ID_wystawy",ID_wystawy);
    Query.bindValue(":data",data);
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
        wpis=Query.value(3).toString();
        /*03*/ N->sprzetNaStanie[0]->ilosc=wpis.toInt();
        wpis=Query.value(9).toString();
        /*04*/N->sprzetNaStanie[0]->dlugosc=wpis.toDouble();
        wpis=Query.value(10).toString();
        /*05*/N->sprzetNaStanie[0]->szerokosc=wpis.toDouble();
        wpis=Query.value(4).toString();
        /*06*/N->sprzetNaStanie[1]->ilosc=wpis.toInt();
        wpis=Query.value(11).toString();
        /*07*/N->sprzetNaStanie[1]->dlugosc=wpis.toDouble();
        wpis=Query.value(10).toString();
        /*08*/N->sprzetNaStanie[1]->szerokosc=wpis.toDouble();
        wpis=Query.value(6).toString();
        /*09*/N->sprzetNaStanie[2]->ilosc=wpis.toInt();
        wpis=Query.value(13).toString();
        /*10*/N->sprzetNaStanie[2]->dlugosc=wpis.toDouble();
        wpis=Query.value(5).toString();
        /*11*/N->sprzetNaStanie[3]->ilosc=wpis.toInt();
        wpis=Query.value(12).toString();
        /*12*/N->sprzetNaStanie[3]->dlugosc=wpis.toDouble();
        wpis=Query.value(7).toString();
        /*13*/N->sprzetNaStanie[4]->ilosc=wpis.toInt();
        starforce->zaplecze.append(*N);
        DopiszMagazyn(N);
    }

    //wczytywanie stoisk
    Query.prepare("select distinct wystawcy_ID from zapotrzebowanie_archiwum where wystawy_ID=:ID_wystawy and edycja=:data");
    Query.bindValue(":ID_wystawy",ID_wystawy);
    Query.bindValue(":data",data);
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
        Query.prepare("select * from zapotrzebowanie_archiwum where wystawy_ID=:ID_wystawy and wystawcy_ID=:ID_wystawcy and edycja=:data");
        Query.bindValue(":data",data);
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
                wpis=Query.value(4).toString();
                /*03*/N->stoly.iloscWJU=((wpis).toDouble());
                wpis=Query.value(6).toString();
                /*04*/N->stoly.typJU=(wpis).toInt();
                int ID_magazynu=Query.value(7).toInt();
                /*05*/N->stoly.nrMagazynu=starforce->nrMagazynu(ID_magazynu);
                N->stoly.nrStaregoMagazynu=N->stoly.nrMagazynu;
            }
            else if (Query.value(2).toInt()==1)
            {
                wpis=Query.value(4).toString();
                /*06*/N->gabloty.iloscWJU=((wpis).toDouble());
                wpis=Query.value(6).toString();
                /*07*/N->gabloty.typJU=(wpis).toInt();
                int ID_magazynu=Query.value(7).toInt();
                /*08*/N->gabloty.nrMagazynu=starforce->nrMagazynu(ID_magazynu);
                N->gabloty.nrStaregoMagazynu=N->gabloty.nrMagazynu;
            }
            else if (Query.value(2).toInt()==2)
            {
                wpis=Query.value(4).toString();
                /*09*/N->barierkiSlupkowe.iloscWJU=((wpis).toDouble());
                wpis=Query.value(6).toString();
                /*10*/N->barierkiSlupkowe.typJU=(wpis).toInt();
                int ID_magazynu=Query.value(7).toInt();
                /*11*/N->barierkiSlupkowe.nrMagazynu=starforce->nrMagazynu(ID_magazynu);
                N->barierkiSlupkowe.nrStaregoMagazynu=N->barierkiSlupkowe.nrMagazynu;
            }
            else if (Query.value(2).toInt()==3)
            {
                wpis=Query.value(4).toString();
                /*12*/N->barierkiStalowe.iloscWJU=((wpis).toDouble());
                wpis=Query.value(6).toString();
                /*13*/N->barierkiStalowe.typJU=(wpis).toInt();
                int ID_magazynu=Query.value(7).toInt();
                /*14*/N->barierkiStalowe.nrMagazynu=starforce->nrMagazynu(ID_magazynu);
                N->barierkiStalowe.nrStaregoMagazynu=N->barierkiStalowe.nrMagazynu;
            }
            else if (Query.value(2).toInt()==4)
            {
                wpis=Query.value(4).toString();
                /*15*/N->krzesla.iloscWJU=((wpis).toDouble());
                int ID_magazynu=Query.value(7).toInt();
                /*16*/N->krzesla.nrMagazynu=starforce->nrMagazynu(ID_magazynu);
                N->krzesla.nrStaregoMagazynu=N->krzesla.nrMagazynu;
            }
            else if (Query.value(2).toInt()==5)
            {
                wpis=Query.value(4).toString();
                /*17*/N->zapotrzebowaniePrad=((wpis).toInt());
            }
            else if (Query.value(2).toInt()==6)
            {
                wpis=Query.value(4).toString();
                /*18*/N->doliczStolyPodGabloty=((wpis).toInt());
            }
        }
        starforce->stoiska.append(*N);
        DopiszStoisko(N);
    }


};

//-----dodawanie magazynu-------------------------------------------------------------------------------------

void historia::DodajMagazyn(magazyn * N)
{
    starforce->dodaj_magazyn(N);
    DopiszMagazyn(N);
};

void historia::DopiszMagazyn(magazyn * N)
{
    ui->suma->setRowCount(ui->suma->rowCount()+1);
    QTableWidgetItem *NowaNazwa;
    NowaNazwa=new QTableWidgetItem(N->nazwa);
    NowaNazwa->setFlags(NowaNazwa->flags() ^ Qt::ItemIsEditable);
    ui->suma->setItem(((ui->suma->rowCount())-1),1,NowaNazwa);
    QTableWidgetItem *haczyk = new QTableWidgetItem();
    haczyk->setCheckState(Qt::Unchecked);
    ui->suma->setItem(((ui->suma->rowCount())-1),0,haczyk);
    UaktualnijTabeleMagazynow();

};

//-----dodawanie stoiska-------------------------------------------------------------------------

void historia::DodajStoisko(stoisko * N)
{
    starforce->dodaj_stoisko(N);
    DopiszStoisko(N);
    UaktualnijTabeleMagazynow();
    //if (!ui->EdytujStoiska->isEnabled()) ui->EdytujStoiska->setEnabled(true);
}

//-----dopisywanie stoiska do tabeli-------------------------------------------------------------------------

void historia::DopiszStoisko(stoisko * N)
{
    ui->wystawcy->setRowCount(ui->wystawcy->rowCount()+1);
    QTableWidgetItem *NowaNazwa;
    NowaNazwa=new QTableWidgetItem(N->nazwa);
    NowaNazwa->setFlags(NowaNazwa->flags() ^ Qt::ItemIsEditable);
    ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),1,NowaNazwa);
    QTableWidgetItem *haczyk = new QTableWidgetItem();
    haczyk->setCheckState(Qt::Unchecked);
    ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),0,haczyk);
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

        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),2,NoweStoly);

        QTableWidgetItem *NoweGabloty;
        if (N->gabloty.typJU==0)
                NoweGabloty=new QTableWidgetItem((QString::number(N->gabloty.iloscWJU))+" szt.");
        else if (N->gabloty.typJU==1)
            NoweGabloty=new QTableWidgetItem((QString::number(N->gabloty.iloscWJU))+" m2");
        NoweGabloty->setFlags(NoweGabloty->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),3,NoweGabloty);

        QTableWidgetItem *NoweBarierkiSlupkowe;
        if (N->barierkiSlupkowe.typJU==0)
            NoweBarierkiSlupkowe=new QTableWidgetItem((QString::number(N->barierkiSlupkowe.iloscWJU))+" szt.");
        else if (N->barierkiSlupkowe.typJU==1)
            NoweBarierkiSlupkowe=new QTableWidgetItem((QString::number(N->barierkiSlupkowe.iloscWJU))+" mb");
        NoweBarierkiSlupkowe->setFlags(NoweBarierkiSlupkowe->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),4,NoweBarierkiSlupkowe);

        QTableWidgetItem *NoweBarierkiStalowe;
        if (N->barierkiStalowe.typJU==0)
            NoweBarierkiStalowe=new QTableWidgetItem((QString::number(N->barierkiStalowe.iloscWJU))+" szt.");
        else if (N->barierkiStalowe.typJU==1)
            NoweBarierkiStalowe=new QTableWidgetItem((QString::number(N->barierkiStalowe.iloscWJU))+" mb");
        NoweBarierkiStalowe->setFlags(NoweBarierkiStalowe->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),5,NoweBarierkiStalowe);

        QTableWidgetItem *NoweKrzesla;
        NoweKrzesla=new QTableWidgetItem((QString::number(N->krzesla.iloscWJU))+" szt.");
        NoweKrzesla->setFlags(NoweKrzesla->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),6,NoweKrzesla);

        QTableWidgetItem *NoweZapotrzebowaniePrad;
        if (N->zapotrzebowaniePrad==0)
            NoweZapotrzebowaniePrad=new QTableWidgetItem("NIE");
        else if (N->zapotrzebowaniePrad==1)
        NoweZapotrzebowaniePrad=new QTableWidgetItem("TAK");
        NoweZapotrzebowaniePrad->setFlags(NoweZapotrzebowaniePrad->flags() ^ Qt::ItemIsEditable);
        ui->wystawcy->setItem(((ui->wystawcy->rowCount())-1),7,NoweZapotrzebowaniePrad);
    }

}


//-----uaktualnianie sumy w tabeli-----------------------------------------------------------------------------

void historia::UaktualnijTabeleMagazynow()
{
    int s=0;
    for (int i=1; i<starforce->zaplecze.count();i++)
    {
        s=0;
        //if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            QTableWidgetItem *NoweStoly;
            NoweStoly=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[0]->ilosc))+" szt.");
            NoweStoly->setFlags(NoweStoly->flags() ^ Qt::ItemIsEditable);
            ui->suma->setItem(i-1,2,NoweStoly);
        };
        s++;
        //if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            QTableWidgetItem *NoweGabloty;
            NoweGabloty=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[1]->ilosc))+" szt.");
            NoweGabloty->setFlags(NoweGabloty->flags() ^ Qt::ItemIsEditable);
            ui->suma->setItem(i-1,3,NoweGabloty);
        };
        s++;
        //if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            QTableWidgetItem *NoweBarierkiSlupkowe;
            NoweBarierkiSlupkowe=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[2]->ilosc))+" szt.");
            NoweBarierkiSlupkowe->setFlags(NoweBarierkiSlupkowe->flags() ^ Qt::ItemIsEditable);
            ui->suma->setItem(i-1,4,NoweBarierkiSlupkowe);
        };
        s++;
        //if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            QTableWidgetItem *NoweBarierkiStalowe;
            NoweBarierkiStalowe=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[3]->ilosc))+" szt.");
            NoweBarierkiStalowe->setFlags(NoweBarierkiStalowe->flags() ^ Qt::ItemIsEditable);
            ui->suma->setItem(i-1,5,NoweBarierkiStalowe);
        };
        s++;
        //if (starforce->zaplecze[i].sprzetNaStanie[s]->ilosc!=0)
        {
            QTableWidgetItem *NoweKrzesla;
            NoweKrzesla=new QTableWidgetItem((QString::number(starforce->zaplecze[i].sprzetNaStanie[4]->ilosc))+" szt.");
            NoweKrzesla->setFlags(NoweKrzesla->flags() ^ Qt::ItemIsEditable);
            ui->suma->setItem(i-1,6,NoweKrzesla);
        };
    };


};

//-----wypelnianie comboboxa edycji------------------------------------------------------------------------------------------------------------------------------------

void historia::on_wystawy_currentIndexChanged(int index)
{
    uaktualnijComboboxEdycji();

}

void historia::uaktualnijComboboxEdycji()
{
    ui->edycja->clear();
    QSqlQuery Query;
    Query.prepare("SELECT ID from wystawy where nazwa like :nazwa");
    Query.bindValue(":nazwa",ui->wystawy->currentText());
    Query.exec();
    Query.first();
    ID_wystawy=Query.value(0).toInt();
    //ID_wystawy=ID[ui->wystawa->currentIndex()];
    Query.prepare("SELECT distinct edycja FROM zaplecze_archiwum WHERE wystawy_ID=:ID");
    Query.bindValue(":ID",QString::number(ID_wystawy));
    Query.exec();
    while(Query.next())
    {
        QString wpis=Query.value(0).toString();
        ui->edycja->addItem(wpis);

    }

}

//------pobieranie danych do tabelek i obiektu wystawy--------------------------------------------------------------------------------

void historia::on_edycja_currentIndexChanged(int index)
{
    ui->suma->setRowCount(0);//czyszczenie tabel
    ui->wystawcy->setRowCount(0);//czyszczenie tabel
    data=ui->edycja->currentText();
    WczytajZBazy(ID_wystawy,data);
    UaktualnijTabeleMagazynow();
}
//------do testow--------------------------------------------------------------------------------------------------

void historia::InformacjaTestowa(QString informacja)
{
    QMessageBox msgBox;
    msgBox.setText(informacja);
    msgBox.exec();
};

//-------kopiowanie wybranych elementow do podstawowej wystawy

void historia::on_historia_accepted()
{
    for (int i=0; i<ui->wystawcy->rowCount(); i++)
    {
        if (ui->wystawcy->item(i,0)->checkState()==Qt::Checked)
        {
            stoisko * N;
            N=new stoisko();
            N->ID=starforce->stoiska[i].ID;
            N->nazwa=starforce->stoiska[i].nazwa;
            N->zapotrzebowaniePrad=starforce->stoiska[i].zapotrzebowaniePrad;
            N->doliczStolyPodGabloty=starforce->stoiska[i].doliczStolyPodGabloty;

            //stoly
            N->stoly.typJU=starforce->stoiska[i].stoly.typJU;
            N->stoly.iloscWJU=starforce->stoiska[i].stoly.iloscWJU;
            N->stoly.nrMagazynu=aktualny_starforce->nrMagazynu(-999);
            N->stoly.nrStaregoMagazynu=aktualny_starforce->nrMagazynu(-999);
            N->stoly.iloscWSzt=0;
            //gabloty
            N->gabloty.typJU=starforce->stoiska[i].gabloty.typJU;
            N->gabloty.iloscWJU=starforce->stoiska[i].gabloty.iloscWJU;
            N->gabloty.nrMagazynu=aktualny_starforce->nrMagazynu(-999);
            N->gabloty.nrStaregoMagazynu=aktualny_starforce->nrMagazynu(-999);
            N->gabloty.iloscWSzt=0;
            //barierki slupkowe
            N->barierkiSlupkowe.typJU=starforce->stoiska[i].barierkiSlupkowe.typJU;
            N->barierkiSlupkowe.iloscWJU=starforce->stoiska[i].barierkiSlupkowe.iloscWJU;
            N->barierkiSlupkowe.nrMagazynu=aktualny_starforce->nrMagazynu(-999);
            N->barierkiSlupkowe.nrStaregoMagazynu=aktualny_starforce->nrMagazynu(-999);
            N->barierkiSlupkowe.iloscWSzt=0;
            //barierki stalowe
            N->barierkiStalowe.typJU=starforce->stoiska[i].barierkiStalowe.typJU;
            N->barierkiStalowe.iloscWJU=starforce->stoiska[i].barierkiStalowe.iloscWJU;
            N->barierkiStalowe.nrMagazynu=aktualny_starforce->nrMagazynu(-999);
            N->barierkiStalowe.nrStaregoMagazynu=aktualny_starforce->nrMagazynu(-999);
            N->barierkiStalowe.iloscWSzt=0;
            //krzesla
            N->krzesla.typJU=starforce->stoiska[i].krzesla.typJU;
            N->krzesla.iloscWJU=starforce->stoiska[i].krzesla.iloscWJU;
            N->krzesla.nrMagazynu=aktualny_starforce->nrMagazynu(-999);
            N->krzesla.nrStaregoMagazynu=aktualny_starforce->nrMagazynu(-999);
            N->krzesla.iloscWSzt=0;

            for (int j=0;j<aktualny_starforce->stoiska.count();j++)
            {
                if (aktualny_starforce->stoiska[j].nazwa==N->nazwa)
                {
                    aktualny_starforce->usun_stoisko(j);
                    emit UsunWiersz();
                }
            }

            emit StworzoneStoisko(N);
            //aktualny_starforce->dodaj_stoisko(N);

        }
    }
    for (int i=0; i<ui->suma->rowCount(); i++)
    {
        if (ui->suma->item(i,0)->checkState()==Qt::Checked)
        {
            magazyn * N;
            double wymiar1=0,wymiar2=0;
            N = new magazyn;
            N->nazwa=starforce->zaplecze[i+1].nazwa;
            N->sprzetNaStanie[0]->ilosc=starforce->zaplecze[i+1].sprzetNaStanie[0]->ilosc;
            wymiar1=starforce->zaplecze[i+1].sprzetNaStanie[0]->dlugosc;
            wymiar2=starforce->zaplecze[i+1].sprzetNaStanie[0]->szerokosc;
            N->sprzetNaStanie[0]->ustawWymiary(wymiar1,wymiar2);
            N->sprzetNaStanie[1]->ilosc=starforce->zaplecze[i+1].sprzetNaStanie[1]->ilosc;
            wymiar1=starforce->zaplecze[i+1].sprzetNaStanie[1]->dlugosc;
            wymiar2=starforce->zaplecze[i+1].sprzetNaStanie[1]->szerokosc;
            N->sprzetNaStanie[1]->ustawWymiary(wymiar1,wymiar2);
            N->sprzetNaStanie[2]->ilosc=starforce->zaplecze[i+1].sprzetNaStanie[2]->ilosc;
            N->sprzetNaStanie[2]->ustawWymiary(starforce->zaplecze[i+1].sprzetNaStanie[2]->dlugosc,0);
            N->sprzetNaStanie[3]->ilosc=starforce->zaplecze[i+1].sprzetNaStanie[3]->ilosc;
            N->sprzetNaStanie[3]->ustawWymiary(starforce->zaplecze[i+1].sprzetNaStanie[3]->dlugosc,0);
            N->sprzetNaStanie[4]->ilosc=starforce->zaplecze[i+1].sprzetNaStanie[4]->ilosc;

            for (int j=1;j<aktualny_starforce->zaplecze.count(); j++)
            {
                if (aktualny_starforce->zaplecze[j].nazwa==N->nazwa)
                {
                    aktualny_starforce->usun_magazyn(j);
                    emit UsunWierszMagazynow();

                }
            }
            emit StworzonyMagazyn(N);
        }
    }
    emit PobieranieZakonczone();
}
