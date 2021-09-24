QT       += core gui charts serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    graph/callout.cpp \
    graph/centralwidget.cpp \
    graph/chart.cpp \
    graph/chartview.cpp \
    graph/panelwidget.cpp \
    kalan_tl.cpp \
    log.cpp \
    main.cpp \
    terminal/SettingsDialog.cpp \
    terminal/console.cpp \
    terminal/serial.cpp

HEADERS += \
    graph/callout.h \
    graph/centralwidget.h \
    graph/chart.h \
    graph/chartview.h \
    graph/panelwidget.h \
    graph/range.h \
    kalan_tl.h \
    log.h \
    terminal/SettingsDialog.h \
    terminal/console.h \
    terminal/serial.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    graph/panelwidget.ui \
    kalan_tl.ui \
    terminal/SettingsDialog.ui \
    terminal/console.ui
