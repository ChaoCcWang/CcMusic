cmake_minimum_required(VERSION 3.12)
project(CcMusic_clion)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 打开Qt的Moc功能和ui文件QRC文件处理功能
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH C:/Qt/Qt5.6.3/5.6.3/mingw49_32)

#查找依赖库
find_package(Qt5Widgets)
find_package(Qt5Xml)
find_package(Qt5Multimedia)
find_package(Qt5Multimediawidgets)

QT5_WRAP_UI(UI CcMusic.ui)
QT5_WRAP_CPP(UI_CPP
        CcMusic.h
        BottomBar.h
        CloseWnd.h
        LyricWgt.h
        MusicPlaylist.h
        PlayerBody.h
        ProcessSlider.h
        SongPicture.h
        TitleBar.h
        VolumeSlider.h)

#添加Ui文件
#set(UI_FILES CcMusic.ui)

#添加.rc文件
#set(RC_FILES)

#添加.qrc文件
#set(QRC_FILES)

#代码文件
set(SOURCE_FILES
        CcMusic.h
        main.cpp
        CloseWnd.cpp
        LyricWgt.h
        VolumeSlider.h
        SongPicture.h
        MusicPlaylist.h
        TitleBar.cpp
        BottomBar.cpp
        BottomBar.h
        ProcessSlider.cpp
        CloseWnd.h
        CcMusic.cpp
        ProcessSlider.h
        LyricWgt.cpp
        PlayerBody.h
        TitleBar.h
        MusicPlaylist.cpp
        PlayerBody.cpp
        SongPicture.cpp
        VolumeSlider.cpp

        res.qrc
        )

#添加可执行文件
add_executable(CcMusic WIN32 ${SOURCE_FILES})

#添加链接库
qt5_use_modules(CcMusic Widgets Xml Multimedia Multimediawidgets)