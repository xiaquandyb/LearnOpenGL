#-------------------------------------------------
#
# Project created by QtCreator 2018-10-17T16:28:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h


## 修改默认配置
#TARGET = 06HelloTransform
CONFIG(debug, debug|release){
    TARGET = 06HelloTransformd
}else{
    TARGET = 06HelloTransform
}

## 通用配置
QT += opengl
INCLUDEPATH += ../include

# i386对应Win32
# x86_64对应x64
contains(QT_ARCH, i386){
    DESTDIR = ../bin/Win32
}else{
    DESTDIR = ../bin/x64
}

contains(QT_ARCH, i386){
    LIBS += -L../lib/Win32 -lglew32
}else{
    LIBS += -L../lib/x64 -lglew32
}
LIBS += -lopengl32
