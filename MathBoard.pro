#-------------------------------------------------
#
# Project created by QtCreator 2016-07-21T00:08:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MathBoard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qconsolewidget.cpp \
    redirect.cpp \
    gui_kernelbridge.cpp \
    kernel/AbstractSyntaxTree.cpp \
    kernel/ASTBuilder.cpp \
    kernel/ExecuteUnit.cpp \
    kernel/ExpressionToken.cpp \
    kernel/Interpreter.cpp \
    kernel/LexicalAnalyzer.cpp \
    qpaintzone.cpp

HEADERS  += mainwindow.h \
    qconsolewidget.h \
    redirect.h \
    gui_kernelbridge.h \
    kernel/AbstractSyntaxTree.h \
    kernel/ASTBuilder.h \
    kernel/DisplayResult.h \
    kernel/ExecuteUnit.h \
    kernel/ExpressionToken.h \
    kernel/Interpreter.h \
    kernel/LexicalAnalyzer.h \
    qpaintzone.h

FORMS    += mainwindow.ui
