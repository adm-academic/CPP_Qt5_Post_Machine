#ifndef FILE_LAYER_H
#define FILE_LAYER_H

#include <QObject>
#include <post_program.h>
#include <post_tape.h>
#include <QPlainTextEdit>

class File_Layer : public QObject
{
    Q_OBJECT
public:
    const QString file_layer_version = "1.0";
    explicit File_Layer(QWidget *parent,
                        QPlainTextEdit* pte_task,
                        Post_Tape* post_tape,
                        Post_Program* post_program );
    virtual ~File_Layer();

    void dialog_save_to_file();
    void dialog_load_from_file();
    void dialog_new_program();

protected:
    void save_to_file(QString filename);
    void load_from_file(QString filename);
    void new_program();
    QWidget*        wparent=nullptr;
    QPlainTextEdit* pte_task=nullptr;
    Post_Tape*      post_tape=nullptr;
    Post_Program*   post_program=nullptr;

signals:

};

#endif // FILE_LAYER_H


