#include "post_engine.h"
#include <QMessageBox>
#include <QDebug>

Post_Engine::Post_Engine(QObject *parent, Post_Program *post_program, Post_Tape *post_tape)
    : QObject{parent}
{
    this->execution_state = Execution_State::STOPPED;
    this->post_program = post_program;
    this->post_tape = post_tape;
    this->timer = new QTimer;
    connect(this->timer,&QTimer::timeout,
            this,&Post_Engine::slot_next_step_program);
    this->timer->setInterval( 1000 );
}

void Post_Engine::set_timer_speed(int speed)
{
    this->timer->setInterval( speed );
}

Post_Engine::~Post_Engine()
{
    delete timer;
}

Execution_State Post_Engine::get_execution_state()
{
    return this->execution_state;
}

void Post_Engine::start_program()
{
    if ( this->execution_state == Execution_State::RUN_AUTO ){ // сейчас запущено
        return; // ничего не делаем если уже запущено !
    }
    else if ( this->execution_state == Execution_State::RUN_STEP ) {// сейчас запущено пошагово
        this->execution_state = Execution_State::RUN_AUTO;
        emit this->change_state( this->execution_state );
        this->timer->start(); // продолжим исполнение автоматически
    }
    else if ( this->execution_state == Execution_State::STOPPED ){ // сейчас остановлено
        this->post_program->selectRow(0); // перейдём на первую строку программы
        this->execution_state = Execution_State::RUN_AUTO;
        emit this->change_state( this->execution_state );
        this->timer->start(); // запустим исполнение автоматически
    };
}

void Post_Engine::stop_program()
{
    this->execution_state = Execution_State::STOPPED;
    this->timer->stop();
    emit this->change_state( this->execution_state );
}

void Post_Engine::debug_program_from_current_line()
{
    if ( this->execution_state == Execution_State::RUN_STEP ) {
        return;
    }
    else if ( this->execution_state == Execution_State::RUN_AUTO
              or this->execution_state == Execution_State::STOPPED ){
        this->timer->stop();
        this->execution_state = Execution_State::RUN_STEP;
        emit this->change_state( this->execution_state );
    };
}

void Post_Engine::error_program(QString error)
{
    this->timer->stop();

    this->execution_state = Execution_State::STOPPED;
    emit this->change_state( this->execution_state );

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
        this->error_program( tr("Error! Bad reference! The reference is not within the acceptable range! ") );
        return false;
    }
    return true;
}

int Post_Engine::get_int_reference(QString reference)
{
    return reference.toInt();
}

void Post_Engine::step_program()
{
    // qDebug() << "###";
    int current_row =   this->post_program->currentRow();
    QString command   = this->post_program->get_cell_string(current_row,0);
    QString reference = this->post_program->get_cell_string(current_row,1);
    QString comment   = this->post_program->get_cell_string(current_row,2);

    if ( (current_row+1)==this->post_program->rowCount() // проверяем штатный конец программы
         and reference.trimmed().isEmpty()
         and command!="stop" ){
        this->error_program( tr("Error! The program ended unexpectedly!") );
        return;
    };

    if ( command=="" ){///+
        this->error_program( tr("Error! Empty instruction!") );
        return;
    }
    else if ( command=="left" ){///-+
         //programm handling reference error
        if ( reference.trimmed().isEmpty() ){
            reference = QString::number( current_row+2 );
        }
        else{
            if ( !this->check_reference(reference) )
                return;
        };

        // ...... tape handling left edge exit error
        this->post_tape->command_tape_left();
        int int_reference = this->get_int_reference(reference);
        this->post_program->selectRow( int_reference-1 );
        return;
    }
    else if ( command=="right" ){///-+
        //programm handling reference error
        if ( reference.trimmed().isEmpty() ){
            reference = QString::number( current_row+2 );
        }
        else{
            if ( !this->check_reference(reference) )
                return;
        };

        // ...... tape handling right edge exit error
        this->post_tape->command_tape_right();
        int int_reference = this->get_int_reference(reference);
        this->post_program->selectRow( int_reference-1 );
        return;
    }
    else if ( command=="mark" ){///-+
        //programm handling reference error
        if ( reference.trimmed().isEmpty() ){
            reference = QString::number( current_row+2 );
        }
        else{
            if ( !this->check_reference(reference) )
                return;
        };

        // ...... tape handling already mark error
        this->post_tape->command_tape_mark();
        int int_reference = this->get_int_reference(reference);
        this->post_program->selectRow( int_reference-1 );
        return;
    }
    else if ( command=="erase" ){///-+
        //programm handling reference error
        if ( reference.trimmed().isEmpty() ){
            reference = QString::number( current_row+2 );
        }
        else{
            if ( !this->check_reference(reference) )
                return;
        };

        // ...... tape handling nothing to erase error
        this->post_tape->command_tape_erase();
        int int_reference = this->get_int_reference(reference);
        this->post_program->selectRow( int_reference-1 );
        return;
    }
    else if ( command=="condition" ){ ///+
        //programm handling reference error for N,M notation
        if ( reference.count(",") != 1 ){
            this->error_program( tr("Error! Bad reference! Valid is 'N,M' !") );
            return;
        };
        QStringList sl = reference.split(",");
        if ( sl.count() != 2 ){
            this->error_program( tr("Error! Bad reference! Valid is 'N,M' !") );
            return;
        }
        QString ref_empty_s  = sl[0];
        QString ref_filled_s = sl[1];
        bool is_valid;
        int ref_empty;
        int ref_filled;
        ref_empty = ref_empty_s.toInt(&is_valid);
        if ( !is_valid ){
            this->error_program( tr("Error! Bad reference! Not valid 'empty' number!") );
            return;
        };
        ref_filled = ref_filled_s.toInt(&is_valid);
        if ( !is_valid ){
            this->error_program( tr("Error! Bad reference! Not valid 'filled' number!") );
            return;
        };
        if ( ref_empty<1 or ref_empty>this->post_program->rowCount() ){
            this->error_program( tr("Error! Bad reference! The 'empty' reference is not within the acceptable range! ") );
            return;
        };
        if ( ref_filled<1 or ref_filled>this->post_program->rowCount() ){
            this->error_program( tr("Error! Bad reference! The 'filled' reference is not within the acceptable range! ") );
            return;
        };
        // CONDITION IF LOGIC !
        bool current_tape_cell_marked = false;
        this->post_tape->command_tape_read( current_tape_cell_marked );
        if ( !current_tape_cell_marked ){
            this->post_program->selectRow( ref_empty-1 );
            return;
        }
        else{
            this->post_program->selectRow( ref_filled-1 );
            return;
        };
    }
    else if ( command=="stop" ){///+
        this->timer->stop();
        this->execution_state = Execution_State::STOPPED;
        emit this->change_state( this->execution_state );
        QMessageBox msgBox;
        msgBox.setWindowTitle( tr("Info") );
        msgBox.setText( tr("Programm success finished!") );
        msgBox.exec();
        return;
    };
}

void Post_Engine::slot_next_step_program() // слот для таймера авто-выыполнения программы
{
    if ( this->execution_state == Execution_State::RUN_AUTO
         or this->execution_state == Execution_State::RUN_STEP ){
        this->step_program(); // обрабатываем очередной шаг программы
    };
}


