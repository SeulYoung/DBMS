#include "DBMS.h"

DBMS::DBMS(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->width(), this->height());

	connect(ui.refresh, SIGNAL(triggered()), this, SLOT(sysAction()));
	connect(ui.exit, SIGNAL(triggered()), this, SLOT(sysAction()));
	connect(ui.newDb, SIGNAL(triggered()), this, SLOT(dbAction()));
	//initTree();
}

void DBMS::initTree()
{
	vector<vector<string>> dbs = cp.getDbs();
	set<string> add;
	QTreeWidgetItem *root;
	for (auto db : dbs)
	{
		// 数据库节点
		if (add.find(db[0]) != add.end())
		{
			add.insert(db[0]);
			root = new QTreeWidgetItem(ui.tree, QStringList() << QString::fromStdString(db[0]));
		}
		// 表节点
		QTreeWidgetItem *table = new QTreeWidgetItem(root, QStringList() << QString::fromStdString(db[1]));
		// 字段
		for (int i = 2; i < db.size(); i++)
			QTreeWidgetItem *col = new QTreeWidgetItem(table, QStringList() << QString::fromStdString(db[i]));
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
	switch (QMessageBox::information(this, tr("提示"), tr("你确定退出该软件？"), tr("确定"), tr("取消"), 0, 1))
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
		string t = parent->text(parent->columnCount()).toStdString();
		string f = item->text(item->columnCount()).toStdString();
		vector<string> field = cp.getField(d, t, f);

		ui.table->setRowCount(field.size());
		ui.table->setColumnCount(1);
		ui.table->setHorizontalHeaderLabels(QStringList() << QString::fromStdString(f));
		for (int i = 0; i < field.size(); i++)
			ui.table->setItem(0, i, new QTableWidgetItem(QString::fromStdString(field[i])));

		connect(ui.newField, SIGNAL(triggered()), this, SLOT(fieldAction()));
		connect(ui.deleteField, SIGNAL(triggered()), this, SLOT(fieldAction()));
	}
	else
	{
		string d = parent->text(0).toStdString();
		string t = item->text(item->columnCount()).toStdString();
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
