#include "wmain.h"
#include "ui_wmain.h"
#include "global_forms.h"
#include <QWidget>
#include <QScreen>
#include <QApplication>
#include <QMessageBox>
#include <QComboBox>
#include <QPalette>
#include <QLineEdit>
#include "post_command.h"

WMain::WMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WMain)
{
    ui->setupUi(this);

    move(screen()->geometry().center() - frameGeometry().center());

//    for(int i=0;i<this->ui->program_widget->rowCount();i++){
//        QComboBox* pcb = new QComboBox(this->ui->program_widget);
//        pcb->addItems( QStringList() << "<" << ">" << "s" << "e" << "if" << "!" );
//        //pcb->setStyleSheet("QComboBox::drop-down {border-width: 0px;  } "
//        //                   "QComboBox::down-arrow {image: none; border-width: 0px; } "
//        //                   );

//        // 1: Установить editable (чтобы использовать lineEdit)
//        pcb->setEditable(true);
//        // 2: Запретить в lineEdit что-либо редактировать
//        pcb->lineEdit()->setReadOnly(true);
//        // 3: Установить выравнивание для lineEdit
//        pcb->lineEdit()->setAlignment(Qt::AlignCenter);

//        this->ui->program_widget->setCellWidget(i,0,pcb);
//    };
    for(int i=0;i<this->ui->program_widget->rowCount();i++){
        Post_Command* ppc = new Post_Command(this->ui->program_widget);

        this->ui->program_widget->setCellWidget(i,0,ppc);
    }

}

WMain::~WMain()
{
    delete ui;
}


void WMain::on_pb_left_clicked()
{
    this->ui->tape_widget->command_tape_left();
}

void WMain::on_pb_right_clicked()
{
    this->ui->tape_widget->command_tape_right();
}

void WMain::on_pb_set_clicked()
{
    this->ui->tape_widget->command_tape_set();
}

void WMain::on_pb_erase_clicked()
{
    this->ui->tape_widget->command_tape_erase();
}


void WMain::on_pb_read_clicked()
{
    bool mark=false;
    this->ui->tape_widget->command_tape_read( mark );
    QMessageBox::warning(this, "Read", QString::number( mark ) );
}

