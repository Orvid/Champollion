
QT       -= core gui

include(../Project.pri)

TARGET = Decompiler
TEMPLATE = lib
win32:TARGET_EXT=.dll

DEFINES += DECOMPILER_LIBRARY

SOURCES += \
    OutputWriter.cpp \
    AsmCoder.cpp \
    StreamWriter.cpp \
    PscCoder.cpp \
    Coder.cpp \
    PscDecompiler.cpp \
    Node/Base.cpp \
    Node/Scope.cpp \
    Node/Visitor.cpp \
    Node/BinaryOperator.cpp \
    Node/UnaryOperator.cpp \
    Node/Assign.cpp \
    Node/Cast.cpp \
    Node/CallMethod.cpp \
    Node/Return.cpp \
    Node/ArrayCreate.cpp \
    Node/ArrayLength.cpp \
    Node/Constant.cpp \
    Node/Params.cpp \
    Node/While.cpp \
    Node/IfElse.cpp \
    Node/AssignOperator.cpp \
    PscCodeGenerator.cpp \
    Node/PropertyAccess.cpp \
    Node/ArrayAccess.cpp \
    Node/Copy.cpp \
    Node/IdentifierString.cpp \
    Node/NodeComparer.cpp \
    PscCodeBlock.cpp \
    Node/Declare.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Pex/release/ -lPex
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Pex/debug/ -lPex
else:unix: LIBS += -L$$OUT_PWD/../Pex/ -lPex

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/../Pex

HEADERS += \
    OutputWriter.hpp \
    AsmCoder.hpp \
    StreamWriter.hpp \
    PscCoder.hpp \
    Coder.hpp \
    PscDecompiler.hpp \
    Node/Base.hpp \
    Node/Scope.hpp \
    Node/Visitor.hpp \
    Node/Nodes.hpp \
    Node/BinaryOperator.hpp \
    Node/UnaryOperator.hpp \
    Node/Assign.hpp \
    Node/Cast.hpp \
    Node/CallMethod.hpp \
    Node/Return.hpp \
    Node/ArrayCreate.hpp \
    Node/ArrayLength.hpp \
    Node/Constant.hpp \
    Node/FieldNodeMixin.hpp \
    Node/While.hpp \
    Node/IfElse.hpp \
    Node/WithNode.hpp \
    Node/AssignOperator.hpp \
    Node/PropertyAccess.hpp \
    Node/ArrayAccess.hpp \
    Node/Copy.hpp \
    Node/IdentifierString.hpp \
    Node/NodeComparer.hpp \
    Decompiler_Global.hpp \
    DumpTree.hpp \
    PscCodeGenerator.hpp \
    PscCodeBlock.hpp \
    Node/Declare.hpp

target.path = $$PREFIX
INSTALLS += target
