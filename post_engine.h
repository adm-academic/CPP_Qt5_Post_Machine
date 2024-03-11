#ifndef POST_ENGINE_H
#define POST_ENGINE_H

#include <QObject>
#include <post_command.h>
#include <post_program.h>
#include <post_tape.h>
#include <QTimer>

enum class Execution_State
{
    STOPPED,  // остановлена
    RUN_AUTO, // автоматическое выполнение
    RUN_STEP  // пошаговое выполнение
};

class Post_Engine : public QObject
{
    Q_OBJECT
public:
    explicit Post_Engine(QObject *parent, Post_Program* post_program, Post_Tape* post_tape );
    virtual ~Post_Engine();

    Execution_State get_execution_state(); // возвращает текущее состояние движка

    void start_program(); // стартует программу
    void step_program();  // делает следующий шаг программы
    void stop_program();  // останавливает программу
    void debug_program_from_current_line(); // с текущей строки программы включает пошаговый режим
    void error_program(QString error); // останавливает программу аварийно

    void set_timer_speed(int speed);

protected:
    bool check_reference(QString reference);
    int  get_int_reference(QString reference);

signals:
    void change_state(Execution_State new_state);

private slots:
    void slot_next_step_program();

private:
    Post_Program* post_program = nullptr;
    Post_Tape* post_tape = nullptr;
    Execution_State execution_state = Execution_State::STOPPED;
    QTimer* timer = nullptr;
};

#endif // POST_ENGINE_H
