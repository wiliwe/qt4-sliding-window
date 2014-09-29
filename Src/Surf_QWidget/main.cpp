
#include "SlidingWindow.h"

#include <QApplication>

/* The initial position of sliding window. */
#define START_POS_X 120
#define START_POS_Y  25

int main(int argc, char* argv[])
{
   QApplication::setGraphicsSystem("raster");
   QApplication app(argc, argv);

   app.setApplicationName("SlidingWindow");
   app.setApplicationVersion("0.1");

   SlidingWindow *slidingWindow = new SlidingWindow();
   slidingWindow->move(QPoint(START_POS_X, START_POS_Y));
   slidingWindow->show();

   return app.exec();		
}
