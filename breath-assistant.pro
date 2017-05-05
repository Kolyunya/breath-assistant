TARGET = breath-assistant

TEMPLATE = app

QT += \
    core \
    gui \
    widgets \

CONFIG += \
    c++11 \
    c++14 \
    warn_on \

INCLUDEPATH += \
    sources

HEADERS += \
    sources/BreathAssistant/MainWindow.hpp \
    sources/BreathAssistant/OverlayWindow.hpp \

SOURCES += \
    sources/main.cpp\
    sources/BreathAssistant/MainWindow.cpp \
    sources/BreathAssistant/OverlayWindow.cpp \

FORMS += \
    forms/MainWindow.ui \

RESOURCES += \
    resources/icons.qrc
