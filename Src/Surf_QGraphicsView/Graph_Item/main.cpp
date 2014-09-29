/*! \file main.cpp
    \brief The main entry of Sliding Window Qt program.

    \author William.L
    \date 2010-06-28
    \version 1.0
 
    \b Change History
    \n 1) 2010-06-28 William.L initialized.
 */

#include <QApplication>

#include "SlidingWindow.h"

/* The initial position of sliding window. */
#define START_POS_X 120
#define START_POS_Y  25

int main(int argc, char *argv[])
{
    /* Sets the default graphics backend to system, which will be used for on-screen widgets and QPixmaps.
       This function must be called before the QApplication constructor is called. */
    QApplication::setGraphicsSystem("raster");

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





