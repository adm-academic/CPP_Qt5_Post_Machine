#include "settings.h"
#include <QSettings>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QApplication>


Settings::Settings(QObject *parent)
    : QObject{parent}
{

}


QString Settings::get_application_locale(){
    QString locale;
    QSettings settings( "settings.conf", QSettings::IniFormat );
    settings.beginGroup( "Locale" );
        locale = settings.value( "Locale", "" ).toString();
    settings.endGroup();
   return locale;
}


void    Settings::set_application_locale(QString locale){
    QSettings settings( "settings.conf", QSettings::IniFormat );
    settings.beginGroup( "Locale" );
       settings.setValue( "Locale", locale );
    settings.endGroup();
}


bool Settings::file_exists(QString path) {
    QFileInfo check_file(path);
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

void Settings::find_basedir_and_changedir(){
    if ( file_exists("settings.conf") )
        return;
    if ( file_exists("../CPP_Qt5_Post_Machine/settings.conf") )
        QDir::setCurrent ( "../CPP_Qt5_Post_Machine/" );
    if ( file_exists("../../CPP_Qt5_Post_Machine/settings.conf") )
            QDir::setCurrent ( "../../CPP_Qt5_Post_Machine/" );
}


void Settings::load_current_config_file_locale(){
    if ( this->get_application_locale()=="en_US" ){
        this->translator.load(nullptr);
        qApp->installTranslator( &(this->translator) );
    }
    else if (this->get_application_locale()=="ru_RU"){
        const QString baseName = "CPP_Qt5_Post_Machine_" + this->get_application_locale() + ".qm";
        this->translator.load(nullptr);
        if ( this->translator.load( baseName ) ) {
            qApp->installTranslator( &(this->translator) );
        };
    }
}
