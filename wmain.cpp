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
#include <QDebug>
#include "post_command.h"

WMain::WMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WMain)
{
    ui->setupUi(this);

    move(screen()->geometry().center() - frameGeometry().center());    

    this->post_engine = new Post_Engine(this,this->ui->program_widget,this->ui->tape_widget);
}

WMain::~WMain()
{
    delete ui;
}

void WMain::slot_change_state(Execution_State new_state)
{
    //..................................
}

void WMain::on_pb_append_clicked()
{
    this->ui->program_widget->row_append();
}


void WMain::on_pb_insert_clicked()
{
    this->ui->program_widget->row_insert();
}


void WMain::on_pb_delete_clicked()
{
    this->ui->program_widget->row_delete();
}


void WMain::on_pb_move_up_clicked()
{
    this->ui->program_widget->row_move_up();
}


void WMain::on_pb_move_down_clicked()
{
    this->ui->program_widget->row_move_down();
}


void WMain::on_pb_start_clicked()
{
    this->post_engine->start_program();
}


void WMain::on_pb_stop_clicked()
{
    this->post_engine->stop_program();
}


void WMain::on_pb_break_clicked()
{
    this->post_engine->break_program();
}


void WMain::on_pb_step_by_step_clicked()
{
    this->post_engine->step_program();
}

