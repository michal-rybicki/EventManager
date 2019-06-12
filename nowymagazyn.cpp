#include "nowymagazyn.h"
#include "ui_nowymagazyn.h"

nowymagazyn::nowymagazyn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::nowymagazyn)
{
    ui->setupUi(this);
    this->setWindowTitle("Nowy magazyn...");
    this->setFixedSize(460,224);//ustawianie stalego rozmiaru okna

    //Zabezpieczenia wprowadzania wartosci:
    QDoubleValidator * Straznik = new QDoubleValidator(0, 1000, 2, this);
    ui->Stoly->setValidator(Straznik);
    ui->Gabloty->setValidator(Straznik);
    ui->BarierkiSlupkowe->setValidator(Straznik);
    ui->BarierkiStalowe->setValidator(Straznik);
    ui->Krzesla->setValidator(Straznik);

    double min=0.1;
    QDoubleValidator * StraznikDodatni = new QDoubleValidator(min, 1000, 2, this);
    StraznikDodatni->setBottom(min);
    ui->DlStolu->setValidator(StraznikDodatni);
    ui->SzStolu->setValidator(StraznikDodatni);
    ui->DlGabloty->setValidator(StraznikDodatni);
    ui->SzGabloty->setValidator(StraznikDodatni);
    ui->DlBarierySlupkowej->setValidator(StraznikDodatni);
    ui->DlBarieryStalowej->setValidator(StraznikDodatni);

    //wypelnianie formularza danymi domyslnymi magazynu
    ui->DlStolu->setText(QString::number(1.2));
    ui->SzStolu->setText(QString::number(0.5));
    ui->DlGabloty->setText(QString::number(1.65));
    ui->SzGabloty->setText(QString::number(0.65));
    ui->DlBarierySlupkowej->setText(QString::number(2));
    ui->DlBarieryStalowej->setText(QString::number(2.5));
}

nowymagazyn::~nowymagazyn()
{
    delete ui;
}

//---tworzy nowy magazyn i emituje go do okna glownego------------------------------------------------------------------------------

void nowymagazyn::on_nowymagazyn_accepted()
{
    magazyn * N;
    double wymiar1=0,wymiar2=0;
    N = new magazyn;
    N->nazwa=ui->nazwa->text();
    N->sprzetNaStanie[0]->ilosc=ui->Stoly->text().toInt();
    wymiar1=tools::toDouble(ui->DlStolu->text());
    wymiar2=tools::toDouble(ui->SzStolu->text());
    N->sprzetNaStanie[0]->ustawWymiary(wymiar1,wymiar2);
    N->sprzetNaStanie[1]->ilosc=ui->Gabloty->text().toInt();
    wymiar1=tools::toDouble(ui->DlGabloty->text());
    wymiar2=tools::toDouble(ui->SzGabloty->text());
    N->sprzetNaStanie[1]->ustawWymiary(wymiar1,wymiar2);
    N->sprzetNaStanie[2]->ilosc=ui->BarierkiSlupkowe->text().toInt();
    N->sprzetNaStanie[2]->ustawWymiary(tools::toDouble(ui->DlBarierySlupkowej->text()),0);
    N->sprzetNaStanie[3]->ilosc=ui->BarierkiStalowe->text().toInt();
    N->sprzetNaStanie[3]->ustawWymiary(tools::toDouble(ui->DlBarieryStalowej->text()),0);
    N->sprzetNaStanie[4]->ilosc=ui->Krzesla->text().toInt();
    emit StworzonyMagazyn(N);
}
