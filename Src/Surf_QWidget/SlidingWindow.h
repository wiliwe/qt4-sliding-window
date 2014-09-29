#ifndef SLIDINGPANELCUST_H
#define SLIDINGPANELCUST_H

#include <QtGui>
#include <QtCore>
#include <QPropertyAnimation>
#include <QDebug>

#include "ui_SlidingWindowForm.h"
//
#include "Apps/DigiFlip/digiflip.h"
#include "Apps/DigitalClock/digitalclock.h"
#include "Apps/AnalogClock/analogclock.h"
#include "Apps/Wiggly/dialog.h"
#include "Apps/ParallaxHome/parallaxhome.h"

class QPropertyAnimation;

/*! \class SlidingWindow
    \brief The sliding window class.
*/
class SlidingWindow : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal slideOffset READ slideOffset WRITE setSlideOffset)

private:
    QWidget *m_slidingSurface; /* e.g. Canvas. */
    QList<QWidget*> m_slidePanelItems;  /* The list of panel items in QWidget type.  */
    QPropertyAnimation *m_animationSlide;
    int m_idxFocusedItem, m_idxOldFocusedItem;
    qreal m_itemSlideOffset;
    Ui::CustSlideControlForm m_slideControl;

    /* QWidget/App items */
    DigiFlip     *m_flipClock;
    DigitalClock *m_digitClock;
    AnalogClock  *m_analogClock;
    WigglyText   *m_wigglyText;
    ParallaxHome *m_parallaxHome;

public:
    SlidingWindow();
    void addWidgetItem(QWidget* widget);
    void addDefaultWidgetItems();  /* Add default widges into sliding panel */

/* Private functions. */
private:
    void setSlideOffset(qreal);
    qreal slideOffset() const;

/* Reimplement QWidget's default event. */
protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    /* The event handlers for sliding item event. */
    void slideLeft();
    void slideRight();
};

#endif /* End of SLIDINGPANELCUST_H */
