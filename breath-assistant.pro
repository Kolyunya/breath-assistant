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
    sources/BreathAssistant/OverlayBorder.hpp \
    sources/BreathAssistant/OverlayCorner.hpp \
    sources/BreathAssistant/OverlayComponent.hpp \
    sources/BreathAssistant/OverlayWindow.hpp \

SOURCES += \
    sources/main.cpp\
    sources/BreathAssistant/MainWindow.cpp \
    sources/BreathAssistant/OverlayBorder.cpp \
    sources/BreathAssistant/OverlayCorner.cpp \
    sources/BreathAssistant/OverlayComponent.cpp \
    sources/BreathAssistant/OverlayWindow.cpp \

FORMS += \
    forms/MainWindow.ui \

RESOURCES += \
    resources/icons.qrc
