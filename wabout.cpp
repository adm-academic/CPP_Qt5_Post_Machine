#include "wabout.h"
#include "ui_wabout.h"

wabout::wabout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wabout)
{
    ui->setupUi(this);
}

wabout::~wabout()
{
    delete ui;
}
