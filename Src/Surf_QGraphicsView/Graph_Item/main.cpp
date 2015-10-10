/*! \file main.cpp
    \brief The main entry of Sliding Window Qt program.

    \author William.L
    \date 2010-06-28
    \version 1.0
 
    \b Change History
    \n 1) 2010-06-28 William.L initialized.
 */


#include <QtGlobal>  // Added for Qt5, 2015-09-10.
#include <QApplication>

#include "SlidingWindow.h"

/* The initial position of sliding window. */
#define START_POS_X 120
#define START_POS_Y  25

int main(int argc, char *argv[])
{
    /* Changed for Qt5, 2015-09-10
     * QApplication::setGraphicsSystem() is removed, because the introduction of QPA made it redundant.
     * -----------
     * QPA: Qt Platform Abstraction , http://doc.qt.io/qt-5/qpa.html
     */
    #if QT_VERSION < 0x050000
    /* Sets the default graphics backend to system, which will be used for on-screen widgets and QPixmaps.
       This function must be called before the QApplication constructor is called. */
    QApplication::setGraphicsSystem("raster");
    #endif

    /* Create a QApplication object and set relavant information. */
    QApplication app(argc, argv);
    app.setApplicationName("Sliding Window");
    app.setApplicationVersion("1.0");

    /* Create a sliding-window object. */
    SlidingWindow *slidingWindow = new SlidingWindow();

    /* Let QGraphicsView object be visible */
    //slidingWindow->showFullScreen();
    slidingWindow->move(QPoint(START_POS_X, START_POS_Y));
    slidingWindow->show();

    return app.exec();
}





