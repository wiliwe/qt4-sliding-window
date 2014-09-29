/*! \file  SlidingWindow.h
    \brief Declaration of SlidingWindow class.

    \author  William.L
    \date    2010-06-28
    \version 1.0

    \b Change History
    \n 1) 2010-06-28 William.L initialized.
 */

#ifndef SLIDINGWINDOW_H
#define SLIDINGWINDOW_H

#include <QtGui>
#include <QGraphicsPixmapItem>

#include "ui_SlidingWindowControl.h"
#include "flickcharm.h"

class QPropertyAnimation;

/*! \class SlidingWindow
    \brief The sliding window class.
*/
class SlidingWindow : public QGraphicsView
{
Q_OBJECT

public:
    SlidingWindow();  /*!< Constructor */

private:
    QGraphicsScene m_scene;       /*!< The canvas */
    QGraphicsItem *m_background;  /*!< The sliding window background picture. */
    QList<QGraphicsPixmapItem*> m_slideItems;  /*!< The list of items of QWidget type. */
    QString m_imageDir;

    #ifdef ADD_PAGE_NUM
    QGraphicsSimpleTextItem *m_textItem;  /*!< To hold the new created text item instance. */
    QList<QGraphicsSimpleTextItem*> m_slidePgNumItems;  /*!< The list of items of QWidget type. */
    #endif

    #ifdef USE_CONTROL_PANEL
    QTimeLine m_timelineItem;             /*!< The timeline for sliding window items. */
    Ui::ControlPanelForm m_slideControl;  /*!< The form constructed from UI designer. */
    int m_nNewPosOfFirstItem;             /*!< The current starting position of X-axis. */
    #endif

    #ifndef USE_CONTROL_PANEL
    FlickCharm m_flickcharm;
    #endif

public:
    void setImageItemPos(QGraphicsPixmapItem *imgItem);  /*!< Set the position of each picture item. */

    #ifdef ADD_PAGE_NUM
    void setImageItemNoPos(QGraphicsPixmapItem *imgItem, QGraphicsSimpleTextItem *imgItemNo,
                           QPointF pos, QPixmap pixmap);  /*!< Set the position of NO. of each picture. */
    #endif

    #ifdef USE_CONTROL_PANEL
    void slideBy(int distanceX);
    #endif

protected:
    void addImages();  /*!< Add default pictures into sliding window */

public slots:
    #ifdef USE_CONTROL_PANEL
    /* The event handlers for buttons to move items. */
    void slideLeft();
    void slideRight();
    #endif

private slots:
    #ifdef USE_CONTROL_PANEL
    void moveItem(int nOffset);  /*!< For moving items. */
    void moveItemFinish();
    #endif
};

#endif /* SLIDINGWINDOW_H */
