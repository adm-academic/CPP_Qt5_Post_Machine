#include "wmain.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "global_forms.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    app_settings = new Settings();
    app_settings->find_basedir_and_changedir();
    app_settings->load_current_config_file_locale();

    wmain = new WMain();
    wmain->show();

    return a.exec();

    delete wmain;
    delete app_settings;
}
