#-------------------------------------------------
#
# Project created by QtCreator 2018-12-24T19:12:13
#
#-------------------------------------------------

QT       += core gui charts xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Taav
TEMPLATE = app


#-release -static -accessibility -qt-zlib -qt-libpng -qt-libjpeg -qt-xcb -qt-pcre -qt-freetype -no-glib -no-cups -no-sql-sqlite -no-qml-debug -no-opengl
#-no-egl -no-xinput -no-xinput2 -no-sm -no-icu -nomake examples -nomake tests -skip qtactiveqt -skip qtenginio -skip qtlocation -skip qtmultimedia
#-skip qtserialport -skip qtquick1 -skip qtquickcontrols -skip qtscript -skip qtsensors -skip qtwebkit -skip qtwebsockets -skip qtxmlpatterns -skip qt3d


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
CONFIG -= static

INCLUDEPATH += Ui_src \

SOURCES += \
    Ui_src/dockwidgegraphic.cpp \
    Ui_src/dockwidgetchart.cpp \
    Ui_src/graphic_view_zoom.cpp \
    Ui_src/picfinder.cpp \
    Ui_src/workfoldernode.cpp \
        main.cpp \
    Ui_src/mainwindow.cpp \
    Ui_src/uimaker.cpp \
    Ui_src/pdockwidget.cpp \
    Ui_src/treemaker.cpp \
    Ui_src/textnode.cpp \
    Ui_src/choosenode.cpp \
    Ui_src/node.cpp \
    Ui_src/fieldnode.cpp \
    Ui_src/pathnode.cpp \
    Ui_src/worker.cpp

HEADERS += \
    Ui_src/dockwidgegraphic.h \
    Ui_src/dockwidgetchart.h \
    Ui_src/graphic_view_zoom.h \
    Ui_src/mainwindow.h \
    Ui_src/picfinder.h \
    Ui_src/uimaker.h \
    Ui_src/pdockwidget.h \
    Ui_src/treemaker.h \
    Ui_src/text.h \
    Ui_src/textnode.h \
    Ui_src/choosenode.h \
    Ui_src/node.h \
    Ui_src/fieldnode.h \
    Ui_src/pathnode.h \
    Ui_src/worker.h \
    Ui_src/workfoldernode.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Ui_src/res.qrc
