#-------------------------------------------------
#
# Project created by QtCreator 2013-04-08T19:43:27
#
#-------------------------------------------------

QT       -= core gui

include(../Project.pri)

TARGET = Pex
TEMPLATE = lib

win32:TARGET_EXT=.dll

DEFINES += PEX_LIBRARY

SOURCES += Binary.cpp \
    Header.cpp \
    FileReader.cpp \
    StringTable.cpp \
    DebugInfo.cpp \
    UserFlag.cpp \
    Object.cpp \
    Variable.cpp \
    Value.cpp \
    Property.cpp \
    UserFlagged.cpp \
    Function.cpp \
    State.cpp \
    NamedItem.cpp \
    TypedItem.cpp \
    TypedName.cpp \
    Instruction.cpp \
    DocumentedItem.cpp

HEADERS += Binary.hpp\
    Header.hpp \
    FileReader.hpp \
    StringTable.hpp \
    DebugInfo.hpp \
    UserFlag.hpp \
    Object.hpp \
    Variable.hpp \
    Value.hpp \
    Property.hpp \
    UserFlagged.hpp \
    Function.hpp \
    State.hpp \
    NamedItem.hpp \
    TypedItem.hpp \
    TypedName.hpp \
    Instruction.hpp \
    DocumentedItem.hpp \
    Pex_global.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

target.path = $$PREFIX
INSTALLS += target
