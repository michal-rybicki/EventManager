#include "wystawy.h"

//-----sprzet_zapotrzebowanie---------------------------------------------------------------------------------

sprzet_zapotrzebowanie::sprzet_zapotrzebowanie(double ilosc0, int typ0)
{
    iloscWJU=ilosc0;
    ID=0;
    iloscWSzt=0;
    typJU=typ0;
    nrMagazynu=100000;
    nrStaregoMagazynu=100000;
}

void sprzet_zapotrzebowanie::ustaw(double ilosc0, int typ0)
{
    iloscWJU=ilosc0;
    typJU=typ0;
}

sprzet_zapotrzebowanie sprzet_zapotrzebowanie::operator+(sprzet_zapotrzebowanie sprzet1)
{
    sprzet_zapotrzebowanie wynik;
    wynik.iloscWJU=iloscWSzt+(sprzet1.iloscWSzt);
    wynik.iloscWSzt=wynik.iloscWJU;
    wynik.typJU=0;
    wynik.nrMagazynu=sprzet1.nrMagazynu;
    return wynik;
}

sprzet_zapotrzebowanie sprzet_zapotrzebowanie::operator-(sprzet_zapotrzebowanie sprzet1)
{
    sprzet_zapotrzebowanie wynik;
    wynik.iloscWJU=iloscWSzt-(sprzet1.iloscWSzt);
    wynik.iloscWSzt=wynik.iloscWJU;
    wynik.typJU=0;
    wynik.nrMagazynu=sprzet1.nrMagazynu;
    return wynik;
}

//-----stoisko---------------------------------------------------------------------------------

stoisko::stoisko(QString n, double ZS, int TZS, double ZG, int TZG, double ZBSL, int TZBSL, double ZBST, int TZBST, int ZK, int ZP)
{
    nazwa=n;
    ID=0;
    stoly.ustaw(ZS,TZS);
    stoly.typ=0;
    gabloty.ustaw(ZG,TZG);
    gabloty.typ=1;
    barierkiSlupkowe.ustaw(ZBSL,TZBSL);
    barierkiSlupkowe.typ=2;
    barierkiStalowe.ustaw(ZBST,TZBST);
    barierkiStalowe.typ=3;
    krzesla.ustaw(ZK,0);
    krzesla.typ=4;
    zapotrzebowaniePrad=ZP;
}

stoisko stoisko::operator+(stoisko stoisko1)
{
    stoisko wynik;
    wynik.stoly=stoly+stoisko1.stoly;
    wynik.gabloty=gabloty+stoisko1.gabloty;
    wynik.barierkiSlupkowe=barierkiSlupkowe+stoisko1.barierkiSlupkowe;
    wynik.barierkiStalowe=barierkiStalowe+stoisko1.barierkiStalowe;
    wynik.krzesla=krzesla+stoisko1.krzesla;
    if (zapotrzebowaniePrad==1 || stoisko1.zapotrzebowaniePrad==1)
        wynik.zapotrzebowaniePrad=1;
    else
        wynik.zapotrzebowaniePrad=0;
    return wynik;
};

void stoisko::zeruj()
{
    stoly.ustaw(0,0);
    gabloty.ustaw(0,0);
    barierkiSlupkowe.ustaw(0,0);
    barierkiStalowe.ustaw(0,0);
    krzesla.ustaw(0,0);
    zapotrzebowaniePrad=0;
    ID=0;
};

//-----jednostki----------------------------------------------------------------------------------------

//   0      1         2           3            4
//   stol   gablota   bar. slu.   bar. stal.   krzesla
//0  szt.   szt.      szt.        szt.         szt.
//1  m2     m2        mb          mb
//2  mb
//3  lawka szkolna
//4

void jednostka::ustawWymiary(double dlugosc1, double szerokosc1)
{
    if (dlugosc1>szerokosc1)
    {
        dlugosc=dlugosc1;
        szerokosc=szerokosc1;
    }
    else
    {
        dlugosc=szerokosc1;
        szerokosc=dlugosc1;
    }
    if (dlugosc<0.1) dlugosc=0.1;
    if (szerokosc<0.1) szerokosc=0.1;
};

//-----stol jednostkowy----------------------------------------------------------------------------------------

stol_jednostkowy::stol_jednostkowy(int ilosc0, double dlugosc0, double szerokosc0)
{
    ilosc=ilosc0;
    ustawWymiary(dlugosc0,szerokosc0);
};

