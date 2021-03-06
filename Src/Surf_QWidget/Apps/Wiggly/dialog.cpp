/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
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

#include <QtGui>

// Added for Qt5, 2015-09-10. Begin
#include <QtGlobal>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#endif
// Added for Qt5, 2015-09-10. End

#include "dialog.h"
#include "wigglywidget.h"

/* 2010-06-09 William.L added. Begin */
#define DEFAULT_TEXT_SHORT "Qt4!"
#define DEFAULT_TEXT_LONG "Hello, Qt4 !"
/* 2010-06-09 William.L added. End */

/* 2010-06-09 William.L changed class name from "Dialog" to "WigglyText". */
//! [0]
WigglyText::WigglyText(QWidget *parent, bool smallScreen)
    : QDialog(parent)
{
    WigglyWidget *wigglyWidget = new WigglyWidget;
    QLineEdit *lineEdit = new QLineEdit;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(wigglyWidget);
    layout->addWidget(lineEdit);
    setLayout(layout);

    connect(lineEdit, SIGNAL(textChanged(QString)),
            wigglyWidget, SLOT(setText(QString)));
    if (!smallScreen){
        lineEdit->setText(tr(DEFAULT_TEXT_LONG/*"Hello world!"*/));  /* 2010-06-09 William.L changed string. */
    }
    else{
        lineEdit->setText(tr(DEFAULT_TEXT_LONG/*"Hello!"*/));  /* 2010-06-09 William.L changed string. */
    }
    setWindowTitle(tr("Wiggly"));
    resize(360, 145);
}
//! [0]
