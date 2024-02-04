#ifndef POST_TAPE_H
#define POST_TAPE_H

#include <QWidget>
#include <QRect>
#include <QToolButton>
#include <QMap>
#include <QPolygon>

typedef QMap<int,QRect> Tape_Rects; // тип коллекции для отображаемой виджетом части ленты.
typedef QMap<int,short> Tape; // тип коллекции для самой ленты Машины Поста.

class Post_Tape : public QWidget // класс виджета ленты Машины Поста.
{
    Q_OBJECT
public:
    explicit Post_Tape(QWidget *parent = nullptr);
    virtual ~Post_Tape();
    const int tape_size_from = -9999; // левый край ленты
    const int tape_size_to = 9999; // правый край ленты
    // общая длинна ленты составляет 19998 ячеек

    int get_tape_visible_start() const; // геттер начальной отображаемой ячейки
    void set_tape_visible_start(int new_tape_visible_start); // сеттер начальной отображаемой ячейки
    int get_tape_visible_length() const; // геттер визуальной длинны ленты

    int map_visible_to_tape(int rect_index); // переводит координату из экранной в ленточную
    int map_tape_to_visible(int cell_index); // переводит координату из ленточной в экранную


    void get_fast_tape_cell( int& cell_index, short& cell_value ); // возвращает значение ячейки ленты по координате
    void set_fast_tape_cell( int& cell_index, short& cell_value ); // устанавливает значение ячейки ленты по координате
    void clear(); // очистить всю ленту
    int  get_carriage_position(); // возвращает позицию каретки над лентой
    void set_carriage_position(int position);// устанавливает позицию каретки над лентой


    bool command_tape_left(); // переход влево
    bool command_tape_right();// переход вправо
    bool command_tape_mark();  // установить метку в текущей ячейке
    bool command_tape_erase();// стереть метку в текущей ячейке
    bool command_tape_read(bool & have_mark); // читает отметку из текущей ячейки


signals:


protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void init_cell_rects(); // инициализиует интерфейс ленты
    void init_tape(); // инициализирует саму ленту, все 19998 ячеек
    int  get_visible_rect_index_by_xy(int x, int y); // возвращает индекс ячейки в ленте по графическим
                                                     // координатам x,y

private:
    QToolButton* tape_scroll_left = nullptr; // кнопка влево
    QToolButton* tape_scroll_right = nullptr; // кнопка вправо
    Tape_Rects*  tape_rects = nullptr; // коллекция видимых ячеек ленты
    QRect*       tape_bounding_rect = nullptr; // прямоуголник ленты
    QPolygon*    carriage_polygon = nullptr; // полигон каретки
    Tape*        tape = nullptr; // вся коллекция ленты
    int          tape_cell_width; // ширина ячейки ленты
    int          tape_cell_height; // высота ячейки ленты

    int          tape_visible_start; // стартовая позиция отображения ленты
    int          tape_visible_length; // длинна отображаемого фрагмента ленты
    int          carriage_tape_position; // позиция каретки над прямоугольником...

private slots:
    void slot_on_left_clicked(); // слот для перемотки ленты влево
    void slot_on_right_clicked(); // слот для перемотки ленты вправо

};

#endif // POST_TAPE_H
