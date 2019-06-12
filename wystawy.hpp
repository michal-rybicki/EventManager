#ifndef wystawy_hpp
#define wystawy_hpp

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



class sprzet;
class stoisko;

//-----sprzet---------------------------------------------------------------------------------

class sprzet
{
public:
    int ilosc;
    int cena;
    int typ_ceny;
    sprzet(int ilosc0, int cena0, int typ_ceny0);
    ustaw(int x0, int y0);
};

sprzet::sprzet(int ilosc0=0, int cena0=0, int typ_ceny0=0)
{
    ilosc=ilosc0;
    cena=cena0;
    typ_ceny=typ_ceny0;
}

class stoisko
{
public:
    QString nazwa;
    double zapotrzebowanieStoly;
    int typZapotrzebowaniaStolow;
    double zapotrzebowanieBarierkiSlupkowe;
    int typZapotrzebowaniaBarierekSlupkowych;
    int zapotrzebowanieKrzesla;
    int zapotrzebowaniePrad;

    stoisko(QString n, double ZS, double TZS, double ZBS, double TZBS, int ZK, int ZP);
};

stoisko::stoisko(QString n="Nowa wystawa", double ZS=0, double TZS=0, double ZBS=0, double TZBS=0, int ZK=0, int ZP=0)
{
    nazwa=n;
    zapotrzebowanieStoly=ZS;
    typZapotrzebowaniaStolow=TZS;
    zapotrzebowanieBarierkiSlupkowe=ZBS;
    typZapotrzebowaniaBarierekSlupkowych=TZBS;
    zapotrzebowanieKrzesla=ZK;
    zapotrzebowaniePrad=ZP;
}
#endif
