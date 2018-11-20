#pragma once

#include <QtWidgets/QMainWindow>
#include <QTextCursor.h>
#include "ui_DBMS.h"
#include "CmdParse.h"

class DBMS : public QMainWindow
{
	Q_OBJECT

public:
	DBMS(QWidget *parent = Q_NULLPTR);

private:
	void initTree();

	Ui::DBMSClass ui;
	CmdParse cp;
	string sql;

public slots:
	void getCmd();
	void menuClicked();
	void treeClicked(QTreeWidgetItem *item, int col);
};
