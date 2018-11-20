#include "DBMS.h"

DBMS::DBMS(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->width(), this->height());

	initTree();
}

void DBMS::initTree()
{
	for (int j = 0; j < 3; j++)
	{
		QTreeWidgetItem *db = new QTreeWidgetItem(ui.tree, QStringList() << "db");
		for (int i = 0; i < 3; ++i)
		{
			QTreeWidgetItem *table = new QTreeWidgetItem(db, QStringList() << "table");
		}
	}
}

void DBMS::getCmd()
{
	QString s = ui.inputLine->text();
	ui.cmdLine->append(s);
	sql += s.toStdString();
	if (s[s.size() - 1] == ';')
	{
		CmdParse cp(sql);
		string s = cp.sqlCheck();
		ui.cmdLine->append(QString::fromLocal8Bit(s.c_str()));
		sql = "";
	}
	ui.inputLine->clear();
}

void DBMS::menuClicked()
{
	QAction *qa = ui.menuBar->activeAction();
	QString action = qa->text();

	if (action == "ÍË³ö")
	{
		exit(0);
	}
}

void DBMS::treeClicked(QTreeWidgetItem *item, int col)
{
	if (item->parent() == NULL)
		return;
	else if (item->childCount() == 0)
	{
		QTreeWidgetItem *table = item->parent();
		ui.table->setColumnCount(3);
		ui.table->setRowCount(3);
		ui.table->setHorizontalHeaderLabels(QStringList() << "col1" << "col2" << "col3");
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				ui.table->setItem(i, j, new QTableWidgetItem("item"));
			}
	}
	else
	{

	}
}
