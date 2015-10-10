######################################################################
# Automatically generated by qmake (2.01a) 7/8 09:30:01 2010
######################################################################

TEMPLATE = app
TARGET = SlidingWindow

DEPENDPATH += . FlickCharm
INCLUDEPATH += . FlickCharm

# Input
HEADERS += SlidingWindow.h \
           FlickCharm/flickcharm.h \
           Apps/DigiFlip/digiflip.h \
           Apps/DigitalClock/digitalclock.h \
           Apps/AnalogClock/analogclock.h \
           Apps/Wiggly/wigglywidget.h \
           Apps/Wiggly/dialog.h \
           Apps/ParallaxHome/parallaxhome.h

FORMS += SlidingWindowControl.ui

SOURCES += main.cpp \
           SlidingWindow.cpp \
           FlickCharm/flickcharm.cpp \
           Apps/DigiFlip/digiflip.cpp \
           Apps/DigitalClock/digitalclock.cpp \
           Apps/AnalogClock/analogclock.cpp \
           Apps/Wiggly/wigglywidget.cpp \
           Apps/Wiggly/dialog.cpp \
           Apps/ParallaxHome/parallaxhome.cpp

RESOURCES = Apps/ParallaxHome/parallaxhome.qrc

DEFINES += ENABLE_TRANSPARENT_FRAME
#DEFINES += USE_CONTROL_PANEL

QT += gui
QT += svg
# Added for Qt5, 2015-09-10. Begin
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# Added for Qt5, 2015-09-10. End
