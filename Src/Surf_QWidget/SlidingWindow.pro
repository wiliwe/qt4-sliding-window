# #####################################################################
# Automatically generated by qmake (2.01a) 6/22 10:42:22 2010
# #####################################################################
TEMPLATE = app
TARGET = SlidingWindow

DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += SlidingWindow.h \
           Apps/DigiFlip/digiflip.h \
           Apps/DigitalClock/digitalclock.h \
           Apps/AnalogClock/analogclock.h \
           Apps/Wiggly/wigglywidget.h \
           Apps/Wiggly/dialog.h \
           Apps/ParallaxHome/parallaxhome.h

FORMS += SlidingWindowForm.ui

SOURCES += main.cpp \
           SlidingWindow.cpp \
           Apps/DigiFlip/digiflip.cpp \
           Apps/DigitalClock/digitalclock.cpp \
           Apps/AnalogClock/analogclock.cpp \
           Apps/Wiggly/wigglywidget.cpp \
           Apps/Wiggly/dialog.cpp \
           Apps/ParallaxHome/parallaxhome.cpp

RESOURCES = Apps/ParallaxHome/parallaxhome.qrc

DEFINES += ENABLE_TRANSPARENT_FRAME

QT += gui
QT += svg
# Added for Qt5, 2015-09-10. Begin
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# Added for Qt5, 2015-09-10. End

