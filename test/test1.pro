TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += test1.cpp

INCLUDEPATH += $$PWD/../src/

DESTDIR = $$PWD/../bin

CONFIG(debug, debug|release) {
    message('debug defined')
    win32 {
        CONFIG += dll
        DEFINES += BUILDING_DLL
        LIBS += -L$$PWD/../lib/dll/debug -lpssmathparser
        message('win32 defined')
    }

    unix {
        LIBS += -L$$PWD/../lib/so/debug -lpssmathparser
        message('unix defined')
    }
}
else {
    message('release defined')
    win32 {
        LIBS += -L$$PWD/../lib/dll/release -lpssmathparser
        message('win32 defined')
    }

    unix {
        LIBS += -L$$PWD/../lib/so/release -lpssmathparser
        message('unix defined')
    }
}
