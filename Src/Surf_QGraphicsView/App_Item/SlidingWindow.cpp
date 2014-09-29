/*! \file SlidingWindow.cpp
    \brief Implementation of SlidingWindow class.

    \author William.L
    \version 1.0
    \date 2010-06-08

    \b Change History
    \n 2010-06-08  William.L initialized.
 */

#include <QDebug>

#include "SlidingWindow.h"

/* The dimension of the main window. */
#define MAIN_WIN_WIDTH  1024
#define MAIN_WIN_HEIGHT  600

/* The dimension of item in the sliding window. */
#define ITEM_WIDTH   350
#define ITEM_HEIGHT  MAIN_WIN_HEIGHT
#define ITEM_GAP     200

#define SLIDE_OFFSET (ITEM_WIDTH+50)   /* The moving offset. */

#ifdef USE_CONTROL_PANEL
/* The initial position of control panel. */
#define CTRL_PANEL_POS_X 570
#define CTRL_PANEL_POS_Y 640

#define ITEM_POS_START_X 0  /* The x-axis value for the first item. */
#define ANIMATION_DURATION 2000
#endif

#define DEBUG // To show debugging messages.

/*! \fn SlidingWindow::SlidingWindow()
    \brief Constructor.

    \param[in] NONE.
    \return NONE.
 */
SlidingWindow::SlidingWindow()
    : QGraphicsView()
    #ifdef USE_CONTROL_PANEL
    , m_nNewPosOfFirstItem(0)
    #endif
    #ifndef USE_CONTROL_PANEL
    , m_flickcharm(this)
    #endif
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

    /* There are three ways to achieve transparent window frame effect. */
    #if 1
    /* <Way - 1> */
    viewport()->setAutoFillBackground(false);
    #elif 0
    /* <Way - 2> */
    QPalette p = viewport()->palette();

    p.setColor(QPalette::Base, Qt::transparent);
    viewport()->setPalette(p);
    #elif 0
    /* <Way - 3>
       Someone may suggest this way. */
    setStyleSheet("background: transparent");
    #endif
