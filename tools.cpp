#include "tools.h"

double tools::toDouble(QString liczba)
{
    double wynik=0;
    int potega=1;
    int przecinek=1;
    int przecinek_zliczanie=1;
    for (int i=liczba.length()-1; i>=0; i--)
    {
        if (liczba[i].isDigit())
        {
            wynik=wynik+liczba[i].digitValue()*potega;
            potega=potega*10;
            przecinek_zliczanie=przecinek_zliczanie*10;
        }
        else
        {
            przecinek=przecinek_zliczanie;
        }
    }
    wynik=wynik/przecinek;
    return wynik;
};

QString tools::toString(double liczba)
{
    QString wynik;

    return wynik;
};
