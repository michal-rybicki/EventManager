#include "edycjawystawy.h"
#include "ui_edycjawystawy.h"

edycjawystawy::edycjawystawy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edycjawystawy)
{
    ui->setupUi(this);
    this->setWindowTitle("Edycja wystawy...");

}

edycjawystawy::~edycjawystawy()
{
    delete ui;
}

void edycjawystawy::Ustaw(wystawa* starforce0)
{
    starforce=starforce0;
    ui->Nazwa->setText(starforce0->nazwa);
    ui->data->setDate(starforce->data);

};
void edycjawystawy::on_buttonBox_accepted()
{
    if (ui->Nazwa->text().length()>0)
    {
        starforce->nazwa=ui->Nazwa->text();
        starforce->data.setDate(ui->data->date().year(),ui->data->date().month(),ui->data->date().day());
    }
}
