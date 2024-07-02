#include "wabout.h"
#include "ui_wabout.h"

WAbout::WAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WAbout)
{
    ui->setupUi(this);
}

WAbout::~WAbout()
{
    delete ui;
}


void WAbout::changeEvent(QEvent* pe) // нужно для перевода окна "На лету"
{
    if (pe->type() == QEvent::LanguageChange){
        ui->retranslateUi(this);
    };
    QWidget::changeEvent(pe);
}
