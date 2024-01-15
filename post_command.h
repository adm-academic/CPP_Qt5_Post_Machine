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

signals:
    void change_command(QString new_command);

protected:
    void leaveEvent(QEvent *event) override;

protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};


class Post_Command : public QLineEdit
{
    Q_OBJECT
public:
    explicit Post_Command(QWidget *parent = nullptr);
    virtual ~Post_Command();
    QString get_selected_command();

signals:

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void show_list();
    Post_Command_List* list_widget;
    QString command;

protected slots:
    void slot_change_command(QString new_command);

};

#endif // POST_COMMAND_H
