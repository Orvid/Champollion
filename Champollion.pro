
TEMPLATE = subdirs
CONFIG += ordered

DEFINES += _CRT_SECURE_NO_WARNINGS

SUBDIRS += \
    Pex \
    Decompiler \
    Champollion

Champollion.depends = Decompiler Pex
Decompiler.depends = Pex

INCLUDEPATH += L:/Dev/Lib/Boost/include/boost-1_53