void stol_jednostkowy::przelicz(sprzet_zapotrzebowanie& sprzet)
{
    if (sprzet.typJU==0)
        sprzet.iloscWSzt=static_cast<int>(ceil(sprzet.iloscWJU));
    else if (sprzet.typJU==1)
        sprzet.iloscWSzt=static_cast<int>(ceil((sprzet.iloscWJU)/(dlugosc*szerokosc)));
    else if (sprzet.typJU==2)
        sprzet.iloscWSzt=static_cast<int>(ceil((sprzet.iloscWJU)/dlugosc));
    else if (sprzet.typJU==3)
        sprzet.iloscWSzt=static_cast<int>(ceil((sprzet.iloscWJU)*(1.2*0.5)/(dlugosc*szerokosc)));
};

void stol_jednostkowy::doliczPodGabloty(sprzet_zapotrzebowanie& stoly, gablota_jednostkowa typGabloty, sprzet_zapotrzebowanie iloscGablot)
{
    int DodatkoweStoly=0;
    if (iloscGablot.iloscWJU>0)
    {
        int StolowNaSzerokosc=static_cast<int>(ceil(typGabloty.szerokosc/szerokosc));
        std::cout << StolowNaSzerokosc;
        int StolowNaDlugosc=static_cast<int>(ceil((typGabloty.dlugosc*iloscGablot.iloscWSzt)/dlugosc));

        DodatkoweStoly=StolowNaDlugosc*StolowNaSzerokosc;

    }
    stoly.iloscWSzt=stoly.iloscWSzt+DodatkoweStoly;
};

//------gablota jednostkowa-----------------------------------------------------------------------------------

gablota_jednostkowa::gablota_jednostkowa(int ilosc0, double dlugosc0, double szerokosc0)
{
    ilosc=ilosc0;
    ustawWymiary(dlugosc0,szerokosc0);
};

void gablota_jednostkowa::przelicz(sprzet_zapotrzebowanie& sprzet)
{
    if (sprzet.typJU==0)
        sprzet.iloscWSzt=static_cast<int>(ceil(sprzet.iloscWJU));
    else if (sprzet.typJU==1)
        sprzet.iloscWSzt=static_cast<int>(ceil((sprzet.iloscWJU)/(dlugosc*szerokosc)));
};

//-----barierka slupkowa jednostkowa----------------------------------------------------------------------------

barierka_slupkowa_jednostkowa::barierka_slupkowa_jednostkowa(int ilosc0, double dlugosc0)
{
    ilosc=ilosc0;
    ustawWymiary(dlugosc0);
};

void barierka_slupkowa_jednostkowa::przelicz(sprzet_zapotrzebowanie& sprzet)
{
    if (sprzet.typJU==0)
        sprzet.iloscWSzt=static_cast<int>(ceil(sprzet.iloscWJU));
    else if (sprzet.typJU==1)
        if (sprzet.iloscWJU>0)
            sprzet.iloscWSzt=static_cast<int>(ceil((sprzet.iloscWJU)/(dlugosc)+1));
        else
            sprzet.iloscWSzt=0;
};

//-----barierka stalowa jednostkowa----------------------------------------------------------------------------

barierka_stalowa_jednostkowa::barierka_stalowa_jednostkowa(int ilosc0, double dlugosc0)
{
    ilosc=ilosc0;
    ustawWymiary(dlugosc0);
};

void barierka_stalowa_jednostkowa::przelicz(sprzet_zapotrzebowanie& sprzet)
{
    if (sprzet.typJU==0)
        sprzet.iloscWSzt=static_cast<int>(ceil(sprzet.iloscWJU));
    else if (sprzet.typJU==1)
        sprzet.iloscWSzt=static_cast<int>(ceil((sprzet.iloscWJU)/(dlugosc)));
};

//-----krzeslo jednostkowe----------------------------------------------------------------------------

krzeslo_jednostkowe::krzeslo_jednostkowe(int ilosc0)
{
    ilosc=ilosc0;
};

void krzeslo_jednostkowe::przelicz(sprzet_zapotrzebowanie& sprzet)
{
        sprzet.iloscWSzt=static_cast<int>(ceil(sprzet.iloscWJU));
};

//-----magazyn--------------------------------------------------------------------------------------------

