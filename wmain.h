#ifndef WMAIN_H
#define WMAIN_H

#include <QMainWindow>
#include <post_engine.h>
#include <file_layer.h>

QT_BEGIN_NAMESPACE
namespace Ui { class WMain; }
QT_END_NAMESPACE

class WMain : public QMainWindow
{
    Q_OBJECT

public:
    WMain(QWidget *parent = nullptr);
    ~WMain();
    void  changeEvent(QEvent* pe);

private slots:
    void slot_change_state(Execution_State new_state);

    void on_pb_append_clicked();
    void on_pb_insert_clicked();
    void on_pb_delete_clicked();
    void on_pb_move_up_clicked();
    void on_pb_move_down_clicked();

    void on_pb_start_clicked();
    void on_pb_stop_clicked();
    void on_pb_debug_clicked();
    void on_pb_next_step_clicked();

    void on_pb_about_qt_clicked();

    void on_pb_file_new_clicked();

    void on_pb_file_save_clicked();

    void on_pb_file_load_clicked();

    void on_pb_about_clicked();

    void on_pb_help_clicked();

    void on_rb_speed_slow_clicked();

    void on_rb_speed_medium_clicked();

    void on_rb_speed_fast_clicked();

    void on_rb_speed_veryfast_clicked();

private:
    Ui::WMain*      ui;
    Post_Engine*    post_engine=nullptr;
    File_Layer*     file_layer=nullptr;
};
#endif // WMAIN_H
