#ifndef WABOUT_H
#define WABOUT_H

#include <QDialog>

namespace Ui {
class wabout;
}

class wabout : public QDialog
{
    Q_OBJECT

public:
    explicit wabout(QWidget *parent = nullptr);
    ~wabout();

private:
    Ui::wabout *ui;
};

#endif // WABOUT_H
