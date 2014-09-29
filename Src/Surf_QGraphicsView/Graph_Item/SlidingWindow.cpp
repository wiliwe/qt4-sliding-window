/*! \file  SlidingWindow.cpp
    \brief Implementation of SlidingWindow class.

    \author  William.L
    \date    2010-06-28
    \version 1.0

    \b Change History
    \n 1) 2010-06-28 William.L initialized.
 */

#include <QDir>
#include <QStringList>
#include <QPixmap>
#include <QDebug>

#include "SlidingWindow.h"

/* The dimension of the main window. */
#define MAIN_WIN_WIDTH  1024
#define MAIN_WIN_HEIGHT  600

#define SLIDE_OFFSET MAIN_WIN_WIDTH  /* The moving offset. */

#ifdef USE_CONTROL_PANEL
#define CTRL_PANEL_POS_X 570
#define CTRL_PANEL_POS_Y 640

#define ITEM_POS_START_X  0    /* The x-axis value for the first item. */
#define ANIMATION_DURATION 500
#endif

#define DFLT_IMG_DIR "images"  /* The default folder containing images. */

#ifdef ADD_PAGE_NUM
#define PAGE_NUM_POS_OFFSET_X  50
#define PAGE_NUM_POS_OFFSET_Y  30
#endif

#define DEBUG   // To show debugging messages.

/*! \fn SlidingWindow::SlidingWindow()
    \brief Constructor
 */
