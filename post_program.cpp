#include "post_program.h"
#include "global_forms.h"
#include <QHeaderView>
#include <QDebug>
#include <QList>
#include <QStandardItem>
#include "post_command.h"

void Post_Program::initialize()
{
    this->setRowCount(6);
    this->setColumnCount(3);
    this->setHorizontalHeaderLabels( QStringList() << "Команда" << "Отсылка" << "Комментарий" );
    this->setEditTriggers( QAbstractItemView::AllEditTriggers  );
    this->setSelectionMode( QAbstractItemView::SingleSelection );
    this->setSelectionBehavior( QAbstractItemView::SelectRows );
    this->setGridStyle( Qt::DashLine );
    this->setCornerButtonEnabled( false );
    this->horizontalHeader()->setStretchLastSection( true );

    for(int i=0;i<this->rowCount();i++){
        Post_Command* ppc = new Post_Command(this);
        this->setCellWidget(i,0,ppc);
    };

    connect( this->model(), &QAbstractItemModel::rowsInserted,
             this, &Post_Program::slot_rows_inserted );

    this->selectRow( 0 );

}

Post_Program::Post_Program(QWidget *parent)
    : QTableWidget{parent}
{
    this->initialize();
}

Post_Program::~Post_Program()
{

}


void Post_Program::row_append()
{
    this->insertRow( this->rowCount() );
}

void Post_Program::row_insert()
{
    this->insertRow( this->currentRow() );
}

void Post_Program::row_delete()
{
    if ( this->rowCount() <= 1 )
        return;

    int drow = this->currentRow();

    this->removeRow( drow );

}

void Post_Program::row_move_up()
{
    int old_row = this->currentRow();

    if( old_row <=0 )
        return;

    // запомним данные для переноса
    Post_Command* old_pc = qobject_cast<Post_Command *>( this->cellWidget(old_row,0) );
    QString old_command = old_pc->get_selected_command();
    QString old_next = this->get_cell_string( old_row, 1 );
    QString old_comment = this->get_cell_string( old_row, 2 );

    // перезапишем строку. которую нужно переносить
    int new_row = old_row - 1;
    Post_Command* new_pc = qobject_cast<Post_Command *>( this->cellWidget(new_row,0) );
    old_pc->set_selected_command( new_pc->get_selected_command() );
    QString tmp_next = this->get_cell_string( new_row, 1 );
    QString tmp_comment = this->get_cell_string( new_row, 2 );
    this->setItem( old_row, 1, new QTableWidgetItem( tmp_next ) );
    this->setItem( old_row, 2, new QTableWidgetItem( tmp_comment ) );

    // перенесём данные в целевую строку
    new_pc->set_selected_command( old_command );
    this->setItem( new_row, 1, new QTableWidgetItem( old_next ) );
    this->setItem( new_row, 2, new QTableWidgetItem( old_comment ) );

    this->selectRow(new_row);
}

void Post_Program::row_move_down()
{
    int old_row = this->currentRow();
    if( old_row >= this->model()->rowCount()-1 )
        return;

    // запомним данные для переноса
    Post_Command* old_pc = qobject_cast<Post_Command *>( this->cellWidget(old_row,0) );
    QString old_command = old_pc->get_selected_command();
    QString old_next = this->get_cell_string( old_row, 1 );
    QString old_comment = this->get_cell_string( old_row, 2 );

    // перезапишем строку. которую нужно переносить
    int new_row = old_row + 1;
    Post_Command* new_pc = qobject_cast<Post_Command *>( this->cellWidget(new_row,0) );
    old_pc->set_selected_command( new_pc->get_selected_command() );
    QString tmp_next = this->get_cell_string( new_row, 1 );
    QString tmp_comment = this->get_cell_string( new_row, 2 );
    this->setItem( old_row, 1, new QTableWidgetItem( tmp_next ) );
    this->setItem( old_row, 2, new QTableWidgetItem( tmp_comment ) );

    // перенесём данные в целевую строку
    new_pc->set_selected_command( old_command );
    this->setItem( new_row, 1, new QTableWidgetItem( old_next ) );
    this->setItem( new_row, 2, new QTableWidgetItem( old_comment ) );

    this->selectRow(new_row);

}

QString Post_Program::get_cell_string(int cell_row, int cell_col) // возвращает значение ячейки с учётом
                                                                  // всех сложностей
{
    if ( this->rowCount()<=0 ) // возврат если таблица пустая
        return "";
    if ( this->columnCount()<=0 ) // возврат если таблица пустая
        return "";
    if ( cell_row<0 or cell_row>=this->rowCount() ) // возврат если строка неактуальна
        return "";
    if ( cell_col<0 or cell_col>=this->columnCount() ) // возврат если столбец неактуален
        return "";


    if ( cell_col ==0 ){ // если столбец нулевой - нужно обратиться к внедрённому виджету
        // получим указатель на внедрённый виджет
        Post_Command* pc = qobject_cast<Post_Command *>( this->cellWidget(cell_row,cell_col) );
        return pc->get_selected_command(); // возвратим выбранную команду из внедрённого виджета
    }
    else if ( cell_col==1 or cell_col==2  ){ // если столбец один или два - обработаем пустой указатель
        if ( this->item(cell_row,cell_col) == nullptr ) // если указатель нулевой
            return ""; // вернём пустую строку
        else // иначе, если указатель ненулевой
            return this->item(cell_row,cell_col)->text(); // вернём текстовое значение из ячейки.
    };
}

void Post_Program::paintEvent(QPaintEvent *event)
{
    this->resizeColumnToContents(0);
    QTableWidget::paintEvent(event);
}


void Post_Program::slot_rows_inserted(const QModelIndex &parent, int start, int end)
{

    for ( int i=start; i<(end+1); i++){
        Post_Command* ppc = new Post_Command(this);
        this->setCellWidget(i,0,ppc);
    };

}