magazyn::magazyn(QString Nazwa0)
{
    ID=0;
    nazwa=Nazwa0;
    sprzetNaStanie[0]=new stol_jednostkowy;
    sprzetNaStanie[1]=new gablota_jednostkowa;
    sprzetNaStanie[2]=new barierka_slupkowa_jednostkowa;
    sprzetNaStanie[3]=new barierka_stalowa_jednostkowa;
    sprzetNaStanie[4]=new krzeslo_jednostkowe;
};


//-----wystawa----------------------------------------------------------------------------------------------

wystawa::wystawa(QString n0)
{
    nazwa=n0;
    ID=0;
    magazyn* brak=new magazyn("brak");
    for (int i=0; i<5; i++)
        brak->sprzetNaStanie[i]->ilosc=1;
    brak->ID=-999;
    this->dodaj_magazyn(brak);
    data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
    data=data.addDays(30);
};

void wystawa::przelicz_stoisko(stoisko& wystawca)
{
        /*
        if (ustawiono magazyn dla typu elementu)
        {
            odejmij ilosc elementow z magazynu ustawionego poprzenio jako aktulany;
            przelicz nowa ilosc elementow wg przelicznika nowego magazynu;
            dodaj elementy do aktualnego magazynu;
            ustaw nr poprzedniego magazynu na aktualny;
        }
        */
        if (wystawca.gabloty.nrMagazynu!=100000)
        {
            zaplecze[wystawca.gabloty.nrStaregoMagazynu].gablotyPobrane=zaplecze[wystawca.gabloty.nrStaregoMagazynu].gablotyPobrane-wystawca.gabloty;
            zaplecze[wystawca.gabloty.nrMagazynu].sprzetNaStanie[1]->przelicz(wystawca.gabloty);
            zaplecze[wystawca.gabloty.nrMagazynu].gablotyPobrane=zaplecze[wystawca.gabloty.nrMagazynu].gablotyPobrane+wystawca.gabloty;
            wystawca.gabloty.nrStaregoMagazynu=wystawca.gabloty.nrMagazynu;
        };
        if (wystawca.stoly.nrMagazynu!=100000)
        {
            zaplecze[wystawca.stoly.nrStaregoMagazynu].stolyPobrane=zaplecze[wystawca.stoly.nrStaregoMagazynu].stolyPobrane-wystawca.stoly;
            zaplecze[wystawca.stoly.nrMagazynu].sprzetNaStanie[0]->przelicz(wystawca.stoly);
            //doliczanie stolow pod gabloty
            if (wystawca.doliczStolyPodGabloty==1)
            {
                stol_jednostkowy stolDoObliczen(0,zaplecze[wystawca.stoly.nrMagazynu].sprzetNaStanie[0]->dlugosc,zaplecze[wystawca.stoly.nrMagazynu].sprzetNaStanie[0]->szerokosc);
                gablota_jednostkowa gablotaDoObliczen(0,zaplecze[wystawca.stoly.nrMagazynu].sprzetNaStanie[1]->dlugosc,zaplecze[wystawca.stoly.nrMagazynu].sprzetNaStanie[1]->szerokosc);
                stolDoObliczen.doliczPodGabloty(wystawca.stoly,gablotaDoObliczen,wystawca.gabloty);
            };
            zaplecze[wystawca.stoly.nrMagazynu].stolyPobrane=zaplecze[wystawca.stoly.nrMagazynu].stolyPobrane+wystawca.stoly;
            wystawca.stoly.nrStaregoMagazynu=wystawca.stoly.nrMagazynu;
        };
        if (wystawca.barierkiSlupkowe.nrMagazynu!=100000)
        {
            zaplecze[wystawca.barierkiSlupkowe.nrStaregoMagazynu].barierkiSlupkowePobrane=zaplecze[wystawca.barierkiSlupkowe.nrStaregoMagazynu].barierkiSlupkowePobrane-wystawca.barierkiSlupkowe;
            zaplecze[wystawca.barierkiSlupkowe.nrMagazynu].sprzetNaStanie[2]->przelicz(wystawca.barierkiSlupkowe);
            zaplecze[wystawca.barierkiSlupkowe.nrMagazynu].barierkiSlupkowePobrane=zaplecze[wystawca.barierkiSlupkowe.nrMagazynu].barierkiSlupkowePobrane+wystawca.barierkiSlupkowe;
            wystawca.barierkiSlupkowe.nrStaregoMagazynu=wystawca.barierkiSlupkowe.nrMagazynu;
        };
        if (wystawca.barierkiStalowe.nrMagazynu!=100000)
        {
            zaplecze[wystawca.barierkiStalowe.nrStaregoMagazynu].barierkiStalowePobrane=zaplecze[wystawca.barierkiStalowe.nrStaregoMagazynu].barierkiStalowePobrane-wystawca.barierkiStalowe;
            zaplecze[wystawca.barierkiStalowe.nrMagazynu].sprzetNaStanie[3]->przelicz(wystawca.barierkiStalowe);
            zaplecze[wystawca.barierkiStalowe.nrMagazynu].barierkiStalowePobrane=zaplecze[wystawca.barierkiStalowe.nrMagazynu].barierkiStalowePobrane+wystawca.barierkiStalowe;
            wystawca.barierkiStalowe.nrStaregoMagazynu=wystawca.barierkiStalowe.nrMagazynu;
        };
        if (wystawca.krzesla.nrMagazynu!=100000)
        {
            zaplecze[wystawca.krzesla.nrStaregoMagazynu].krzeslaPobrane=zaplecze[wystawca.krzesla.nrStaregoMagazynu].krzeslaPobrane-wystawca.krzesla;
            zaplecze[wystawca.krzesla.nrMagazynu].sprzetNaStanie[4]->przelicz(wystawca.krzesla);
            zaplecze[wystawca.krzesla.nrMagazynu].krzeslaPobrane=zaplecze[wystawca.krzesla.nrMagazynu].krzeslaPobrane+wystawca.krzesla;
            wystawca.krzesla.nrStaregoMagazynu=wystawca.krzesla.nrMagazynu;
        };

};

