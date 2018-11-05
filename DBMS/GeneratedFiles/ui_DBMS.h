/********************************************************************************
** Form generated from reading UI file 'DBMS.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBMS_H
#define UI_DBMS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DBMSClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DBMSClass)
    {
        if (DBMSClass->objectName().isEmpty())
            DBMSClass->setObjectName(QStringLiteral("DBMSClass"));
        DBMSClass->resize(600, 400);
        menuBar = new QMenuBar(DBMSClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        DBMSClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DBMSClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        DBMSClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(DBMSClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        DBMSClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(DBMSClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DBMSClass->setStatusBar(statusBar);

        retranslateUi(DBMSClass);

        QMetaObject::connectSlotsByName(DBMSClass);
    } // setupUi

    void retranslateUi(QMainWindow *DBMSClass)
    {
        DBMSClass->setWindowTitle(QApplication::translate("DBMSClass", "DBMS", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DBMSClass: public Ui_DBMSClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBMS_H
