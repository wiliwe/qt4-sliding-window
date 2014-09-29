/*! \file SlidingWindow.h
    \brief Declaration of SlidingWindow class.

    \author William.L
    \date 2010-06-08
    \version 1.0

    \b Change History
    \n 2010-06-08  William.L initialized.
 */

#ifndef SLIDINGWINDOW_H
#define SLIDINGWINDOW_H

#include <QtGui>

#include "ui_SlidingWindowControl.h"
#include "FlickCharm/flickcharm.h"
//
#include "Apps/DigiFlip/digiflip.h"
#include "Apps/DigitalClock/digitalclock.h"
#include "Apps/AnalogClock/analogclock.h"
#include "Apps/Wiggly/dialog.h"
#include "Apps/ParallaxHome/parallaxhome.h"

/*! \class SlidingWindow
    \brief The sliding window class.
*/
class SlidingWindow : public QGraphicsView
{
Q_OBJECT

public:
    SlidingWindow();  /*!< Constructor */

private:
    QGraphicsScene scene;  /*!< The canvas */
    QGraphicsItem *background;  /*!< The sliding window background picture. */
    QList<QWidget*> slideItems;  /*!< The list of items of QGraphicsItem type.  */
    QList<QGraphicsProxyWidget*> sceneProxyWidgets;

    #ifdef USE_CONTROL_PANEL
    QTimeLine m_timelineItem;  /*!< The timeline for sliding items. */
    Ui::ControlPanelForm m_slideControl;  /*!< The form constructed from UI designer. */
    int m_nNewPosOfFirstItem;  /*!< The current starting position of X-axis. */
    #endif

    #ifndef USE_CONTROL_PANEL
    FlickCharm m_flickcharm;
    #endif

    /* QWidget/App items */
    DigiFlip     *m_flipClock;
    DigitalClock *m_digitClock;
    AnalogClock  *m_analogClock;
    WigglyText   *m_wigglyText;
    ParallaxHome *m_parallaxHome;

public:
    #ifdef USE_CONTROL_PANEL
    void slideBy(int distanceX);
    #endif
    void addWidgetItem(QWidget* widget);

protected:
    void addDefaultWidgetItems();  /*!< Add default widges into sliding window */

public slots:
    #ifdef USE_CONTROL_PANEL
    /* The event handlers for sliding item event. */
    void slideLeft();
    void slideRight();
    #endif

private slots:
    #ifdef USE_CONTROL_PANEL
    void moveItem(int nOffset);  /*!< For moving items. */
    void moveItemFinish();
    #endif
};
#endif /* SlidingWindow_H */
