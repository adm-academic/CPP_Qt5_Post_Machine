#include "settings.h"
#include <QSettings>
#include <QFileInfo>
#include <QFile>
#include <QDir>


Settings::Settings(QObject *parent)
    : QObject{parent}
{

}


QString Settings::application_locale_get(){
    QString locale;
    QSettings settings( "settings.conf", QSettings::IniFormat );
    settings.beginGroup( "Locale" );
        locale = settings.value( "Locale", "" ).toString();
    settings.endGroup();
   return locale;
}


void    Settings::application_locale_set(QString locale){
    QSettings settings( "settings.conf", QSettings::IniFormat );
    settings.beginGroup( "Locale" );
       settings.setValue( "Locale", locale );
    settings.endGroup();
}


bool Settings::fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

void Settings::find_basedir_and_changedir(){
    if ( fileExists("settings.conf") )
        return;
    if ( fileExists("../CPP_Qt5_Post_Machine/settings.conf") )
        QDir::setCurrent ( "../CPP_Qt5_Post_Machine/" );
    if ( fileExists("../../CPP_Qt5_Post_Machine/settings.conf") )
            QDir::setCurrent ( "../../CPP_Qt5_Post_Machine/" );
}


void Settings::load_current_config_file_locale(){
    if ( this->application_locale_get()=="en_US" ){
        this->translator.load(nullptr);
        qApp->installTranslator( &(this->translator) );
    }
    else if (this->application_locale_get()=="ru_RU"){
        const QString baseName = "CPP_Qt5_Post_Machine_" + this->application_locale_get() + ".qm";
        this->translator.load(nullptr);
        if ( this->translator.load( baseName ) ) {
            qApp->installTranslator( &(this->translator) );
        }
        return;
    }
}