void wystawa::dodaj_stoisko(stoisko * nowe_stoisko)
{
        stoiska.append(*nowe_stoisko);
        przelicz_stoisko(stoiska.last());
};

void wystawa::usun_stoisko(int nr_stoiska)
{

    //odejmowanie zapotrzebowania stoiska z magazynu
    if (stoiska[nr_stoiska].stoly.nrMagazynu!=100000)
    {
        zaplecze[stoiska[nr_stoiska].stoly.nrStaregoMagazynu].stolyPobrane=zaplecze[stoiska[nr_stoiska].stoly.nrStaregoMagazynu].stolyPobrane-stoiska[nr_stoiska].stoly;
    };
    if (stoiska[nr_stoiska].gabloty.nrMagazynu!=100000)
    {
        zaplecze[stoiska[nr_stoiska].gabloty.nrStaregoMagazynu].gablotyPobrane=zaplecze[stoiska[nr_stoiska].gabloty.nrStaregoMagazynu].gablotyPobrane-stoiska[nr_stoiska].gabloty;
    };
    if (stoiska[nr_stoiska].barierkiSlupkowe.nrMagazynu!=100000)
    {
        zaplecze[stoiska[nr_stoiska].barierkiSlupkowe.nrStaregoMagazynu].barierkiSlupkowePobrane=zaplecze[stoiska[nr_stoiska].barierkiSlupkowe.nrStaregoMagazynu].barierkiSlupkowePobrane-stoiska[nr_stoiska].barierkiSlupkowe;
    };
    if (stoiska[nr_stoiska].barierkiStalowe.nrMagazynu!=100000)
    {
        zaplecze[stoiska[nr_stoiska].barierkiStalowe.nrStaregoMagazynu].barierkiStalowePobrane=zaplecze[stoiska[nr_stoiska].barierkiStalowe.nrStaregoMagazynu].barierkiStalowePobrane-stoiska[nr_stoiska].barierkiStalowe;
    };
    if (stoiska[nr_stoiska].krzesla.nrMagazynu!=100000)
    {
        zaplecze[stoiska[nr_stoiska].krzesla.nrStaregoMagazynu].krzeslaPobrane=zaplecze[stoiska[nr_stoiska].krzesla.nrStaregoMagazynu].krzeslaPobrane-stoiska[nr_stoiska].krzesla;
    };

    //usuwanie stoiska z listy
    stoiska.removeAt(nr_stoiska);
};

void wystawa::dodaj_magazyn(magazyn * nowy_magazyn)
{
    /*int maxID=0;
    for (int i=0; i<zaplecze.count(); i++)
    {
        if (zaplecze[i].ID>maxID)
        {
            maxID=zaplecze[i].ID;
        }
    }*/
    zaplecze.append(*nowy_magazyn);
    //zaplecze[zaplecze.count()-1].ID=maxID+1;
};

