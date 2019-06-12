#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "wystawy.h"
#include <QFileDialog>
#include <QMainWindow>
#include <QTextStream>
#include <QDataStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QtSql>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSqlDatabase db; //polaczenie do bazy danych
    void DopiszStoisko(stoisko * N);//dopisuje stoisko do tabeli
    void DopiszMagazyn(magazyn * N);//dopisuje magazyn do tabeli
    void PolaczZBazaDanych(); //laczy z baza danych;
    void Koloruj();
    void ZapiszDoPliku();//zapisuje wystawe do pliku
    void ZapiszDoBazy();//zapisuje wystawe w bazie danych
    void ZaladujPlik();//laduje dane z pliku do wystawy
    void WczytajZBazy(int ID_wystawy);//laduje dane o wystawie z bazy danych
    void NowyPlik();//czysci tabele i tworzy nowa wystawe
    int TrybWyswietlania;//0 - ilosci w tabeli stoisk wyswietlane w jednostkach podanych przez uzytkowanika
                         //1 - ilosci w tabeli stoisk wyswietlane w szt.
    int TrybWyswietlaniaMagazynow; //0 - ilosc pobrana z magazynu
                                   //1 - ilosc pozostala w magazynie
                                   //2 - ilosc w sumie dostepna w magazynie
    QFile plik;//aktualnie otwartu plik z danymi wystawy
    wystawa * starforce;//podstawowy obiekt na ktorym sa wykonywane operacje

    //Do testowania:
    bool TrybTestowania;
    int testySprawdzajStoiska;//do testowania
    int testySprawdzajStareMagazyny;//do testowania
    void TestStoisk();//do testowania
    void TestStarychMagazynow();
    void InformacjaTestowa(QString informacja);

signals:
    AktualnaWystawaDoStoisk(wystawa*);//sygnal przekazyjacy stan wystawy do innych okien
    AktualnaWystawaDoMagazynow(wystawa*);//sygnal przekazyjacy stan wystawy do innych okien
    AktualnaWystawaDoNowegoStoiska(wystawa*);//sygnal przekazyjacy stan wystawy do innych okien
    AktualnaWystawaDoEdycji(wystawa*);//sygnal przekazyjacy stan wystawy do innych okien
    AktualnaWystawaDoHistorii(wystawa*);//sygnal przekazyjacy stan wystawy do innych okien

public slots:
    void DodajStoisko(stoisko * N);//dodaje stoisko do wystawy i dopisuje do tabeli
    void DodajMagazyn(magazyn * N);//dodaje magazyn do wystawy i dopisuje do tabeli
    void UaktualnijTabeleMagazynow();//uaktualnia tabele magazynow
    void UaktualnijTabeleStoisk();//auaktualnia tabele stoisk
    void UaktualnijMagazyny();//przelicza wystawe i uaktualnia tabele magazynow
    void UaktualnijStoiska();//przelicza wystawe i uaktualnia tabele stoisk oraz magazynow
    void ZamknijProgram();
    void UsunWierszStoisk();//usuwa ostatni wiersz tabeli stoisk i uaktualnia tabele stoisk
    void UsunWierszMagazynow();//usuwa ostatni wiersz tabeli magazynow i uaktualnia tabele magazynow
    void wybranoWystawe(int ID_wystawy); //ustawia wybrana wystawe z bazy danych


private slots:
    void on_pushButtonStoisko_clicked();
    void on_pushButtonSprzet_clicked();
    void on_EdytujMagazyny_clicked();
    void on_EdytujStoiska_clicked();
    void on_actionOtw_rz_triggered();
    void on_actionZapisz_triggered();
    void on_actionZapisz_jako_triggered();
    void on_actionZapisz_do_bazy_danych_triggered();
    void on_actionWczytaj_z_bazy_danych_triggered();
    void on_Przelicz_stateChanged(int arg1);
    void on_actionNowy_triggered();
    void on_actionZamknij_triggered();


    void on_zerezerwowane_clicked();

    void on_wolne_clicked();

    void on_wsumie_clicked();

    void on_actionW_a_ciwo_ci_wystawy_triggered();

    void on_actionPobierz_dane_z_innej_wystawy_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
