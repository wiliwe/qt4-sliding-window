/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Graphics Dojo project on Qt Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore>
#include <QtGui>
#include <QtSvg>

#if defined (Q_OS_SYMBIAN)
#include "sym_iap_util.h"
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#endif

#define PAGE_COUNT 5

/*! \class DigitalClock
    \brief The class of navigator bar of parallax home screen.
*/
class NaviBar : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    NaviBar();

    void setPageOffset(qreal ofs);

signals:
    void pageSelected(int page);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    QList<QGraphicsSvgItem*> m_icons;
    QGraphicsRectItem *m_cursor;
};

/*! \class ParallaxHome
    \brief The class of parallax home screen.
*/
class ParallaxHome: public QGraphicsView
{
    Q_OBJECT

public:
    QGraphicsScene m_scene;
    NaviBar *m_naviBar;
    QGraphicsPixmapItem *m_wallpaper;
    QTimeLine m_pageAnimator;
    qreal m_pageOffset;
    QList<QGraphicsPixmapItem*> m_items;
    QList<QPointF> m_positions;

public:
    ParallaxHome(QWidget *parent = 0);

signals:
    void pageChanged(int page);

public slots:

    void slideRight();
    void slideLeft();
    void slideBy(int dx);
    void choosePage(int page);

private slots:

    void shiftPage(int frame);

protected:

    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void layoutScene();
    void setupScene();
};
