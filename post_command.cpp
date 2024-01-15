#include "post_command.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QStringList>
#include <QDebug>
#include <QListWidgetItem>
#include <QKeyEvent>


Post_Command::Post_Command(QWidget *parent)
    : QLineEdit{parent}
{
    if ( parent != nullptr ){
        this->list_widget = new Post_Command_List( parent );
        this->list_widget->move( this->x(), this->y() );
        QStringList sl=QStringList() << "" << "left" << "right" << "set" << "erase"
                                     << "if" << "!";
        this->list_widget->addItems(sl);
        this->list_widget->hide();
        this->list_widget->setStyleSheet( "border-style:solid; border-width:2px; border-color:black; "
                                          "background-color:white; " );

        this->command = this->get_selected_command();
        connect( this->list_widget, SIGNAL(change_command(QString)),
                 this, SLOT(slot_change_command(QString)) );

        this->setReadOnly(true);
        this->setStyleSheet(" border-width:0px; border-color: white;  ");
    };
}

Post_Command::~Post_Command()
{

}

QString Post_Command::get_selected_command()
{
   if ( this->list_widget->currentRow()==-1 ) {
        return "";
   }
   else{
        return this->list_widget->currentItem()->text();
   };
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

    //p.translate(10,0);

    QRect r = this->rect();

    p.drawText( r, Qt::AlignRight | Qt::AlignVCenter , this->command );



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

}

void Post_Command::show_list()
{
    this->list_widget->setParent( this->parentWidget() );
    this->list_widget->move( this->pos() );

    this->list_widget->resize( this->width(), 175 );

    if (  this->parentWidget()->height()< (this->list_widget->pos().y()+this->list_widget->height()) ){
        QPoint qp=this->list_widget->pos();
        qp.setY(this->parentWidget()->height()-this->list_widget->height());
        this->list_widget->move(qp);
    };

    this->list_widget->show();
    this->list_widget->setFocus(Qt::PopupFocusReason);
}

void Post_Command::slot_change_command(QString new_command)
{
    this->command = new_command;
}

Post_Command_List::Post_Command_List(QWidget *parent) : QListWidget(parent)
{
    connect( this, SIGNAL(itemClicked(QListWidgetItem*)),
             this, SLOT(hide())
           );

}

Post_Command_List::~Post_Command_List()
{

}

void Post_Command_List::leaveEvent(QEvent *event)
{
    this->hide();
    QListWidget::leaveEvent(event);
}

void Post_Command_List::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    emit change_command( this->itemFromIndex(current)->text() );
    QListWidget::currentChanged( current, previous );
}

void Post_Command_List::keyPressEvent(QKeyEvent *event)
{

}




