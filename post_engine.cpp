#include "post_engine.h"
#include <QMessageBox>
#include <QDebug>

Post_Engine::Post_Engine(QObject *parent, Post_Program *post_program, Post_Tape *post_tape)
    : QObject{parent}
{
    this->post_program = post_program;
    this->post_tape = post_tape;
    this->timer = new QTimer;
    connect(this->timer,&QTimer::timeout,
            this,&Post_Engine::slot_next_step_program);
    this->timer->setInterval(500);
}

Post_Engine::~Post_Engine()
{
    delete timer;
}

Execution_State Post_Engine::get_execution_state()
{
    return this->execution_state;
}

int Post_Engine::get_current_row()
{
    return this->current_row;
}

void Post_Engine::start_program()
{
    if ( this->execution_state == Execution_State::RUN_AUTO )
        return;
    else if ( this->execution_state == Execution_State::RUN_STEP )
        this->timer->start();
    else if ( this->execution_state == Execution_State::STOPPED ){
        this->current_row = 0;
        this->timer->start();
    };
}

void Post_Engine::break_program()
{

}

void Post_Engine::stop_program()
{

}

void Post_Engine::error_program(QString error)
{
    this->timer->stop();
    this->execution_state = Execution_State::STOPPED;
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Post Machine Error!"));
    msgBox.setText( error );
    msgBox.exec();
}

bool Post_Engine::check_reference(QString reference)
{
    bool is_valid;
    int reference_int = reference.toInt(&is_valid);
    if ( !is_valid ){
        this->error_program( tr("Error! Bad reference! Not valid number!") );
        return false;
    };
    if ( reference_int<1 or reference_int>this->post_program->rowCount() ){
        this->error_program( tr("Error! Bad reference! The link is not within the acceptable range! ") );
        return false;
    }
    return true;
}

void Post_Engine::step_program()
{
    // qDebug() << "###";
    this->current_row = this->post_program->currentRow();
    QString command   = this->post_program->get_cell_string(this->current_row,0);
    QString reference = this->post_program->get_cell_string(this->current_row,1);
    QString comment   = this->post_program->get_cell_string(this->current_row,2);

    if ( command=="" ){
        this->error_program( tr("Error! Empty instruction!") );
        return;
    }
    else if ( command=="left" ){
        //...tape handling left edge exit error

        //programm handling reference error
        if ( !this->check_reference(reference) )
            return;
        //........
    }
    else if ( command=="right" ){
        //...tape handling right edge exit error

        //...programm handling reference error

        //........
    }
    else if ( command=="mark" ){
        //...tape handling already mark error

        //...programm handling reference error

        //........
    }
    else if ( command=="erase" ){
        //...tape handling nothing to erase error

        //...programm handling reference error

        //........
    }
    else if ( command=="condition" ){
        //...programm handling reference error for N,M notation

    }
    else if ( command=="stop" ){
        this->timer->stop();
        this->execution_state = Execution_State::STOPPED;
        QMessageBox msgBox;
        msgBox.setWindowTitle( tr("Info") );
        msgBox.setText( tr("Programm success finished!") );
        msgBox.exec();
        return;
    };
}

void Post_Engine::slot_next_step_program() // слот для таймера авто-выыполнения программы
{
    this->step_program(); // обрабатываем очередной шаг программы
}


