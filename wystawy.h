#ifndef wystawy_h
#define wystawy_h

#include <conio.h> //do sterowania klawiszami
#include <iomanip> //do wyswietlania z zaokrogleniem
#include <fstream> //do obslugi plikow
#include <iostream> // do strumienia wejscia/wyjscia
#include <windows.h> //do koloru
#include <stdlib.h> //do koloru.
#include <cctype> //do analizy znakow (funkcja tolower)
#include <cstdlib> //do losowania
#include <ctime> //do losowania
#include <cmath> //do obliczen czasu podrozy
#include <vector> //do przechowywania zgloszen
#include <QString>
#include <QList>
#include <QDate>


//-----sprzet_zapotrzebowanie---------------------------------------------------------------------------------

class sprzet_zapotrzebowanie
{
public:
    //Pola:
    int ID; //ID z bazy danych
    double iloscWJU; //w jednostakch uzytkowanika
    int iloscWSzt; //w sztukach
    int typJU; //typ jednostki uzytkownika
    int typ; //typ sprzetu
    int nrMagazynu; //nr magazynu z ktorego ma byc pobierany sprzet
    int nrStaregoMagazynu; //nr magazynu z ktorego byl pobierany sprzet przed edycja
    //   0      1         2           3            4
    //   stol   gablota   bar. slu.   bar. stal.   krzesla
    //0  szt.   szt.      szt.        szt.         szt.
    //1  m2     m2        mb          mb
    //2  mb
    //3  lawka szkolna
    //4

    //Metody:
    sprzet_zapotrzebowanie(double ilosc0=0, int typ0=0);
    void ustaw(double ilosc0, int typ0);
    sprzet_zapotrzebowanie operator+(sprzet_zapotrzebowanie sprzet1);
    sprzet_zapotrzebowanie operator-(sprzet_zapotrzebowanie sprzet1);

};

//-----jednostka-----------------------------------------------------------------------------------

class jednostka
{
public:
    int ilosc;
    double dlugosc;
    double szerokosc;
    void ustawWymiary(double dlugosc1=1, double szerokosc1=0);
    virtual void przelicz(sprzet_zapotrzebowanie& sprzet)=0;
};

class gablota_jednostkowa:public jednostka
{
public:
    gablota_jednostkowa(int ilosc0=0, double dlugosc0=1.65, double szerokosc0=0.65);
    void przelicz(sprzet_zapotrzebowanie& sprzet);
};

class stol_jednostkowy:public jednostka
{
public:
    stol_jednostkowy(int ilosc0=0, double dlugosc0=1.2, double szerokosc0=0.5);
    void przelicz(sprzet_zapotrzebowanie& sprzet);
    void doliczPodGabloty(sprzet_zapotrzebowanie& sprzet, gablota_jednostkowa typGabloty, sprzet_zapotrzebowanie iloscGablot);
};

class barierka_slupkowa_jednostkowa:public jednostka
{
public:
    barierka_slupkowa_jednostkowa(int ilosc0=0, double dlugosc0=1.5);
    void przelicz(sprzet_zapotrzebowanie& sprzet);
};

class barierka_stalowa_jednostkowa:public jednostka
{
public:
    barierka_stalowa_jednostkowa(int ilosc0=0, double dlugosc0=1.5);
    void przelicz(sprzet_zapotrzebowanie& sprzet);
};

class krzeslo_jednostkowe:public jednostka
{
public:
    krzeslo_jednostkowe(int ilosc0=0);
    void przelicz(sprzet_zapotrzebowanie& sprzet);
};

//-----magazyn----------------------------------------------------------------------------------

class magazyn
{
public:
    //Pola:
    QString nazwa;
    int ID; //ID w bazie danych
    jednostka * sprzetNaStanie[5];
    sprzet_zapotrzebowanie stolyPobrane;
    sprzet_zapotrzebowanie gablotyPobrane;
    sprzet_zapotrzebowanie barierkiSlupkowePobrane;
    sprzet_zapotrzebowanie barierkiStalowePobrane;
    sprzet_zapotrzebowanie krzeslaPobrane;

    //Metody:
    magazyn(QString Nazwa0="Nowy magazyn");
    void dodajZapotrzebowanie(sprzet_zapotrzebowanie NoweZapotrzebowanie);
};

//-----stoisko---------------------------------------------------------------------------------

class stoisko
{
public:
    //Pola:
    QString nazwa;
    int ID; //ID wystawcy z bazy danych
    sprzet_zapotrzebowanie stoly;
    sprzet_zapotrzebowanie gabloty;
    sprzet_zapotrzebowanie barierkiSlupkowe;
    sprzet_zapotrzebowanie barierkiStalowe;
    sprzet_zapotrzebowanie krzesla;
    int zapotrzebowaniePrad;
    int doliczStolyPodGabloty;

    //Metody:
    stoisko(QString n="Nowa wystawa", double ZS=0, int TZS=0, double ZG=0, int TZG=0, double ZBSL=0, int TZBSL=0, double ZBST=0, int TZBST=0, int ZK=0, int ZP=0);
    stoisko operator+(stoisko stoisko1);
    void zeruj();
};

//------wystawa---------------------------------------------------------------------------------------

class wystawa
{
public:
    //Pola:
    QString nazwa;
    int ID; //ID wystawy z bazy danych;
    QDate data; //planowana data wystawy
    QList<stoisko> stoiska;
    QList<magazyn> zaplecze;

    //Metody:
    wystawa(QString n0="Nowa wystawa");
    void przelicz_stoisko(stoisko& wystawca);
    //void przelicz();
    void dodaj_stoisko(stoisko * nowe_stoisko);
    void usun_stoisko(int nr_stoiska);
    void dodaj_magazyn(magazyn * nowy_magazyn);
    void usun_magazyn(int nr_magazynu);
    void przelicz_wszystko();
    int nrMagazynu(int ID_magazynu);
};

#endif