#endif

    /* To set the scene(canvas) */
    setScene(&scene);

    /* To let Horizontal/Vertical scrollbar be invisible always. */
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /* Add QGraphicsView object into FlickCharm. */
    #ifndef USE_CONTROL_PANEL
    m_flickcharm.activateOn(this, FLICK_HORIZONTAL);
    #endif

    #if 0 // These two lines are for real device.
    /* Set the window size fitting the dimension of the screen. */
    QRect screen_size = QApplication::desktop()->screenGeometry();
    setFixedSize(screen_size.width(), screen_size.height());
    #else  // This is for simulating on development environment.
    /* Set the window size. */
    setFixedSize((int)MAIN_WIN_WIDTH, (int)MAIN_WIN_HEIGHT);
    #endif

    /* To set the UI Designer created form instance to a QWidget instance. */
    #ifdef USE_CONTROL_PANEL
    QWidget *toolWidget = new QWidget(this);

    toolWidget->setWindowFlags(Qt::Tool);
    m_slideControl.setupUi(toolWidget);
    toolWidget->move(QPoint(CTRL_PANEL_POS_X, CTRL_PANEL_POS_Y));
    toolWidget->show();

    /* Connect the buttons' event handler. */
    QObject::connect(m_slideControl.pushButtonLeft, SIGNAL(clicked()), this, SLOT(slideLeft()));
    QObject::connect(m_slideControl.pushButtonRight, SIGNAL(clicked()), this, SLOT(slideRight()));
    QObject::connect(m_slideControl.pushButtonClose, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

    /* Set timeline easing curve. */
    m_timelineItem.setCurveShape(QTimeLine::EaseInOutCurve);

    /* Set timeline's "new-frame" event handler */
    QObject::connect(&m_timelineItem, SIGNAL(frameChanged(int)), this, SLOT(moveItem(int)));

    /* Set timeline's "finished" event handler */
    QObject::connect(&m_timelineItem, SIGNAL(finished()), this, SLOT(moveItemFinish()));

    /* Set the duration of animation. */
    m_timelineItem.setDuration((int)ANIMATION_DURATION);
    m_timelineItem.stop();
    #endif

    addDefaultWidgetItems();

    /* Move the first image. */
    #ifdef USE_CONTROL_PANEL
    slideBy(0);
    #endif
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

/*! \fn void SlidingWindow::addWidgetItem(QWidget* widget)
    \brief Add QWidget/App items.

    \param[in] QWidget object.
    \return NONE.
 */
void SlidingWindow::addWidgetItem(QWidget* widget)
{
   if (widget == NULL)
     return;

   #ifndef USE_CONTROL_PANEL
   widget->installEventFilter(&m_flickcharm);
   #endif

   /* Add the new created widget to the scene */
   QGraphicsProxyWidget *proxyWidget = scene.addWidget(widget);

   if (slideItems.count() == 0)
       proxyWidget->setPos((slideItems.count() * ITEM_WIDTH), 0);
   else
       proxyWidget->setPos((slideItems.count() * (ITEM_WIDTH + ITEM_GAP)), 0);

   sceneProxyWidgets += proxyWidget; /* Add the new created proxy widget to the proxy widget list. */

   /* Add the new created widget to the item list. */
   slideItems += widget;

   #ifdef DEBUG
   qDebug("Total %d items; Total %d graphics proxy widgets; \nWidget's position = (%f,%f).",
          slideItems.count(), sceneProxyWidgets.count(),
          proxyWidget->pos().x(), proxyWidget->pos().y());
   #endif
}

#ifdef USE_CONTROL_PANEL
/*! \fn void SlidingWindow::slideLeft()
    \brief The slot to handle sliding items LEFT process.

    \param[in] NONE.
    \return NONE.
 */
void SlidingWindow::slideLeft()
{
   #ifdef DEBUG
   qDebug(" Push Left button! nOffset = %d", m_nNewPosOfFirstItem);
   #endif

   /* Determine if the timeline is running now. If it is running, just return. */
   if(m_timelineItem.state() != QTimeLine::NotRunning)
   {
      qDebug(" Timeline is still Running! ");
      return;
   }

   if( !(m_nNewPosOfFirstItem >= ITEM_POS_START_X) )
     slideBy(SLIDE_OFFSET);
}

/*! \fn void SlidingWindow::slideRight()
    \brief The slot to handle sliding items RIGHT process.

    \param[in] NONE.
    \return NONE.
 */
void SlidingWindow::slideRight()
{
   #ifdef DEBUG
   qDebug(" Push Right button! nOffset = %d", m_nNewPosOfFirstItem);
   #endif

   /* Determine if the timeline is running now. If it is running, just return. */
   if(m_timelineItem.state() != QTimeLine::NotRunning)
   {
      qDebug(" Timeline is still Running! ");
      return;
   }

   if( !(m_nNewPosOfFirstItem <= -(SLIDE_OFFSET * (sceneProxyWidgets.count()-1))) )
      slideBy(-SLIDE_OFFSET);
}

/*! \fn void SlidingWindow::moveItem(int offset)
    \brief The slot to handle LEFT/RIGHT arrow key events.

    \param[in] offset of integer type for moving items.
    \return NONE.
 */
void SlidingWindow::moveItem(int offset)
{
   #ifdef DEBUG
   qDebug("offset = %d", offset);
   #endif

   if (sceneProxyWidgets.count() > 1)
   {
     for(int i = 0; i < sceneProxyWidgets.count(); i++)
       sceneProxyWidgets[i]->setPos((offset + i * (ITEM_WIDTH + ITEM_GAP)), sceneProxyWidgets[i]->pos().y());
   }
}

/*! \fn void SlidingWindow::moveItemFinish()
    \brief The callback function called when animation accomplished.

    \param[in] NONE.
    \return NONE.
 */
void SlidingWindow::moveItemFinish()
{
   ;

   #ifdef DEBUG
   for(int i = 0; i < sceneProxyWidgets.count(); i++)
      qDebug("Widget-%d position = (%f, %f)",
             i, sceneProxyWidgets[i]->pos().x(), sceneProxyWidgets[i]->pos().y());
   #endif
}


/*! \fn void SlidingWindow::slideBy(int distanceX)
    \brief Start animation of moving items.

    \param[in] the distance of moving an item.
    \return NONE.
 */
void SlidingWindow::slideBy(int distanceX)
{
   int itemStart = m_nNewPosOfFirstItem;  /* Set start frame of the animation. */
   int itemEnd = m_nNewPosOfFirstItem + distanceX;  /* Set end frame of the animation. */

   #ifdef DEBUG
   qDebug("%d Old nOffset= %d, itemStart= %d, itemEnd= %d",
           __LINE__, m_nNewPosOfFirstItem, itemStart, itemEnd);
   #endif

   /* Set the animation frame range. */
   m_timelineItem.setFrameRange(itemStart, itemEnd);

   /* Start the timeline and start do animation. */
   m_timelineItem.start();

   /* Update the latest moved distance. */
   m_nNewPosOfFirstItem = itemEnd;

   #ifdef DEBUG
   qDebug("%d Old nOffset= %d, itemStart= %d, itemEnd= %d",
           __LINE__, m_nNewPosOfFirstItem, itemStart, itemEnd);
   #endif

   /* If the view reach the LEFT/RIGHT boundary,
      disable LEFT/RIGHT push button. */
   m_slideControl.pushButtonLeft->setEnabled( !(m_nNewPosOfFirstItem >= ITEM_POS_START_X) );
   m_slideControl.pushButtonRight->setEnabled( !(m_nNewPosOfFirstItem <= -(SLIDE_OFFSET*(sceneProxyWidgets.count()-1))) );
}
#endif
