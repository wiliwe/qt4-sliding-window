/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

/*! 
 * 2010-06-09 William.L changed to be header form.
 */

#ifndef DIGIFLIP_H
#define DIGIFLIP_H

#include <QtGui>
// Added for Qt5, 2015-09-10. Begin
#include <QtGlobal>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
// Added for Qt5, 2015-09-10. End

/*! \class Digits
    \brief The class of digit number.
*/
class Digits: public QWidget
{
    Q_OBJECT

public:
    Digits(QWidget *parent);
    void setTransition(int tr);
    int transition() const;
    void setNumber(int n);
    void flipTo(int n);

protected:
    void drawFrame(QPainter *p, const QRect &rect);
    QPixmap drawDigits(int n, const QRect &rect);
    void preparePixmap();
    void resizeEvent(QResizeEvent*);
    void paintStatic();
    void paintSlide();
    void paintFlip();
    void paintRotate();
    void paintEvent(QPaintEvent *event);

private:
    int m_number;
    int m_transition;
    QPixmap m_pixmap;
    QPixmap m_lastPixmap;
    QTimeLine m_animator;
};

/* 2010-06-09 William.L changed the super class from QMainWindow to QWdiget. */
/*! \class DigiFlip
    \brief The class of digit flipping clock.
*/
class DigiFlip : public QWidget
{
    Q_OBJECT

public:
    DigiFlip(QWidget *parent = 0);
    void updateTime();
    void switchTransition(int delta);

protected:
    void resizeEvent(QResizeEvent*);
    void timerEvent(QTimerEvent*);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void chooseSlide();
    void chooseFlip();
    void chooseRotate();

private:
    QBasicTimer m_ticker;
    Digits *m_hour;
    Digits *m_minute;
};

#endif // DIGIFLIP_H
