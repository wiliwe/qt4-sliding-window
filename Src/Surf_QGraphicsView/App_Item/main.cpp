/*! \file main.cpp
    \brief

    \author William.L
    \version 1.0
    \date 2010-06-08

    \b Change History
    \n 2010-06-08  William.L initialized.
 */

#include <QApplication>

#include "SlidingWindow.h"

/* The initial position of sliding window. */
#define START_POS_X 120
#define START_POS_Y  25

int main(int argc, char *argv[])
{
    /* Create a QApplication object and set relavant information. */
    QApplication app(argc, argv);

    app.setApplicationName("Sliding Window");
    app.setApplicationVersion("1.0");

    /* Create a sliding window object. */
    SlidingWindow *slidingWindow = new SlidingWindow();

    /* Let QGraphicsView object be visible. */
    //slidingWindow->showFullScreen();
    slidingWindow->move(QPoint(START_POS_X, START_POS_Y));
    slidingWindow->show();

    return app.exec();
}
