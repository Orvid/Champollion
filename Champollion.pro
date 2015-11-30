
TEMPLATE = subdirs
CONFIG += ordered

DEFINES += _CRT_SECURE_NO_WARNINGS

SUBDIRS += \
    Pex \
    Decompiler \
    Champollion

Champollion.depends = Decompiler Pex
Decompiler.depends = Pex

INCLUDEPATH += F:\Software\boost_1_58_0
INCLUDEPATH += F:\Qt\5.5\msvc2013_64\include
