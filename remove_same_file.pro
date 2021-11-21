QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += debug
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    hashthread.cpp \
    main.cpp \
    repeatfileitem.cpp \
    samefilelistwidget.cpp

HEADERS += \
    hashthread.h \
    repeatfileitem.h \
    samefilelistwidget.h

FORMS += \
    repeatfileitem.ui \
    samefilelistwidget.ui

TRANSLATIONS += \
    remove_same_file_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
