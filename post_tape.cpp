#include "post_tape.h"
#include "global_forms.h"
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <limits>
#include <QMouseEvent>

Post_Tape::Post_Tape(QWidget *parent)
    : QWidget{parent}
{   
    this->tape_scroll_left  = new QToolButton(this) ;
    this->tape_scroll_left->setText("<");
    this->tape_scroll_left->setAutoRepeat(true);
    this->tape_scroll_left->setAutoRepeatDelay(1000);
    this->tape_scroll_left->setAutoRepeatInterval(20);
    this->tape_scroll_right = new QToolButton(this);
    this->tape_scroll_right->setText(">");
    this->tape_scroll_right->setAutoRepeat(true);
    this->tape_scroll_right->setAutoRepeatDelay(1000);
    this->tape_scroll_right->setAutoRepeatInterval(20);
    this->tape_rects        = new Tape_Rects;
    this->tape_bounding_rect= new QRect();
    this->carriage_polygon  = new QPolygon();
    this->tape              = new Tape();

    this->init_tape();
    this->init_cell_rects();
    this->set_tape_visible_start(-8);


    connect((this->tape_scroll_left),SIGNAL(clicked()),
            (this),SLOT(slot_on_left_clicked()));
    connect((this->tape_scroll_right),SIGNAL(clicked()),
            (this),SLOT(slot_on_right_clicked()));
}

Post_Tape::~Post_Tape()
{
    delete this->tape_scroll_right;
    delete this->tape_scroll_left;
    delete this->tape_bounding_rect;
    delete this->tape_rects;
    delete this->carriage_polygon;
    delete this->tape;
}


void Post_Tape::init_cell_rects()
{
    //------------ кнопки прокрутки ленты ------------------------------------------------
    this->tape_scroll_left->resize(this->height()/3,this->height());
    this->tape_scroll_left->move(0,0);
    this->tape_scroll_left->show();
    this->tape_scroll_right->resize(this->height()/3,this->height());
    this->tape_scroll_right->move(this->width()-this->tape_scroll_right->width(),0);
    this->tape_scroll_right->show();
    //------------- видимые ячейки ленты -------------------------------------------------
    int bounding_rect_left_begin = 0+this->tape_scroll_left->width()+1;
    int bounding_rect_right_end = this->width() - this->tape_scroll_right->width()-1;
    int bounding_rect_width = bounding_rect_right_end - bounding_rect_left_begin;
    int bounding_rect_height = this->height();
    this->tape_bounding_rect->setLeft(bounding_rect_left_begin);
    this->tape_bounding_rect->setTop(0);
    this->tape_bounding_rect->setWidth(bounding_rect_width);
    this->tape_bounding_rect->setHeight(bounding_rect_height);

    int tape_topline_y = this->height() / 2;
    this->tape_cell_height = this->height() / 2;
    this->tape_cell_width = tape_cell_height;
    int tape_cells_visible_count = this->tape_bounding_rect->width() / tape_cell_width;

    this->tape_rects->clear();
    QRect tmp_rect;
    for (int i=0;i<tape_cells_visible_count;i++){
        tmp_rect.setLeft( i*tape_cell_width+this->tape_bounding_rect->left() );
        tmp_rect.setTop( tape_topline_y );
        tmp_rect.setWidth(tape_cell_width);
        tmp_rect.setHeight(tape_cell_height);
        this->tape_rects->insert(i,tmp_rect);
    };
    QRect last_tmp = (*this->tape_rects)[this->tape_rects->size()-1];
    QRect half_rect =
            QRect( last_tmp.right(), tape_topline_y,
                   this->tape_bounding_rect->right()-last_tmp.right(),
                   tape_cell_height
                 );
    this->tape_rects->insert(this->tape_rects->size(),half_rect);
    this->tape_visible_length = this->tape_rects->size();
    //------------- каретка --------------------------------------------------------
    int carriage_rect_position = this->tape_rects->size() / 2;
    this->carriage_tape_position = this->map_visible_to_tape(carriage_rect_position);
    QRect carriage_stands_on_rect = (*this->tape_rects)[carriage_rect_position];
    this->carriage_polygon->clear();
    this->carriage_polygon->append( QPoint(carriage_stands_on_rect.left()+5,carriage_stands_on_rect.top()-5));
    this->carriage_polygon->append( QPoint(carriage_stands_on_rect.left(),carriage_stands_on_rect.top()-15));
    this->carriage_polygon->append( QPoint(carriage_stands_on_rect.left(),0));
    this->carriage_polygon->append( QPoint(carriage_stands_on_rect.right(),0));
    this->carriage_polygon->append( QPoint(carriage_stands_on_rect.right(),carriage_stands_on_rect.top()-15));
    this->carriage_polygon->append( QPoint(carriage_stands_on_rect.right()-5,carriage_stands_on_rect.top()-5)); 
    //------------------------------------------------------------------------------
}

void Post_Tape::init_tape()
{
    for (int i=this->tape_size_from;i<=this->tape_size_to;i++){
        this->tape->insert(i,0);
    };
}

void Post_Tape::slot_on_left_clicked()
{
    this->command_tape_left();
}

