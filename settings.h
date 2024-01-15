// Класс объекта настроек всего приложения и отдельных окон
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QTranslator>

// объект доступа к конфигурации программы
class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

public:
    QString get_application_locale();
    void    set_application_locale(QString locale);

public:
    // возвращает истину если файл существует
    bool    file_exists(QString path);
    // находит папку с исходниками и переходит туда
    void    find_basedir_and_changedir();
    // загружает текущую указанную в конфиг-файле локаль
    void    load_current_config_file_locale();
private:
    QTranslator translator;
signals:

};

#endif // SETTINGS_H
