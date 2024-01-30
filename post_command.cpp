#include "post_command.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QStringList>
#include <QDebug>
#include <QListWidgetItem>
#include <QKeyEvent>
#include <QWheelEvent>


Post_Command::Post_Command(QWidget *parent)
    : QLineEdit{parent}
{
    if ( parent != nullptr ){
        this->list_widget = new Post_Command_List( parent );
        this->list_widget->move( this->x(), this->y() );

        this->setReadOnly(true);
        this->setStyleSheet(" border-width:0px; border-color: white;  ");
    };
}

Post_Command::~Post_Command()
{

}

QString Post_Command::get_selected_command()
{
    return this->list_widget->get_selected_command();
}

void Post_Command::set_selected_command(QString command)
{
    this->list_widget->set_selected_command( command );
    this->update();
}


void Post_Command::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

    QPainter p(this);

    QBrush br;
    br.setColor(Qt::red);
    p.setBrush(br);

    QPen pn;
    pn.setColor( Qt::black );
    pn.setStyle( Qt::SolidLine );
    p.setPen(pn);



    QPixmap pxm=this->list_widget->get_selected_icon().pixmap(this->list_widget->icon_size ,
                                                              this->list_widget->icon_size );
    QRect pxm_target_rect=pxm.rect();
    pxm_target_rect.setTop( this->height()/2 - pxm.rect().height()/2 );
    pxm_target_rect.setLeft( this->icon_padding );
    pxm_target_rect.setHeight( this->list_widget->icon_size );
    pxm_target_rect.setWidth( this->list_widget->icon_size );
    p.drawPixmap( pxm_target_rect, pxm, pxm.rect() );

    QRect r = this->rect();
    r.setLeft( this->icon_padding + this->list_widget->icon_size + this->icon_padding );
    p.drawText( r, Qt::AlignLeft | Qt::AlignVCenter , this->list_widget->get_selected_command() );
}

void Post_Command::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void Post_Command::mousePressEvent(QMouseEvent *event)
{
    this->show_list();
}

void Post_Command::keyPressEvent(QKeyEvent *event)
{
    this->show_list();
}

void Post_Command::show_list()
{
    this->list_widget->setParent( this->parentWidget() );
    this->list_widget->move( this->pos() );

    this->list_widget->resize( this->width()+50, 200 );

    if (  this->parentWidget()->height()< (this->list_widget->pos().y()+this->list_widget->height()) ){
        QPoint qp=this->list_widget->pos();
        qp.setY(this->parentWidget()->height()-this->list_widget->height());
        this->list_widget->move(qp);
    };

    this->list_widget->show();
    this->list_widget->setFocus(Qt::PopupFocusReason);
}

Post_Command_List::Post_Command_List(QWidget *parent) : QListWidget(parent)
{
    this->setIconSize( QSize(this->icon_size , this->icon_size) );


    QListWidgetItem* empty= new QListWidgetItem("");
    this->addItem(empty);
    QListWidgetItem* left= new QListWidgetItem("left");
    left->setIcon( QIcon(":/arrow_l.png") );
    this->addItem(left);
    QListWidgetItem* right = new QListWidgetItem("right");
    right->setIcon( QIcon(":/arrow_r.png") );
    this->addItem(right);
    QListWidgetItem* mark = new QListWidgetItem("mark");
    mark->setIcon( QIcon(":/mark.png") );
    this->addItem(mark);
    QListWidgetItem* erase = new QListWidgetItem("erase");
    erase->setIcon( QIcon(":/erase.png") );
    this->addItem(erase);
    QListWidgetItem* condition = new QListWidgetItem("condition");
    condition->setIcon( QIcon(":/condition.png") );
    this->addItem( condition );
    QListWidgetItem* stop = new QListWidgetItem("stop");
    stop->setIcon(  QIcon(":/stop.png")  );
    this->addItem(stop);

    connect( this, SIGNAL(itemClicked(QListWidgetItem*)),
             this, SLOT(hide())
           );

    this->hide();

}

Post_Command_List::~Post_Command_List()
{

}

QString Post_Command_List::get_selected_command()
{
    if ( this->currentRow()==-1 ) {
         return "";
    }
    else{
         return this->currentItem()->text();
    };
}

void Post_Command_List::set_selected_command(QString command)
{
    QList<QListWidgetItem*> list = this->findItems( command, Qt::MatchExactly );
    if (list.count() == 1){
        list[0]->setSelected(true);
        this->setCurrentItem( list[0] );
    };
    this->update();
}

QIcon Post_Command_List::get_selected_icon()
{
    if ( this->currentRow()>0 and this->currentRow()<this->count() )
        return this->currentItem()->icon();
    else
        return QIcon();
}



void Post_Command_List::leaveEvent(QEvent *event)
{
    this->hide();
    QListWidget::leaveEvent(event);
}

void Post_Command_List::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{

    QListWidget::currentChanged( current, previous );
}

void Post_Command_List::keyPressEvent(QKeyEvent *event)
{

    if ( event->key()==Qt::Key_Up ){
        int new_row = 0;
        if ( this->currentRow() == 0 )
            new_row = this->count()-1;
        else if ( this->currentRow()>0 and this->currentRow()<this->count() )
            new_row = this->currentRow() - 1;
        this->setCurrentRow( new_row );
        return;
    }
    else if ( event->key()==Qt::Key_Down ){
        int new_row = 0;
        if ( this->currentRow() == this->count()-1  )
            new_row = 0;
        else if ( this->currentRow()>=0 and this->currentRow()<this->count()-1 )
            new_row = this->currentRow() + 1;
        this->setCurrentRow( new_row );
        return;
    };

    if ( event->key() == Qt::Key_Escape
         or event->key() == Qt::Key_Enter
         or event->key()==Qt::Key_Return )
        this->hide();

}

void Post_Command_List::wheelEvent(QWheelEvent *event)
{
    int new_row = 0;
    if( event->delta() > 0)
    {
        if ( this->currentRow() == 0 )
            new_row = this->count()-1;
        else if ( this->currentRow()>0 and this->currentRow()<this->count() )
            new_row = this->currentRow() - 1;
    }
    else{
        if ( this->currentRow() == this->count()-1  )
            new_row = 0;
        else if ( this->currentRow()>=0 and this->currentRow()<this->count()-1 )
            new_row = this->currentRow() + 1;
    };

    this->setCurrentRow( new_row );

}




