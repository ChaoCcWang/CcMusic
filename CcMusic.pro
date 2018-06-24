#-------------------------------------------------
#
# Project created by QtCreator 2018-06-05T13:07:08
#
#-------------------------------------------------

QT      += core gui xml
QT += multimedia multimediawidgets
CONFIG  +=  c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CcMusic
TEMPLATE = app

RC_FILE = uac.rc

SOURCES += main.cpp\
        CcMusic.cpp \
    TitleBar.cpp \
    BottomBar.cpp \
    MusicPlaylist.cpp \
    PlayerBody.cpp \
    CloseWnd.cpp \
    ProcessSlider.cpp \
    VolumeSlider.cpp

HEADERS  += CcMusic.h \
    TitleBar.h \
    BottomBar.h \
    MusicPlaylist.h \
    PlayerBody.h \
    CloseWnd.h \
    ProcessSlider.h \
    VolumeSlider.h

FORMS    += CcMusic.ui

RESOURCES += \
    res.qrc
