#ifndef WMAIN_H
#define WMAIN_H

#include <QMainWindow>
#include <post_engine.h>

QT_BEGIN_NAMESPACE
namespace Ui { class WMain; }
QT_END_NAMESPACE

class WMain : public QMainWindow
{
    Q_OBJECT

public:
    WMain(QWidget *parent = nullptr);
    ~WMain();

private slots:
    void on_pb_left_clicked();
    void on_pb_right_clicked();
    void on_pb_set_clicked();
    void on_pb_erase_clicked();
    void on_pb_read_clicked();

    void on_pushButton_clicked();



    void on_pb_append_clicked();
    void on_pb_insert_clicked();
    void on_pb_delete_clicked();
    void on_pb_move_up_clicked();
    void on_pb_move_down_clicked();

    void on_pb_start_clicked();
    void on_pb_stop_clicked();
    void on_pb_break_clicked();
    void on_pb_step_by_step_clicked();

private:
    Ui::WMain *ui;
    Post_Engine* post_engine;
};
#endif // WMAIN_H
