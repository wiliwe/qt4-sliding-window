/*! \file  SlidingWindow.cpp
    \brief Implementation of SlidingWindow class.

    \author  William.L
    \date    2010-06-28
    \version 1.0

    \b Change History
    \n 1) 2010-06-28 William.L initialized.
 */

#include "SlidingWindow.h"

/* The dimension of the main window. */
#define MAIN_WIN_WIDTH  1024
#define MAIN_WIN_HEIGHT  600

/* The initial position of control panel. */
#define CTRL_PANEL_POS_X 570
#define CTRL_PANEL_POS_Y 640

/* The dimension of each item. */
#define ITEM_WIDTH   350
#define ITEM_HEIGHT  MAIN_WIN_HEIGHT

#define ITEM_START_X 0  /* The starting position of the item on the sliding window. */
#define ITEM_X_OFFSET 0.0
#define ANIMATION_DURATION 400  /* This is the animation duration in milliseconds. */
#define IDX_HOMESCREEN 0  /* This is the index number indicating the widget to be HomeScreen. */

#define DEBUG // To show debugging messages.

/*! \fn void SlidingWindow::SlidingWindow()
    \brief Constructor.

    \param[in] NONE.
    \return NONE.
 */
SlidingWindow::SlidingWindow()
        : m_slidingSurface(new QWidget(this))
        , m_flipClock(NULL)
        , m_digitClock(NULL)
        , m_analogClock(NULL)
        , m_wigglyText(NULL)
        , m_parallaxHome(NULL)
{
    /* Set the view widget to be frameless. */
    setWindowFlags(Qt::FramelessWindowHint);

#ifdef ENABLE_TRANSPARENT_FRAME
    /* To set the scene's background to be transparent */
    /* LOOK! These two lines will take effect only when COMPIZ(compositing manager) is OPENED */
    /* When using this line, it must set WindowFlags "Qt::FramelessWindowHint" */
    setAttribute(Qt::WA_TranslucentBackground);

    /* Someone may suggest this way. */
    //setStyleSheet("background: transparent");
#endif

    /* Set the sliding window size. */
    setFixedSize((int)MAIN_WIN_WIDTH, (int)MAIN_WIN_HEIGHT);

    m_idxFocusedItem = (qreal)IDX_HOMESCREEN;
    m_idxOldFocusedItem = (int)IDX_HOMESCREEN;
    m_itemSlideOffset = (qreal)ITEM_X_OFFSET;

    /* To set the UI Designer created form instance to a QWidget instance. */
    QWidget *toolWidget = new QWidget(this);
    toolWidget->setWindowFlags(Qt::Tool);
    m_slideControl.setupUi(toolWidget);
    toolWidget->move(QPoint(CTRL_PANEL_POS_X, CTRL_PANEL_POS_Y));
    toolWidget->show();

    /* Connect the LEFT/RIGHT-Sliding and Close buttons' event handler. */
    QObject::connect(m_slideControl.pushButtonLeft, SIGNAL(clicked()), this, SLOT(slideLeft()));
    QObject::connect(m_slideControl.pushButtonRight, SIGNAL(clicked()), this, SLOT(slideRight()));
    QObject::connect(m_slideControl.butnClose, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

    /* Add default item onto sliding window */
    addDefaultWidgetItems();

    /* Create an animation object. */
    m_animationSlide = new QPropertyAnimation(this, "slideOffset");

    /* Set the animation duration */
    if (m_animationSlide)
       m_animationSlide->setDuration(ANIMATION_DURATION);

    /* Set "slideOffset" property to initial value "0.0" that is the location of "HomeScreent" widget */
    setSlideOffset (0.0f + IDX_HOMESCREEN*1.0f);

    /* Initialize the state to disable cause to current shown item is the first one. */
    m_slideControl.pushButtonLeft->setEnabled(false);
}

/*! \fn void SlidingWindow::setSlideOffset(qreal slideRatio)
    \brief Set the offset value for moving items.

    \param[in] ratio value in qreal type.
    \return NONE.
 */
void SlidingWindow::setSlideOffset(qreal slideRatio)
{        
    // we use a ratio to handle resize corectly
    const int pos = -qRound(slideRatio * width());

    m_slidingSurface->scroll(pos - m_slidePanelItems[IDX_HOMESCREEN]->x(), 0);

    if (qFuzzyCompare(slideRatio, static_cast<qreal>(m_itemSlideOffset)))
    {
       m_slidePanelItems[m_idxOldFocusedItem]->hide();
       m_slidePanelItems[m_idxFocusedItem]->show();
    }
    else
    {
       m_slidePanelItems[m_idxFocusedItem]->show();
       m_slidePanelItems[m_idxOldFocusedItem]->show();
    }
}

/*! \fn qreal SlidingWindow::slideOffset() const
    \brief Calculate and return the offset value for moving items.

    \param[in] NONE.
    \return value of qreal type.
 */
qreal SlidingWindow::slideOffset() const
{
    Q_ASSERT(m_slidingSurface->x() < width());

    return static_cast<qreal>(qAbs(m_slidePanelItems[IDX_HOMESCREEN]->x()))/width();
}

/*! \fn void SlidingWindow::addWidgetItem(QWidget* widget)
    \brief Add QWidget/App items.

    \param[in] QWidget object.
    \return NONE.
 */
void SlidingWindow::addWidgetItem(QWidget* widget)
{
    if (widget == NULL)
      return;

    /* Set the parent of the widget item. */
    widget->setParent(m_slidingSurface);

    /* Add the new created widget to the item list. */
    m_slidePanelItems += widget;
}

/*! \fn void SlidingWindow::addDefaultWidgetItems()
    \brief Add QWidget/App items.

    \param[in] NONE.
    \return NONE.
 */
void SlidingWindow::addDefaultWidgetItems()
{
   /* Flipping digital clock */
   m_flipClock = new DigiFlip();
   m_flipClock->resize(ITEM_WIDTH, ITEM_HEIGHT);
   addWidgetItem(m_flipClock);

   /* Digital number clock */
   m_digitClock = new DigitalClock();
   m_digitClock->setWindowFlags(Qt::FramelessWindowHint);  /* Set the view widget to be frameless. */
   m_digitClock->resize(ITEM_WIDTH, ITEM_HEIGHT);
   addWidgetItem(m_digitClock);

   /* Analog clock */
   m_analogClock = new AnalogClock();
   #ifdef ENABLE_TRANSPARENT_FRAME
   m_analogClock->setStyleSheet("background: transparent");
   #endif
   m_analogClock->resize(ITEM_WIDTH, ITEM_HEIGHT);
   addWidgetItem(m_analogClock);

   /* Wiggly text */
   m_wigglyText = new WigglyText(0, true);  /* true for using small screen. */
   #ifdef ENABLE_TRANSPARENT_FRAME
   m_analogClock->setStyleSheet("background: transparent");
   #endif
   m_wigglyText->resize(ITEM_WIDTH, ITEM_HEIGHT);
   addWidgetItem(m_wigglyText);

   /* Parallax Home */
   m_parallaxHome = new ParallaxHome();  /* true for using small screen. */
   m_parallaxHome->resize(ITEM_WIDTH, ITEM_HEIGHT);
   addWidgetItem(m_parallaxHome);
}

/*! \fn void SlidingWindow::slideLeft()
    \brief The slot to handle sliding items LEFT process.

    \param[in] NONE.
    \return NONE.
 */
void SlidingWindow::slideLeft()
{
    #ifdef DEBUG
    qDebug(" Sliding Left! ");
    #endif

    if (m_animationSlide->state() == QAbstractAnimation::Running)
    {
       qDebug ("%s - %s(%d) : Animation is still running!", __FILE__, __FUNCTION__, __LINE__);

       return;
    }

    m_idxOldFocusedItem = m_idxFocusedItem;

   /* If the index of the focused item is greater than
      and equals to zero, e.g.the first element. */
   if(--m_idxFocusedItem >= 0)
   {
      m_slidePanelItems[m_idxFocusedItem]->setFocus();
      m_itemSlideOffset -= 1.0;
   }
   else
     m_idxFocusedItem = m_idxOldFocusedItem;

   m_animationSlide->setDirection(QAbstractAnimation::Forward);
   m_animationSlide->setStartValue(slideOffset());
   m_animationSlide->setEndValue(m_itemSlideOffset);
   m_animationSlide->start();

   /* If the view reach the LEFT/RIGHT boundary, disable LEFT/RIGHT push button. */
   m_slideControl.pushButtonLeft->setEnabled( (m_idxFocusedItem != 0) );
   m_slideControl.pushButtonRight->setEnabled( (m_idxFocusedItem != (m_slidePanelItems.count()-1)) );
}

/*! \fn void SlidingWindow::slideRight()
    \brief The slot to handle sliding items RIGHT process.

    \param[in] NONE.
    \return NONE.
 */
void SlidingWindow::slideRight()
{
   #ifdef DEBUG
   qDebug(" Sliding Right! ");
   #endif

   if(m_animationSlide->state() == QAbstractAnimation::Running)
   {
      qDebug ("%s - %s(%d) : Animation is still running!", __FILE__, __FUNCTION__, __LINE__);

      return;
   }

   m_idxOldFocusedItem = m_idxFocusedItem;

   if(++m_idxFocusedItem <= m_slidePanelItems.count()-1)
   {
      m_slidePanelItems[m_idxFocusedItem]->setFocus();
      m_itemSlideOffset += 1.0;
   }
   else
      m_idxFocusedItem = m_idxOldFocusedItem;

   m_animationSlide->setDirection(QAbstractAnimation::Forward);
   m_animationSlide->setStartValue(slideOffset());
   m_animationSlide->setEndValue(m_itemSlideOffset);
   m_animationSlide->start();

   /* If the view reach the LEFT/RIGHT boundary, disable LEFT/RIGHT push button. */
   m_slideControl.pushButtonLeft->setEnabled( (m_idxFocusedItem != 0) );
   m_slideControl.pushButtonRight->setEnabled( (m_idxFocusedItem != (m_slidePanelItems.count()-1)) );
}

/*! \fn void SlidingWindow::resizeEvent(QResizeEvent *event)
    \brief Override QWidget's default event handler.

    \param[in] QResizeEvent object.
    \return NONE.
 */
void SlidingWindow::resizeEvent(QResizeEvent *event)
{
   int new_width = 0;
   const QSize oldSize = event->oldSize();
   const qreal oldSlidingRatio = static_cast<qreal>(qAbs(m_slidePanelItems[IDX_HOMESCREEN]->x())) / oldSize.width();
   const QSize newSize = event->size();

   /* Set width of the sliding window(canvas) */
   m_slidingSurface->resize((newSize.width()*m_slidePanelItems.count()), newSize.height());

   /* Store the new width */
   new_width = newSize.width();

   /* Set the widgets' size to fit the size of the sliding window. */
   for(int i = 0; i < m_slidePanelItems.count(); i++)
   {
       m_slidePanelItems[i]->resize(newSize);
       m_slidePanelItems[i]->move(((int)ITEM_START_X + i*newSize.width()), 0);
   }

   setSlideOffset(oldSlidingRatio);
}
