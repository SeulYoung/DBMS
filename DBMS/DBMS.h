#pragma once

#include <QtWidgets/QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <set>
#include "ui_DBMS.h"
#include "CmdParse.h"

class DBMS : public QMainWindow
{
	Q_OBJECT

public:
	DBMS(QWidget *parent = Q_NULLPTR);

private:
	void initTree();
	void disConnAll();
	void closeEvent(QCloseEvent *event);

	Ui::DBMSClass ui;
	CmdParse cp;
	string sql;

public slots:
	void getCmd();
	void treeClicked(QTreeWidgetItem *item, int col);
	void tableChanged(QTableWidgetItem *item);
	void sysAction();
	void dbAction();
	void tableAction();
	void fieldAction();
};
