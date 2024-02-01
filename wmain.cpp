#include "wmain.h"
#include "ui_wmain.h"
#include "global_forms.h"
#include <QWidget>
#include <QScreen>
#include <QApplication>
#include <QDebug>
#include "post_command.h"

WMain::WMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WMain)
{
    ui->setupUi(this);

    move(screen()->geometry().center() - frameGeometry().center());    

    this->post_engine = new Post_Engine(this,this->ui->program_widget,this->ui->tape_widget);
    connect( this->post_engine, &Post_Engine::change_state,
             this, &WMain::slot_change_state );
    this->slot_change_state(Execution_State::STOPPED);
    this->ui->toolbox_menu->setCurrentIndex(1);
}

WMain::~WMain()
{
    delete ui;
}

void WMain::slot_change_state(Execution_State new_state)
{
    if (new_state==Execution_State::RUN_AUTO) {
        this->ui->lb_mode->setText("RUNNING");

        this->ui->pb_start->setEnabled(false);
        this->ui->pb_next_step->setEnabled(false);

    }
    else if (new_state==Execution_State::RUN_STEP){
        this->ui->lb_mode->setText("STEP BY STEP");

        this->ui->pb_next_step->setEnabled(true);
        this->ui->pb_start->setEnabled(true);

    }
    else if (new_state==Execution_State::STOPPED){
        this->ui->lb_mode->setText("STOPPED");

        this->ui->pb_start->setEnabled(true);
        this->ui->pb_next_step->setEnabled(false);

    };
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

void WMain::on_pb_debug_clicked()
{
    this->post_engine->debug_program_from_current_line();
}

void WMain::on_pb_next_step_clicked()
{
    this->post_engine->step_program();
}



void WMain::on_pb_about_qt_clicked()
{
    QApplication::aboutQt();
}

