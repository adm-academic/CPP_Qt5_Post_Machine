#include "wmain.h"
#include "ui_wmain.h"
#include <QFile>

WMain::WMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WMain)
{
    ui->setupUi(this);

    QFile histogramFile("settings.conf");
    if (histogramFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray histogramByteArray = histogramFile.readAll();
        QString s(histogramByteArray);
        this->ui->pte_Main->setPlainText(s);
    }

}

WMain::~WMain()
{
    delete ui;
}

