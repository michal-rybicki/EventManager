#ifndef EDYCJAWYSTAWY_H
#define EDYCJAWYSTAWY_H

#include <QDialog>
#include "wystawy.h"

namespace Ui {
class edycjawystawy;
}

class edycjawystawy : public QDialog
{
    Q_OBJECT

public:
    wystawa * starforce;
    explicit edycjawystawy(QWidget *parent = nullptr);
    ~edycjawystawy();

public slots:
    void Ustaw(wystawa * starforce0);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::edycjawystawy *ui;
};

#endif // EDYCJAWYSTAWY_H
