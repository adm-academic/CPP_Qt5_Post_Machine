#ifndef WMAIN_H
#define WMAIN_H

#include <QMainWindow>

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

private:
    Ui::WMain *ui;
};
#endif // WMAIN_H
