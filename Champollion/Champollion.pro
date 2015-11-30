TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../Project.pri)

SOURCES += main.cpp
INCLUDEPATH += $$PWD/..

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Decompiler/release/ -lDecompiler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Decompiler/debug/ -lDecompiler
else:unix: LIBS += -L$$OUT_PWD/../Decompiler/ -lDecompiler


DEPENDPATH += $$PWD/../Decompiler

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Pex/release/ -lPex
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Pex/debug/ -lPex
else:unix: LIBS += -L$$OUT_PWD/../Pex/ -lPex

DEPENDPATH += $$PWD/../Pex

target.path = $$PREFIX
INSTALLS += target
