#-------------------------------------------------
#
# Проект TRules 2017-02-21T23:29:38
#
#-------------------------------------------------

QT       -= gui

PROJECT = TRules

CONFIG += create_prl
CONFIG += no_install_prl
CONFIG += create_pc
CONFIG += link_pkgconfig
CONFIG += c++11

TARGET = $$PROJECT
TEMPLATE = lib
VERSION  = 0.0.1

DEFINES += TRULES_LIBRARY

SOURCES += $$PWD/src/trules.cpp \
           $$PWD/src/ruleparser.cpp \
           $$PWD/src/variables.cpp

HEADERS += $$PWD/src/trules.h \
           $$PWD/src/trules_global.h \
           $$PWD/src/TRules \
           $$PWD/src/ruleparser.h \
           $$PWD/src/variables.h

DESTDIR = $$PWD/build/lib
MOC_DIR = $$PWD/build/moc
OBJECTS_DIR = $$PWD/build/obj

isEmpty(PREFIX) {
    PREFIX=$$PWD
}

PKG_CONFIG_LIBDIR  = $$PREFIX/share/pkgconfig
PKG_CONFIG_LIBDIR += $$system(pkg-config --variable pc_path pkg-config)

PKGCONFIG += TLogger

unix {
    target.path = $$PREFIX/lib

    # includes
    includes.path = $$PREFIX/include/$$PROJECT
    includes.files += $$PWD/src/trules.h
    includes.files += $$PWD/src/trules_global.h
    includes.files += $$PWD/src/TRules
    includes.files += $$PWD/src/ruleparser.h
    includes.files += $$PWD/src/variables.h

    INSTALLS += target includes

    # pc-file params
    QMAKE_PKGCONFIG_NAME          = $$PROJECT
    QMAKE_PKGCONFIG_FILE          = $$PROJECT
    QMAKE_PKGCONFIG_DESCRIPTION   = The $$PROJECT library
    QMAKE_PKGCONFIG_LIBDIR        = $$target.path
    QMAKE_PKGCONFIG_INCDIR        = $$PREFIX/include
    QMAKE_PKGCONFIG_REQUIRES      =
    QMAKE_PKGCONFIG_DESTDIR       = ../share/pkgconfig #путь берётся относительно установки библиотеки
}
