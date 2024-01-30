#ifndef POST_COMMAND_H
#define POST_COMMAND_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>

class Post_Command_List : public QListWidget
{
    Q_OBJECT
public:
    Post_Command_List(QWidget *parent = nullptr);
    virtual ~Post_Command_List();
    QString get_selected_command();
    void    set_selected_command( QString command );
    QIcon get_selected_icon();
    const int icon_size = 16;

protected:
    void leaveEvent(QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;   

};


class Post_Command : public QLineEdit
{
    Q_OBJECT
public:
    explicit Post_Command(QWidget *parent = nullptr);
    virtual ~Post_Command();
    QString get_selected_command();
    void    set_selected_command( QString command );
    const int icon_padding = 10;

signals:

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void show_list();
    Post_Command_List* list_widget;

};

#endif // POST_COMMAND_H
