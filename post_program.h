#ifndef POST_PROGRAM_H
#define POST_PROGRAM_H

#include <QWidget>
#include <QTableWidget>

class Post_Program : public QTableWidget
{
    Q_OBJECT

    friend QAbstractItemView;

public:
    explicit Post_Program( QWidget *parent = nullptr );
    virtual ~Post_Program();

    QString get_cell_string(int cell_row, int cell_col);

    void row_append();
    void row_insert();
    void row_delete();
    void row_move_up();
    void row_move_down();



signals:


protected:
    void paintEvent(QPaintEvent *event) override;
    void initialize();

protected slots:
    void slot_rows_inserted(const QModelIndex &parent, int start, int end);

};

#endif // POST_PROGRAM_H
