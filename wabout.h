#ifndef WABOUT_H
#define WABOUT_H

#include <QDialog>

namespace Ui {
class WAbout;
}

class WAbout : public QDialog
{
    Q_OBJECT

public:
    explicit WAbout(QWidget *parent = nullptr);
    ~WAbout();
    void changeEvent(QEvent* pe);

private:
    Ui::WAbout *ui;
};

#endif // WABOUT_H