void wystawa::usun_magazyn(int nr_magazynu)
{

    //aktualizacja nr magazynow o nr wiekszych niz magazyn usuwany
    for (int i=0;i<stoiska.count();i++)
    {
        if (stoiska[i].stoly.nrMagazynu==nr_magazynu) stoiska[i].stoly.nrMagazynu=100000;
        if (stoiska[i].gabloty.nrMagazynu==nr_magazynu) stoiska[i].gabloty.nrMagazynu=100000;
        if (stoiska[i].barierkiSlupkowe.nrMagazynu==nr_magazynu) stoiska[i].barierkiSlupkowe.nrMagazynu=100000;
        if (stoiska[i].barierkiStalowe.nrMagazynu==nr_magazynu) stoiska[i].barierkiStalowe.nrMagazynu=100000;
        if (stoiska[i].krzesla.nrMagazynu==nr_magazynu) stoiska[i].krzesla.nrMagazynu=100000;
        if (stoiska[i].stoly.nrStaregoMagazynu==nr_magazynu) stoiska[i].stoly.nrStaregoMagazynu=100000;
        if (stoiska[i].gabloty.nrStaregoMagazynu==nr_magazynu) stoiska[i].gabloty.nrStaregoMagazynu=100000;
        if (stoiska[i].barierkiSlupkowe.nrStaregoMagazynu==nr_magazynu) stoiska[i].barierkiSlupkowe.nrStaregoMagazynu=100000;
        if (stoiska[i].barierkiStalowe.nrStaregoMagazynu==nr_magazynu) stoiska[i].barierkiStalowe.nrStaregoMagazynu=100000;
        if (stoiska[i].krzesla.nrStaregoMagazynu==nr_magazynu) stoiska[i].krzesla.nrStaregoMagazynu=100000;

        if (stoiska[i].stoly.nrMagazynu>nr_magazynu && stoiska[i].stoly.nrMagazynu!=100000) stoiska[i].stoly.nrMagazynu--;
        if (stoiska[i].gabloty.nrMagazynu>nr_magazynu && stoiska[i].gabloty.nrMagazynu!=100000) stoiska[i].gabloty.nrMagazynu--;
        if (stoiska[i].barierkiSlupkowe.nrMagazynu>nr_magazynu && stoiska[i].barierkiSlupkowe.nrMagazynu!=100000) stoiska[i].barierkiSlupkowe.nrMagazynu--;
        if (stoiska[i].barierkiStalowe.nrMagazynu>nr_magazynu && stoiska[i].barierkiStalowe.nrMagazynu!=100000) stoiska[i].barierkiStalowe.nrMagazynu--;
        if (stoiska[i].krzesla.nrMagazynu>nr_magazynu && stoiska[i].krzesla.nrMagazynu!=100000) stoiska[i].krzesla.nrMagazynu--;
        if (stoiska[i].stoly.nrStaregoMagazynu>nr_magazynu && stoiska[i].stoly.nrStaregoMagazynu!=100000) stoiska[i].stoly.nrStaregoMagazynu--;
        if (stoiska[i].gabloty.nrStaregoMagazynu>nr_magazynu && stoiska[i].gabloty.nrStaregoMagazynu!=100000) stoiska[i].gabloty.nrStaregoMagazynu--;
        if (stoiska[i].barierkiSlupkowe.nrStaregoMagazynu>nr_magazynu && stoiska[i].barierkiSlupkowe.nrStaregoMagazynu!=100000) stoiska[i].barierkiSlupkowe.nrStaregoMagazynu--;
        if (stoiska[i].barierkiStalowe.nrStaregoMagazynu>nr_magazynu && stoiska[i].barierkiStalowe.nrStaregoMagazynu!=100000) stoiska[i].barierkiStalowe.nrStaregoMagazynu--;
        if (stoiska[i].krzesla.nrStaregoMagazynu>nr_magazynu && stoiska[i].krzesla.nrStaregoMagazynu!=100000) stoiska[i].krzesla.nrStaregoMagazynu--;
    }

    //usuwanie magazynu z listy
    zaplecze.removeAt(nr_magazynu);
};

void wystawa::przelicz_wszystko()
{
    for (int i=0; i<stoiska.count(); i++)
    {
        przelicz_stoisko(stoiska[i]);
    };
};

int wystawa::nrMagazynu(int ID_magazynu)
{
    for (int i=0; i<zaplecze.length(); i++)
    {
        if (zaplecze[i].ID==ID_magazynu)
            return i;
    }
    return 0;
};
