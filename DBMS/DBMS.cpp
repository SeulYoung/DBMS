#include "DBMS.h"

DBMS::DBMS(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->width(), this->height());

	tableMenu = new QMenu();

	connect(ui.refresh, SIGNAL(triggered()), this, SLOT(sysAction()));
	connect(ui.exit, SIGNAL(triggered()), this, SLOT(sysAction()));
	connect(ui.newDb, SIGNAL(triggered()), this, SLOT(dbAction()));
	initTree();
}

void DBMS::initTree()
{
	ui.tree->clear();

	vector<vector<string>> dbs = cp.getDbs();
	set<string> add;
	QTreeWidgetItem *root;
	for (auto db : dbs)
	{
		// 数据库节点
		if (add.find(db[0]) == add.end())
		{
			add.insert(db[0]);
			root = new QTreeWidgetItem(ui.tree, QStringList() << QString::fromStdString(db[0]));
		}
		// 表节点
		if (db.size() > 1)
		{
			QTreeWidgetItem *table = new QTreeWidgetItem(root, QStringList() << QString::fromStdString(db[1]));
			// 字段
			for (int i = 2; i < db.size(); i++)
				QTreeWidgetItem *col = new QTreeWidgetItem(table, QStringList() << QString::fromStdString(db[i]));
		}
	}
}

void DBMS::disConnAll()
{
	disconnect(ui.deleteDb, SIGNAL(triggered()), this, SLOT(dbAction()));
	disconnect(ui.newTable, SIGNAL(triggered()), this, SLOT(tableAction()));
	disconnect(ui.deleteTable, SIGNAL(triggered()), this, SLOT(tableAction()));
	disconnect(ui.newField, SIGNAL(triggered()), this, SLOT(fieldAction()));
	disconnect(ui.deleteField, SIGNAL(triggered()), this, SLOT(fieldAction()));
}

void DBMS::closeEvent(QCloseEvent *event)
{
	switch (QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("你确定退出该软件？"), QString::fromLocal8Bit("确定"), QString::fromLocal8Bit("取消"), 0, 1))
	{
	case 0:
		event->accept();
		break;
	case 1:
		event->ignore();
		break;
	}
}

void DBMS::getCmd()
{
	disConnAll();
	QString s = ui.inputLine->text();
	ui.cmdLine->append(s);
	sql += s.toStdString();
	if (s[s.size() - 1] == ';')
	{
		string s = cp.sqlCheck(sql);
		ui.cmdLine->append(QString::fromLocal8Bit(s.c_str()));
		sql = "";
	}
	ui.inputLine->clear();
}

void DBMS::treeClicked(QTreeWidgetItem *item, int col)
{
	disConnAll();
	QTreeWidgetItem *parent = item->parent();
	if (parent == NULL)
	{
		connect(ui.deleteDb, SIGNAL(triggered()), this, SLOT(dbAction()));
		connect(ui.newTable, SIGNAL(triggered()), this, SLOT(tableAction()));
	}
	else if (item->childCount() == 0)
	{
		string d = parent->parent()->text(0).toStdString();
		string t = parent->text(0).toStdString();
		string f = item->text(0).toStdString();
		vector<vector<string>> field = cp.getField(d, t, f);

		ui.table->setRowCount(field.size());
		ui.table->setColumnCount(3);
		QStringList header{ QString::fromLocal8Bit("约束名"), QString::fromLocal8Bit("列名"), QString::fromLocal8Bit("约束内容") };
		ui.table->setHorizontalHeaderLabels(header);
		for (int i = 0; i < field.size(); i++)
			for (int j = 0; j < 3; j++)
				ui.table->setItem(i, j, new QTableWidgetItem(QString::fromStdString(field[i][j])));

		connect(ui.newField, SIGNAL(triggered()), this, SLOT(fieldAction()));
		connect(ui.deleteField, SIGNAL(triggered()), this, SLOT(fieldAction()));
	}
	else
	{
		string d = parent->text(0).toStdString();
		string t = item->text(0).toStdString();
		vector<vector<string>> table = cp.getTableInfo(d, t);

		ui.table->setRowCount(table.size());
		ui.table->setColumnCount(table[0].size());
		QStringList header;
		for (auto s : table[0])
			header << QString::fromStdString(s);
		ui.table->setHorizontalHeaderLabels(header);
		for (int i = 1; i < table.size(); i++)
			for (int j = 0; j < table[i].size(); j++)
				ui.table->setItem(i - 1, j, new QTableWidgetItem(QString::fromStdString(table[i][j])));

		connect(ui.newTable, SIGNAL(triggered()), this, SLOT(tableAction()));
		connect(ui.deleteTable, SIGNAL(triggered()), this, SLOT(tableAction()));
		connect(ui.newField, SIGNAL(triggered()), this, SLOT(fieldAction()));
	}
}

void DBMS::tableChanged(QTableWidgetItem *item)
{
}

void DBMS::contextMenuEvent(QContextMenuEvent *event)
{
	tableMenu->clear();
	QPoint point = event->pos(); // 得到窗口坐标
	QTableWidgetItem *tableItem = ui.table->itemAt(point);
	QTreeWidgetItem *treeItem = ui.tree->currentItem();

	if (tableItem != NULL)
	{


		// 菜单出现的位置为当前鼠标的位置
		tableMenu->exec(QCursor::pos());
		event->accept();
	}
	else if (treeItem != NULL)
	{
		disConnAll();
		QTreeWidgetItem *parent = treeItem->parent();
		if (parent == NULL)
		{
			tableMenu->addAction(ui.newDb);
			tableMenu->addAction(ui.deleteDb);
			tableMenu->addSeparator();
			tableMenu->addAction(ui.newTable);

			connect(ui.deleteDb, SIGNAL(triggered()), this, SLOT(dbAction()));
			connect(ui.newTable, SIGNAL(triggered()), this, SLOT(tableAction()));
		}
		else if (treeItem->childCount() == 0)
		{
			tableMenu->addAction(ui.newDb);
			tableMenu->addSeparator();
			tableMenu->addAction(ui.newField);
			tableMenu->addAction(ui.deleteField);

			connect(ui.newField, SIGNAL(triggered()), this, SLOT(fieldAction()));
			connect(ui.deleteField, SIGNAL(triggered()), this, SLOT(fieldAction()));
		}
		else
		{
			tableMenu->addAction(ui.newDb);
			tableMenu->addSeparator();
			tableMenu->addAction(ui.newTable);
			tableMenu->addAction(ui.deleteTable);
			tableMenu->addSeparator();
			tableMenu->addAction(ui.newField);

			connect(ui.newTable, SIGNAL(triggered()), this, SLOT(tableAction()));
			connect(ui.deleteTable, SIGNAL(triggered()), this, SLOT(tableAction()));
			connect(ui.newField, SIGNAL(triggered()), this, SLOT(fieldAction()));
		}
		// 菜单出现的位置为当前鼠标的位置
		tableMenu->exec(QCursor::pos());
		event->accept();
	}
}

void DBMS::sysAction()
{
	QString s = sender()->objectName();
	if (s == "refresh")
		initTree();
	else
		this->close();
}

void DBMS::dbAction()
{
}

void DBMS::tableAction()
{
}

void DBMS::fieldAction()
{
}
