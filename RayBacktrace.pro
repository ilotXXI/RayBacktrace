#-------------------------------------------------
#
# Project created by QtCreator 2016-06-15T22:27:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RayBacktrace
TEMPLATE = app

SOURCES += main.cpp\
    MainWindow.cpp \
    Polygon.cpp \
    SpotLight.cpp \
    RenderUtilities.cpp \
    AddPolygonDialog.cpp \
    AddLightDialog.cpp

HEADERS  += MainWindow.h \
    Polygon.h \
    Point.h \
    Line.h \
    SpotLight.h \
    RenderUtilities.h \
    AddPolygonDialog.h \
    Rgb.h \
    AddLightDialog.h

FORMS    += MainWindow.ui \
    AddPolygonDialog.ui \
    AddLightDialog.ui

DISTFILES += \
    Unit1.cpp \
    Unit3.cpp \
    Unit4.cpp \
    Unit5.cpp \
    Unit1.h \
    Unit3.h \
    Unit4.h \
    Unit5.h
