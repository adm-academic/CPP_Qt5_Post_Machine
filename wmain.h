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

private:
    Ui::WMain *ui;
};
#endif // WMAIN_H
