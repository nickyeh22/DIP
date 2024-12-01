QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#INCLUDEPATH += C:\DIP\opencv\build\include\
#               C:\DIP\opencv\build\include\opencv\
#               C:\DIP\opencv\build\include\opencv2\

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    complex.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    complex.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    HW4_zh_TW.ts
CONFIG += lrelease
CONFIG += embed_translations



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/build/x64/vc15/lib/ -lopencv_world3411
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/build/x64/vc15/lib/ -lopencv_world3411d
else:unix: LIBS += -L$$PWD/../../opencv/build/x64/vc15/lib/ -lopencv_world3411

INCLUDEPATH += $$PWD/../../opencv/build/include
DEPENDPATH += $$PWD/../../opencv/build/include
