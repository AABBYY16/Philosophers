TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        additionalfunctions.cpp \
        fork.cpp \
        main.cpp \
        philosopher.cpp \
        statusprinter.cpp

DISTFILES += \
    README.md

HEADERS += \
    additionalfunctions.h \
    fork.h \
    philosopher.h \
    statusprinter.h

QMAKE_CXXFLAGS += -std=c++0x -pthread 
LIBS += -pthread
LIBS += -lncurses