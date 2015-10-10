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

int main(int argc, char* argv[])
{
   /* Changed for Qt5, 2015-09-10
    * QApplication::setGraphicsSystem() is removed, because the introduction of QPA made it redundant.
    * -----------
    * QPA: Qt Platform Abstraction , http://doc.qt.io/qt-5/qpa.html
    */
   #if QT_VERSION < 0x050000
   QApplication::setGraphicsSystem("raster");
   #endif
   QApplication app(argc, argv);

   app.setApplicationName("SlidingWindow");
   app.setApplicationVersion("0.1");

   SlidingWindow *slidingWindow = new SlidingWindow();
   slidingWindow->move(QPoint(START_POS_X, START_POS_Y));
   slidingWindow->show();

   return app.exec();		
}
