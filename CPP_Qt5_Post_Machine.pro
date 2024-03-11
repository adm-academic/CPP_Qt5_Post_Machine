QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    file_layer.cpp \
    global_forms.cpp \
    main.cpp \
    post_command.cpp \
    post_engine.cpp \
    post_program.cpp \
    post_tape.cpp \
    settings.cpp \
    wabout.cpp \
    wmain.cpp

HEADERS += \
    file_layer.h \
    global_forms.h \
    post_command.h \
    post_engine.h \
    post_program.h \
    post_tape.h \
    settings.h \
    wabout.h \
    wmain.h

FORMS += \
    wabout.ui \
    wmain.ui

TRANSLATIONS += \
    CPP_Qt5_Post_Machine_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    TODO.txt \
    settings.conf

RESOURCES += \
    resources.qrc