void Post_Tape::slot_on_right_clicked()
{
    this->command_tape_right();
}

int Post_Tape::get_tape_visible_length() const
{
    return tape_visible_length;
}

int Post_Tape::map_visible_to_tape(int rect_index)
{
    return this->tape_visible_start + rect_index;
}

int Post_Tape::map_tape_to_visible(int cell_index)
{
    if ( cell_index<this->tape_visible_start )
        return INT_MIN;
    else if ( cell_index > (this->tape_visible_start+this->tape_visible_length) )
        return INT_MIN;
    else
        return cell_index - this->tape_visible_start;
}

bool Post_Tape::command_tape_left()
{
    this->tape_visible_start --;
    this->init_cell_rects();
    this->repaint();

    return true;
}

bool Post_Tape::command_tape_right()
{
    this->tape_visible_start ++;
    this->init_cell_rects();
    this->repaint();

    return true;
}

bool Post_Tape::command_tape_set()
{
    if ( (*this->tape)[this->carriage_tape_position]==1 )
        return false;
    else{
        (*this->tape)[this->carriage_tape_position] = 1;
        this->init_cell_rects();
        this->repaint();
        return true;
    };
}

bool Post_Tape::command_tape_erase()
{
    if ( (*this->tape)[this->carriage_tape_position]!=1 )
        return false;
    else{
        (*this->tape)[this->carriage_tape_position]=0;
        this->init_cell_rects();
        this->repaint();
        return true;
    };
}

bool Post_Tape::command_tape_read(bool &have_mark)
{
    have_mark = ( (*this->tape)[this->carriage_tape_position]==1 );
    return true;
}


int Post_Tape::get_tape_visible_start() const
{
    return tape_visible_start;
}

void Post_Tape::set_tape_visible_start(int new_tape_visible_start)
{
    tape_visible_start = new_tape_visible_start;
}

int Post_Tape::get_visible_rect_index_by_xy(int x, int y)
{
    for (int i=0;i<this->tape_rects->size();i++){
        QRect r = (*this->tape_rects)[i];
        if ( r.contains(x,y) )
            return i;
    };
    return INT_MIN;
}

void Post_Tape::mouseDoubleClickEvent(QMouseEvent *event)
{
    int indx = this->get_visible_rect_index_by_xy( event->x(), event->y() );
    if ( indx>INT_MIN ){
        if ( (*this->tape)[this->map_visible_to_tape(indx)]!=1 )
            (*this->tape)[this->map_visible_to_tape(indx)] = 1;
        else if ( (*this->tape)[this->map_visible_to_tape(indx)]==1 )
            (*this->tape)[this->map_visible_to_tape(indx)] = 0;
        this->repaint();
    };
}

void Post_Tape::keyPressEvent(QKeyEvent *event)
{

}

void Post_Tape::paintEvent(QPaintEvent *event)
{
    // настраиваем отрисовщик
    QPainter painter(this);
    QBrush old_brush = painter.brush();
    QPen   old_pen = painter.pen();
    QBrush tape_brush(Qt::white);
    painter.setBrush(tape_brush);
    QPen tape_cell_pen(Qt::black);
    tape_cell_pen.setWidth(1);
    painter.setPen(tape_cell_pen);


    // отрисовываем ячейки
    painter.save();
    for (int i=0;i<this->tape_rects->size();i++){
        QRect r = (*this->tape_rects)[i];
        painter.drawRect( r );
        int tape_index = this->map_visible_to_tape( i );
        if ( (*this->tape)[tape_index]==1 ){
            painter.save();
            QPen p = painter.pen();
            p.setWidth(2);
            painter.setPen(p);
            painter.translate(r.center());
            painter.drawLine( -8, -8,  0,  8 );
            painter.drawLine(   0, 8,  8, -8 );
            painter.restore();
        };
    };
    painter.restore();


    // отрисовываем каретку
    painter.drawPolygon(*this->carriage_polygon);

    ///tape_cell_pen.setColor(Qt::red);
    ///painter.setPen(tape_cell_pen);
    ///painter.drawRect(*this->tape_bounding_rect);

    // отрисовываем номера ячеек
    QFont font = painter.font();
    font.setPixelSize(14);
    painter.setFont(font);
    for (int i=0;i<this->tape_visible_length;i++){
        QRect text_rect = QRect( (*this->tape_rects)[i].left(), 0,
                                 (*this->tape_rects)[i].width(), (*this->tape_rects)[i].top() );
        int x = text_rect.left();
        int y = text_rect.bottom();
        painter.save();
        painter.translate( x+12,y-5 );
        if ( abs(this->map_visible_to_tape(i))>99 )
            painter.rotate(-45.0);
        painter.drawText(0,0, QString::number( this->map_visible_to_tape(i) ) );
        painter.restore();
    };

}

void Post_Tape::resizeEvent(QResizeEvent *event)
{
    this->tape_scroll_left->resize(this->tape_scroll_left->width(),this->height());
    this->tape_scroll_right->resize(this->tape_scroll_right->width(),this->height());
    this->init_cell_rects();
    this->repaint();
}

