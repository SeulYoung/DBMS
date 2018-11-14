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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DBMSClass
{
public:
    QWidget *centralWidget;
    QTreeWidget *tree;
    QTableWidget *table;
    QTextEdit *cmdLine;
    QLineEdit *inputLine;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QMenu *menu_5;
    QMenu *menu_6;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DBMSClass)
    {
        if (DBMSClass->objectName().isEmpty())
            DBMSClass->setObjectName(QStringLiteral("DBMSClass"));
        DBMSClass->resize(1000, 800);
        centralWidget = new QWidget(DBMSClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tree = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        tree->setHeaderItem(__qtreewidgetitem);
        tree->setObjectName(QStringLiteral("tree"));
        tree->setGeometry(QRect(0, 0, 341, 451));
        table = new QTableWidget(centralWidget);
        table->setObjectName(QStringLiteral("table"));
        table->setGeometry(QRect(349, 0, 651, 451));
        cmdLine = new QTextEdit(centralWidget);
        cmdLine->setObjectName(QStringLiteral("cmdLine"));
        cmdLine->setEnabled(true);
        cmdLine->setGeometry(QRect(0, 460, 1000, 251));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(11);
        cmdLine->setFont(font);
        cmdLine->setReadOnly(true);
        inputLine = new QLineEdit(centralWidget);
        inputLine->setObjectName(QStringLiteral("inputLine"));
        inputLine->setGeometry(QRect(0, 710, 1000, 31));
        inputLine->setFont(font);
        DBMSClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DBMSClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QStringLiteral("menu_4"));
        menu_5 = new QMenu(menuBar);
        menu_5->setObjectName(QStringLiteral("menu_5"));
        menu_6 = new QMenu(menuBar);
        menu_6->setObjectName(QStringLiteral("menu_6"));
        DBMSClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DBMSClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        DBMSClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DBMSClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DBMSClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menuBar->addAction(menu_5->menuAction());
        menuBar->addAction(menu_6->menuAction());

        retranslateUi(DBMSClass);
        QObject::connect(inputLine, SIGNAL(returnPressed()), DBMSClass, SLOT(getCmd()));

        QMetaObject::connectSlotsByName(DBMSClass);
    } // setupUi

    void retranslateUi(QMainWindow *DBMSClass)
    {
        DBMSClass->setWindowTitle(QApplication::translate("DBMSClass", "DBMS", nullptr));
        menu->setTitle(QApplication::translate("DBMSClass", "\347\263\273\347\273\237", nullptr));
        menu_2->setTitle(QApplication::translate("DBMSClass", "\346\225\260\346\215\256\345\272\223", nullptr));
        menu_3->setTitle(QApplication::translate("DBMSClass", "\350\241\250", nullptr));
        menu_4->setTitle(QApplication::translate("DBMSClass", "\345\255\227\346\256\265", nullptr));
        menu_5->setTitle(QApplication::translate("DBMSClass", "\350\256\260\345\275\225", nullptr));
        menu_6->setTitle(QApplication::translate("DBMSClass", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DBMSClass: public Ui_DBMSClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBMS_H
