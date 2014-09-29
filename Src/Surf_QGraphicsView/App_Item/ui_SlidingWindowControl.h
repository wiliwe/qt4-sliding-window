/********************************************************************************
** Form generated from reading UI file 'SlidingWindowControl.ui'
**
** Created: Tue Sep 30 00:24:10 2014
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLIDINGWINDOWCONTROL_H
#define UI_SLIDINGWINDOWCONTROL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlPanelForm
{
public:
    QPushButton *pushButtonLeft;
    QPushButton *pushButtonRight;
    QPushButton *pushButtonClose;

    void setupUi(QWidget *ControlPanelForm)
    {
        if (ControlPanelForm->objectName().isEmpty())
            ControlPanelForm->setObjectName(QString::fromUtf8("ControlPanelForm"));
        ControlPanelForm->resize(271, 108);
        pushButtonLeft = new QPushButton(ControlPanelForm);
        pushButtonLeft->setObjectName(QString::fromUtf8("pushButtonLeft"));
        pushButtonLeft->setGeometry(QRect(10, 10, 121, 51));
        pushButtonRight = new QPushButton(ControlPanelForm);
        pushButtonRight->setObjectName(QString::fromUtf8("pushButtonRight"));
        pushButtonRight->setGeometry(QRect(150, 10, 111, 51));
        pushButtonClose = new QPushButton(ControlPanelForm);
        pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));
        pushButtonClose->setGeometry(QRect(10, 70, 251, 27));

        retranslateUi(ControlPanelForm);

        QMetaObject::connectSlotsByName(ControlPanelForm);
    } // setupUi

    void retranslateUi(QWidget *ControlPanelForm)
    {
        ControlPanelForm->setWindowTitle(QApplication::translate("ControlPanelForm", "Form", 0, QApplication::UnicodeUTF8));
        pushButtonLeft->setText(QApplication::translate("ControlPanelForm", "Left", 0, QApplication::UnicodeUTF8));
        pushButtonRight->setText(QApplication::translate("ControlPanelForm", "Right", 0, QApplication::UnicodeUTF8));
        pushButtonClose->setText(QApplication::translate("ControlPanelForm", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ControlPanelForm: public Ui_ControlPanelForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLIDINGWINDOWCONTROL_H
