#include "file_layer.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>


File_Layer::File_Layer(QWidget *parent, QPlainTextEdit *pte_task,
                       Post_Tape *post_tape, Post_Program *post_program)
: QObject{parent}
{
    this->wparent = parent;
    this->pte_task = pte_task;
    this->post_tape = post_tape;
    this->post_program = post_program;
}

File_Layer::~File_Layer()
{

}

void File_Layer::dialog_save_to_file()
{
    QString file_name = QFileDialog::getSaveFileName( this->wparent,
             tr("Save program of Post Machine"), "",
             tr("Post Program (*.pstj);;All Files (*)") );
    if (file_name.isEmpty())
             return;
    else {
        QFileInfo f( file_name );
        if (f.suffix().isEmpty())
        {
            file_name += ".pstj";
        };
        this->save_to_file(file_name);
    }
}

void File_Layer::dialog_load_from_file()
{
    QString file_name = QFileDialog::getOpenFileName( this->wparent,
                                tr("Load program of Post Machine"),
                                "",
                                tr("Post Program (*.pstj);;All Files (*)") );
    if (file_name.isEmpty())
             return;
    else {
        this->load_from_file(file_name);
    };
}

void File_Layer::dialog_new_program()
{
    QMessageBox msgBox;
    msgBox.setText(tr("New program."));
    msgBox.setInformativeText("Create a new program and clean up the existing one?");
    msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
    int ret = msgBox.exec();
    if ( ret==QMessageBox::Yes ){
        this->new_program();
    }
    else if ( ret == QMessageBox::No ){
        return;
    };
}

void File_Layer::save_to_file(QString filename)
{
    qDebug() << "saving to file:" << filename;
    //
    QJsonObject json_all;
    //--- Serialize "File Layer Version" ---
    json_all.insert( "file_layer_version", QJsonValue::fromVariant( this->file_layer_version ) );
    //--- Serialize Task -----------
    json_all.insert( "task", QJsonValue::fromVariant(this->pte_task->toPlainText()) );
    //--- Serialize Tape -----------
    QJsonObject json_tape_marks;
    for( int i=this->post_tape->tape_size_from; i<this->post_tape->tape_size_to; i++ ){
        short marked = 0;
        this->post_tape->get_fast_tape_cell(i,marked);
        if (marked==1){
            json_tape_marks.insert( QString::number(i), QJsonValue::fromVariant(marked) );
        };
    };
    //--- Serialize Carriage Position ---
    json_all.insert( "carriage_position", QJsonValue::fromVariant( this->post_tape->get_carriage_position() ) );
    //--- Serialize Program --------
    json_all.insert( "program_length", QJsonValue::fromVariant( this->post_program->rowCount() ) );
    QJsonArray json_program;
    for (int i=0; i<this->post_program->rowCount();i++){
        QJsonObject json_program_row;
        json_program_row.insert( "command",   QJsonValue::fromVariant(
                                     this->post_program->get_cell_string(i,0)) );
        json_program_row.insert( "reference", QJsonValue::fromVariant(
                                     this->post_program->get_cell_string(i,1)) );
        json_program_row.insert( "comment",   QJsonValue::fromVariant(
                                     this->post_program->get_cell_string(i,2)) );
        json_program.push_back(json_program_row);
    };
    json_all.insert("program",json_program);
    //--- Output Serialized Data ---
    json_all.insert("tape",json_tape_marks);
    QJsonDocument json_doc(json_all);
    QString json_string = json_doc.toJson(QJsonDocument::Indented);
    //--- Write Json Text Data -----
    QFile file;
    file.setFileName( filename );
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream( &file );
    stream << json_string;
    file.close();
}

void File_Layer::load_from_file(QString filename)
{
    //--- Clear Current All Post Programm Data -----
    this->pte_task->clear();
    this->post_tape->clear();
    this->post_program->clear();
    qDebug() << "loading from file:" << filename;
    //--- Read Json Text Data -----
    QString text_json;
    QFile file;
    file.setFileName( filename );
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    text_json = file.readAll();
    file.close();
    //--- Primary Parsing JSON ----
    QJsonDocument json_doc = QJsonDocument::fromJson( text_json.toUtf8() );
    QJsonObject json_tree = json_doc.object();
    //--- Read and Check File Loader (Layer) Version
    QString layer_verion = json_tree["file_layer_version"].toString();
    if ( layer_verion != this->file_layer_version ){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("File Load Error!"));
        msgBox.setText( "Error! Incompatable Version of '*.pstj' file!" );
        msgBox.exec();
        return;
    };
    //--- Read Task ---------------
    QString task = json_tree["task"].toString();
    this->pte_task->setPlainText(task);
    //--- Read Tape ---------------
    QJsonValue tmp = json_tree.value("tape");
    if (tmp.isObject())
    {
        QJsonObject json_tape = tmp.toObject();
        foreach(const QString& key, json_tape.keys()) {
            QJsonValue value = json_tape.value(key);
            int   cell_index = key.toInt();
            short cell_marked = (short)value.toInt();
            this->post_tape->set_fast_tape_cell(cell_index,cell_marked);
        };
    };
    //--- Read Carriage Position ---
    int carriage_position = json_tree["carriage_position"].toInt();
    this->post_tape->set_carriage_position(carriage_position);
    //--- Read Program -------------
    int program_length = json_tree["program_length"].toInt();
    if ( program_length<1 ){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("File Load Error!"));
        msgBox.setText( "Error! Bad 'program_length' in '*.pstj' file!" );
        msgBox.exec();
        return;
    };
    this->post_program->setRowCount(program_length);
    QJsonValue tmp2 = json_tree.value("program");
    if (tmp2.isArray())
    {
        QJsonArray json_program = tmp2.toArray();
        if ( program_length != json_program.count() ){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("File Load Error!"));
            msgBox.setText( "Error! Wrong length of program in '*.pstj' file!" );
            msgBox.exec();
            return;
        };
        for (int i=0;i<json_program.count();i++){
            QJsonValue tmp3 = json_program[i];
            QJsonObject json_program_row = tmp3.toObject();

            this->post_program->set_cell_string(i,0,json_program_row["command"].toString());
            this->post_program->set_cell_string(i,1,json_program_row["reference"].toString());
            this->post_program->set_cell_string(i,2,json_program_row["comment"].toString());
        };
    };
    //--- Repaint Window -------------
    this->wparent->update();
}

void File_Layer::new_program()
{
    this->pte_task->clear();
    this->post_tape->clear();
    this->post_program->clear();
}
