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
    int get_current_row(); // возвращает текущую строку движка

    void start_program(); // стартует программу
    void break_program(); // останавливает программу
    void step_program();  // делает следующий шаг программы
    void stop_program();  // останавливает программу штатно
    void error_program(QString error); // останавливает программу аварийно

protected:
    bool check_reference(QString reference);

signals:

private slots:
    void slot_next_step_program();

private:
    Post_Program* post_program = nullptr;
    Post_Tape* post_tape = nullptr;
    int current_row = 0;
    Execution_State execution_state = Execution_State::STOPPED;
    QTimer* timer = nullptr;



};

#endif // POST_ENGINE_H
