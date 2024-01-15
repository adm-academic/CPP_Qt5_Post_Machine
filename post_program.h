#ifndef POST_PROGRAM_H
#define POST_PROGRAM_H

#include <QWidget>
#include <QTableWidget>

class Post_Program : public QTableWidget
{
    Q_OBJECT
public:
    explicit Post_Program(QWidget *parent = nullptr);
    virtual ~Post_Program();

signals:

};

#endif // POST_PROGRAM_H
