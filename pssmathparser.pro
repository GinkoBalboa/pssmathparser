#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T13:44:32
#
#-------------------------------------------------

TARGET = pssmathparser
TEMPLATE = lib

DEFINES += PSSMATHPARSER_LIBRARY

VERSION = 1.0.0
DEFINES += VERSION=1.0.0

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain
# version of Qt.

# disables all the APIs deprecated before Qt 6.0.0
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += $$PWD/src/pssmathparser.cpp

# This so you can call .h files like: #include "pssmathparser.h"
INCLUDEPATH += $$PWD/src

HEADERS += $$PWD/src/pssmathparser_global.h \
           $$PWD/src/pssmathparser.h

# CONFIG(debug, debug|release) evaluates to true if CONFIG contains
# "debug" but not "release", or if it contains both "debug" and "release"
# but "release" doesn't appear after the last occurrence of "debug".
# USE THIS TO DETECT DEBUG OR RELEASE
CONFIG(debug, debug|release) {
    message('debug defined')
    win32 {
        CONFIG += dll
        CONFIG += skip_target_version_ext
        DEFINES += BUILDING_DLL
        DESTDIR = $$PWD/lib/dll/debug
        message('win32 defined')
    }

    unix {
        DESTDIR = $$PWD/lib/so/debug
        message('unix defined')
    }
    # NO Optimization
}
else {
    message('release defined')
    win32 {
        CONFIG += dll
        CONFIG += skip_target_version_ext
        DEFINES += BUILDING_DLL
        DESTDIR = $$PWD/lib/dll/release
        message('win32 defined')
    }

    unix {
        DESTDIR = $$PWD/lib/so/release
        message('unix defined')
    }
    # O3 Optimization
    QMAKE_CXXFLAGS += -O3
}
