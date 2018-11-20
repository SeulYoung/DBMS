/********************************************************************************
** Form generated from reading UI file 'DBMS.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
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
    QAction *exit;
    QAction *newDb;
    QAction *refresh;
    QAction *deleteDb;
    QAction *newTable;
    QAction *deleteTable;
    QAction *newField;
    QAction *deleteField;
    QWidget *centralWidget;
    QTreeWidget *tree;
    QTableWidget *table;
    QTextEdit *cmdLine;
    QLineEdit *inputLine;
    QMenuBar *menuBar;
    QMenu *systemMenu;
    QMenu *databaseMenu;
    QMenu *tableMenu;
    QMenu *fieldMenu;
    QMenu *recordMenu;
    QMenu *helpMenu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DBMSClass)
    {
        if (DBMSClass->objectName().isEmpty())
            DBMSClass->setObjectName(QStringLiteral("DBMSClass"));
        DBMSClass->resize(1000, 800);
        exit = new QAction(DBMSClass);
        exit->setObjectName(QStringLiteral("exit"));
        newDb = new QAction(DBMSClass);
        newDb->setObjectName(QStringLiteral("newDb"));
        refresh = new QAction(DBMSClass);
        refresh->setObjectName(QStringLiteral("refresh"));
        deleteDb = new QAction(DBMSClass);
        deleteDb->setObjectName(QStringLiteral("deleteDb"));
        newTable = new QAction(DBMSClass);
        newTable->setObjectName(QStringLiteral("newTable"));
        deleteTable = new QAction(DBMSClass);
        deleteTable->setObjectName(QStringLiteral("deleteTable"));
        newField = new QAction(DBMSClass);
        newField->setObjectName(QStringLiteral("newField"));
        deleteField = new QAction(DBMSClass);
        deleteField->setObjectName(QStringLiteral("deleteField"));
        centralWidget = new QWidget(DBMSClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tree = new QTreeWidget(centralWidget);
        tree->setObjectName(QStringLiteral("tree"));
        tree->setGeometry(QRect(0, 0, 341, 451));
        tree->header()->setVisible(true);
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
        systemMenu = new QMenu(menuBar);
        systemMenu->setObjectName(QStringLiteral("systemMenu"));
        databaseMenu = new QMenu(menuBar);
        databaseMenu->setObjectName(QStringLiteral("databaseMenu"));
        tableMenu = new QMenu(menuBar);
        tableMenu->setObjectName(QStringLiteral("tableMenu"));
        fieldMenu = new QMenu(menuBar);
        fieldMenu->setObjectName(QStringLiteral("fieldMenu"));
        recordMenu = new QMenu(menuBar);
        recordMenu->setObjectName(QStringLiteral("recordMenu"));
        helpMenu = new QMenu(menuBar);
        helpMenu->setObjectName(QStringLiteral("helpMenu"));
        DBMSClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DBMSClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        DBMSClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DBMSClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DBMSClass->setStatusBar(statusBar);

        menuBar->addAction(systemMenu->menuAction());
        menuBar->addAction(databaseMenu->menuAction());
        menuBar->addAction(tableMenu->menuAction());
        menuBar->addAction(fieldMenu->menuAction());
        menuBar->addAction(recordMenu->menuAction());
        menuBar->addAction(helpMenu->menuAction());
        systemMenu->addAction(refresh);
        systemMenu->addAction(exit);
        databaseMenu->addAction(newDb);
        databaseMenu->addAction(deleteDb);
        tableMenu->addAction(newTable);
        tableMenu->addAction(deleteTable);
        fieldMenu->addAction(newField);
        fieldMenu->addAction(deleteField);

        retranslateUi(DBMSClass);
        QObject::connect(inputLine, SIGNAL(returnPressed()), DBMSClass, SLOT(getCmd()));
        QObject::connect(menuBar, SIGNAL(triggered(QAction*)), DBMSClass, SLOT(menuClicked()));
        QObject::connect(tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), DBMSClass, SLOT(treeClicked(QTreeWidgetItem*,int)));

        QMetaObject::connectSlotsByName(DBMSClass);
    } // setupUi

    void retranslateUi(QMainWindow *DBMSClass)
    {
        DBMSClass->setWindowTitle(QApplication::translate("DBMSClass", "DBMS", nullptr));
        exit->setText(QApplication::translate("DBMSClass", "\351\200\200\345\207\272", nullptr));
        newDb->setText(QApplication::translate("DBMSClass", "\346\226\260\345\273\272\345\272\223", nullptr));
        refresh->setText(QApplication::translate("DBMSClass", "\345\210\267\346\226\260", nullptr));
        deleteDb->setText(QApplication::translate("DBMSClass", "\345\210\240\351\231\244\345\272\223", nullptr));
        newTable->setText(QApplication::translate("DBMSClass", "\346\226\260\345\273\272\350\241\250", nullptr));
        deleteTable->setText(QApplication::translate("DBMSClass", "\345\210\240\351\231\244\350\241\250", nullptr));
        newField->setText(QApplication::translate("DBMSClass", "\346\226\260\345\273\272\345\255\227\346\256\265", nullptr));
        deleteField->setText(QApplication::translate("DBMSClass", "\345\210\240\351\231\244\345\255\227\346\256\265", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = tree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("DBMSClass", "\346\225\260\346\215\256\345\272\223", nullptr));
        systemMenu->setTitle(QApplication::translate("DBMSClass", "\347\263\273\347\273\237", nullptr));
        databaseMenu->setTitle(QApplication::translate("DBMSClass", "\346\225\260\346\215\256\345\272\223", nullptr));
        tableMenu->setTitle(QApplication::translate("DBMSClass", "\350\241\250", nullptr));
        fieldMenu->setTitle(QApplication::translate("DBMSClass", "\345\255\227\346\256\265", nullptr));
        recordMenu->setTitle(QApplication::translate("DBMSClass", "\350\256\260\345\275\225", nullptr));
        helpMenu->setTitle(QApplication::translate("DBMSClass", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DBMSClass: public Ui_DBMSClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBMS_H
