######################################################################
# Automatically generated by qmake (2.01a) 7/7 08:47:24 2010
######################################################################

TEMPLATE = app
TARGET = SlidingWindow

DEPENDPATH += . FlickCharm
INCLUDEPATH += . FlickCharm

# Input
HEADERS += SlidingWindow.h FlickCharm/flickcharm.h
FORMS += SlidingWindowControl.ui
SOURCES += main.cpp SlidingWindow.cpp FlickCharm/flickcharm.cpp

DEFINES += ENABLE_TRANSPARENT_FRAME
DEFINES += ADD_PAGE_NUM
#DEFINES += USE_CONTROL_PANEL

QT += gui