SlidingWindow::SlidingWindow()
    : QGraphicsView()
    #ifdef USE_CONTROL_PANEL
    , m_nNewPosOfFirstItem(0)
    #endif
    #ifndef USE_CONTROL_PANEL
    , m_flickcharm(this, FLICK_HORIZONTAL)
    #endif
    #ifdef ADD_PAGE_NUM
    , m_textItem(NULL)
    #endif
{
    /* Set the view widget to be frameless. */
    setWindowFlags(Qt::FramelessWindowHint);

#ifdef ENABLE_TRANSPARENT_FRAME
    /* To set the scene's background to be transparent */
    /* LOOK! In Linux, this will take effect only when
        COMPIZ(compositing manager) turns on. */
    /* When using this line, it must set WindowFlags "Qt::FramelessWindowHint" */
    setAttribute(Qt::WA_TranslucentBackground);

    /* There are three ways to achieve transparent window frame effect. */
    #if 0
    /* <Way - 1> */
    viewport()->setAutoFillBackground(false);
    #elif 1
    /* <Way - 2>. */
    QPalette p = viewport()->palette();
    p.setColor(QPalette::Base, Qt::transparent);
    viewport()->setPalette(p);
    #elif 0
    /* <Way - 3>
       Someone may suggest this way. */
    setStyleSheet("background: transparent");
    #endif
#endif /* ENABLE_TRANSPARENT_FRAME */

    /* To set the scene(canvas) */
    setScene(&m_scene);

    /* To let Horizontal/Vertical scrollbar be invisible always. */
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /* Add QGraphicsView object into FlickCharm. */
    #ifndef USE_CONTROL_PANEL
    m_flickcharm.activateOn(this);
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

    m_slideControl.setupUi(toolWidget);
    toolWidget->setWindowFlags(Qt::Tool);
    toolWidget->move(QPoint(CTRL_PANEL_POS_X, CTRL_PANEL_POS_Y));
    toolWidget->show();

    /* Connect the buttons' event handler. */
    QObject::connect(m_slideControl.pushButtonLeft, SIGNAL(clicked()), this, SLOT(slideLeft()));
    QObject::connect(m_slideControl.pushButtonRight, SIGNAL(clicked()), this, SLOT(slideRight()));
    QObject::connect(m_slideControl.pushButtonClose, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

    /* Set animation timeline's easing curve. Using Linear easing curve. */
    m_timelineItem.setCurveShape(QTimeLine::LinearCurve);

    /* Set animation timeline's "new-frame" event handler */
    QObject::connect(&m_timelineItem, SIGNAL(frameChanged(int)), this, SLOT(moveItem(int)));

    /* Set timeline's "finished" event handler */
    QObject::connect(&m_timelineItem, SIGNAL(finished()), this, SLOT(moveItemFinish()));

    /* Set the duration of animation. */
    m_timelineItem.setDuration((int)ANIMATION_DURATION);
    m_timelineItem.stop();
    #endif

    /* Reset QString content. */
    m_imageDir.clear();
    m_imageDir.append(DFLT_IMG_DIR);

    /* Add all images under the image folder. */
    addImages();

    centerOn(width() / 2, height() / 2);

    /* Move the first image. */
    #ifdef USE_CONTROL_PANEL
    slideBy(0);
    #endif
}

/*! \fn void SlidingWindow::addImages()
    \brief Add image items.

    \param[in] NONE.
    \return NONE.
 */
void SlidingWindow::addImages()
{
    QDir imgDir(m_imageDir); // Create a QDir instance representing the directory containing images.
    #ifdef ADD_PAGE_NUM
    QPen pen;  // Create a Pen object for draw page number of each picture.
    QBrush brush(Qt::SolidPattern);
    #endif

    /* To examinate if the image folder is existent. */
    if(imgDir.exists() == false)
    {
       qDebug("%s - %s(%d) : %s does not exist!!!",
               __FILE__, __FUNCTION__, __LINE__, m_imageDir.toAscii().data());
       return;
    }
    else
       qDebug("Current image directory name = %s", imgDir.dirName().toAscii().data());

    /* Sort contents under the directory by File Name. */
    imgDir.setSorting(QDir::Name);

    /* Set the extenstion name filter */
    QStringList nameFilters;
    nameFilters << "*.png" << "*.jpg" << "*.jpeg";  /* Currently, allow reading PNG, JPEG, JPG image files. */
    imgDir.setNameFilters(nameFilters);

    /* Set filter for displaying Files only */
    imgDir.setFilter(QDir::Files);

    /* Configure the Pen object. */
    #ifdef ADD_PAGE_NUM
    pen.setWidth(2);
    pen.setColor(QColor(105, 158, 0, 255)); // R, G, B, Alpha
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    #endif

    /* To iterate all images under the created the directory and add them into  */
    QStringList imgFileList = imgDir.entryList();
    for(int i = 0; i < imgFileList.count(); i++)
    {
        /* Create a Pixmap*/
        QPixmap pixmap( QString("%1/%2").arg(m_imageDir).arg(((QString)imgFileList.at(i)).toAscii().data()) ) ; /* 1st, 2nd argement assignment to form a string. */

        /* Declare a QGraphicsPixmapItem pointer */
        QGraphicsPixmapItem *imageItem = NULL;

        /* Scale the new created image item to fit the dimension of the screen. */
        int diffScreenWidth = pixmap.width() - width();
        int diffScreenHeight = pixmap.height() - height();
        qreal scaleRatio = 1.0;
        int newHeight = 0, newWidth = 0;

        /* If the width of pixmap is greater than screen width. */
        if(diffScreenWidth > 0)
        {
           scaleRatio =  static_cast<qreal>(width()) / static_cast<qreal>(pixmap.width());
           newWidth = static_cast<int>(pixmap.width() * scaleRatio);

           /* If the calculated width is exactly the same as screen width minus one,
              just assign the screen width to the new width. */
           if (newWidth == (width()-1))
              newWidth = width();

           #ifdef DEBUG
           qDebug("%s : %s(%d) - Item(%d) WIDTH(%d) : screenWidth = %d, diffScreenWidth = %d , scaleRatio=%f, newWidth = %d",
                  __FILE__, __FUNCTION__, __LINE__, i,
                  pixmap.width(), width(), diffScreenWidth, scaleRatio, newWidth);
           #endif
        }

        /* If the height of pixmap is greater than screen height. */
        if(diffScreenHeight > 0)
        {
           scaleRatio =  static_cast<qreal>(height()) / static_cast<qreal>(pixmap.height());
           newHeight = static_cast<int>(pixmap.height() * scaleRatio);

           /* If the calculated height is exactly the same as screen height minus one,
              just assign the screen height to the new height. */
           if(newHeight == (height()-1))
              newHeight = height();

           #ifdef DEBUG
           qDebug("%s : %s(%d) - Item(%d) HEIGHT(%d) : screenHeight = %d, diffScreenHeight = %d, scaleRatio=%f, newHeight = %d",
                  __FILE__, __FUNCTION__, __LINE__, i,
                  pixmap.height(), height(), diffScreenHeight, scaleRatio, newHeight);
           #endif
        }

        /* Create a graphic item from a Scaled Pixmap image. */
        if(scaleRatio != 1.0)
        {
            if(newWidth != 0 && newHeight != 0)
            {
                #ifdef DEBUG
                qDebug("%s : %s(%d)", __FILE__, __FUNCTION__, __LINE__);
                #endif

                imageItem = m_scene.addPixmap(pixmap.scaled( QSize(newWidth,newHeight) ));
            }
            else if(newWidth != 0)
            {
                #ifdef DEBUG
                qDebug("%s : %s(%d)", __FILE__, __FUNCTION__, __LINE__);
                #endif

                imageItem = m_scene.addPixmap(pixmap.scaled( QSize(newWidth,pixmap.height()) ));
            }
            else if(newHeight != 0)
            {
                #ifdef DEBUG
                qDebug("%s : %s(%d)", __FILE__, __FUNCTION__, __LINE__);
                #endif

                imageItem = m_scene.addPixmap(pixmap.scaled( QSize(pixmap.width(),newHeight) ));
            }
        } /* Create a graphic item from unscaled Pixmap image. */
        else if(diffScreenWidth<=0 && diffScreenHeight<=0)
        {
            #ifdef DEBUG
            qDebug("%s : %s(%d)", __FILE__, __FUNCTION__, __LINE__);
            #endif

            imageItem = m_scene.addPixmap(pixmap);
        }

        /* Create a QGraphicsSimpleTextItem pointer */
        #ifdef ADD_PAGE_NUM
        QString pageNo;

        pageNo.sprintf ("P.%d", (i+1));
        m_textItem = m_scene.addSimpleText(pageNo);
        m_textItem->setPen(pen);
        m_textItem->setBrush(brush);
        m_textItem->setFont(QFont("Arial", 14)); // set the font of page number.
        m_textItem->setZValue(1); // Let it be the layer above the pixmap item.

        QFont textFont = m_textItem->font();
        textFont.setPointSize(16); // set the font size of page number.
        #endif

        /* Set the position of pixmap graphic item on the scene. */
        setImageItemPos(imageItem);

        #ifdef ADD_PAGE_NUM
        /* Set the position of NO. of pixmap graphic item on the scene. */
        setImageItemNoPos(imageItem, m_textItem, imageItem->pos(), imageItem->pixmap());
        #endif
    }
}

/*! \fn void SlidingWindow::setImageItemPos(QGraphicsPixmapItem* imgItem)
    \brief Set item's position

    \param[in] QGraphicsPixmapItem object.
    \return NONE.
 */
void SlidingWindow::setImageItemPos(QGraphicsPixmapItem* imgItem)
{
   if(imgItem == NULL)
   {
      qDebug("%s - %s(%d) : Passed in graphic Pixmap object is NULL!!!",
              __FILE__, __FUNCTION__, __LINE__);
      return;
   }

   QPixmap pixmap = imgItem->pixmap();
   int posX = 0.0, posY = 0.0;
   int diffWidth = 0, diffHeight = 0;

   if(pixmap.width() < width())
   {
       diffWidth = width() - pixmap.width();
       posX = diffWidth / 2;

       #ifdef DEBUG
       qDebug("%s : %s(%d) - posX = %d", __FILE__, __FUNCTION__, __LINE__, posX);
       #endif
   }

   if(pixmap.height() < height())
   {
      diffHeight = height() - pixmap.height();
      posY = diffHeight / 2 ;

      #ifdef DEBUG
      qDebug("%s : %s(%d) - posY = %d", __FILE__, __FUNCTION__, __LINE__, posY);
      #endif
   }

   /* Add the new created graphic items to the scene */
   imgItem->setPos(m_slideItems.count()*SLIDE_OFFSET+posX, 0+posY);

   #ifdef DEBUG
   qDebug("%s : %s() - pixWidth = %d, pixHeight = %d, posX = %d, posY = %d, sceneX = %f, sceneY = %f\n",
          __FILE__, __FUNCTION__,
          pixmap.width(), pixmap.height(), posX, posY,
          imgItem->scenePos().x(), imgItem->scenePos().y());
   #endif

   /* Add the new created a graphical item to the item list. */
   m_slideItems += imgItem;
}

#ifdef ADD_PAGE_NUM
/*! \fn void SlidingWindow::setImageItemNoPos(QGraphicsPixmapItem* imgItem, QGraphicsSimpleTextItem *imgItemNo, QPointF pos, QPixmap pixmap)
    \brief Add page number onto a image item.

    \param[in] QGraphicsPixmapItem object.
    \param[in] QGraphicsSimpleTextItem object.
    \param[in] QPointF object.
    \param[in] QPixmap object.
    \return NONE.
 */
void SlidingWindow::setImageItemNoPos(QGraphicsPixmapItem* imgItem, QGraphicsSimpleTextItem *imgItemNo,
                                      QPointF pos, QPixmap pixmap)
{
    if(imgItem == NULL || imgItemNo == NULL)
    {
       qDebug("%s - %s(%d) : Passed in simple-text/graphic Pixmap object is NULL!!!",
               __FILE__, __FUNCTION__, __LINE__);
       return;
    }

    if(pixmap.isNull())
    {
       qDebug("%s - %s(%d) : Passed in Pixmap object is NULL!!!",
               __FILE__, __FUNCTION__, __LINE__);
       return;
    }

    imgItemNo->setPos(pos.x()+pixmap.width()-PAGE_NUM_POS_OFFSET_X, pos.y()+pixmap.height()-PAGE_NUM_POS_OFFSET_Y);

    /* Add the new created a page number item to the list. */
    m_slidePgNumItems += imgItemNo;
}
#endif

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

   if( !(m_nNewPosOfFirstItem <= -(SLIDE_OFFSET * (m_slideItems.count()-1))) )
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

   for(int i = 0; i < m_slideItems.count(); i++)
   {
      /* Move picture items to the new position */
      m_slideItems[i]->setPos(offset + i * width(), m_slideItems[i]->pos().y());

      /* Move page number items to the new position */
      m_slidePgNumItems[i]->setPos(m_slideItems[i]->pos().x()+m_slideItems[i]->pixmap().width()-PAGE_NUM_POS_OFFSET_X,
                                   m_slideItems[i]->pos().y()+m_slideItems[i]->pixmap().height()-PAGE_NUM_POS_OFFSET_Y);
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
   qDebug("%d nOffset= %d ", __LINE__, m_nNewPosOfFirstItem);
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
   qDebug("%s(%d) New nOffset= %d", __FUNCTION__, __LINE__, m_nNewPosOfFirstItem);
   #endif

   /* If the view reach the LEFT/RIGHT boundary, disable LEFT/RIGHT push button. */
   m_slideControl.pushButtonLeft->setEnabled( !(m_nNewPosOfFirstItem >= ITEM_POS_START_X) );
   m_slideControl.pushButtonRight->setEnabled( !(m_nNewPosOfFirstItem <= -(SLIDE_OFFSET*(m_slideItems.count()-1))) );
}
#endif
