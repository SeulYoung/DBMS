#include "DBMS.h"

DBMS::DBMS(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->width(), this->height());

	tableMenu = new QMenu();

	connect(ui.refresh, SIGNAL(triggered()), this, SLOT(sysAction()));
	connect(ui.commit, SIGNAL(triggered()), this, SLOT(sysAction()));
	connect(ui.exit, SIGNAL(triggered()), this, SLOT(sysAction()));
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
	disconnect(ui.newDb, SIGNAL(triggered()), this, SLOT(dbAction()));
	disconnect(ui.deleteDb, SIGNAL(triggered()), this, SLOT(dbAction()));

	disconnect(ui.newTable, SIGNAL(triggered()), this, SLOT(tableAction()));
	disconnect(ui.deleteTable, SIGNAL(triggered()), this, SLOT(tableAction()));
	disconnect(ui.editTable, SIGNAL(triggered()), this, SLOT(tableAction()));
	disconnect(ui.insertTable, SIGNAL(triggered()), this, SLOT(tableAction()));
	disconnect(ui.saveTabel, SIGNAL(triggered()), this, SLOT(tableAction()));

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
	disConnAll();
	tableMenu->clear();
	QPoint point = event->pos(); // 得到窗口坐标
	QTableWidgetItem *tableItem = ui.table->itemAt(point);
	QTreeWidgetItem *treeItem = ui.tree->currentItem();

	if (treeItem != NULL)
	{
		QTreeWidgetItem *parent = treeItem->parent();
		if (parent == NULL)
		{
			tableMenu->addAction(ui.newDb);
			tableMenu->addAction(ui.deleteDb);
			tableMenu->addSeparator();
			tableMenu->addAction(ui.newTable);

			connect(ui.newDb, SIGNAL(triggered()), this, SLOT(dbAction()));
			connect(ui.deleteDb, SIGNAL(triggered()), this, SLOT(dbAction()));
			connect(ui.newTable, SIGNAL(triggered()), this, SLOT(tableAction()));
		}
		else if (treeItem->childCount() == 0)
		{
			tableMenu->addAction(ui.newField);
			tableMenu->addAction(ui.deleteField);

			connect(ui.newField, SIGNAL(triggered()), this, SLOT(fieldAction()));
			connect(ui.deleteField, SIGNAL(triggered()), this, SLOT(fieldAction()));
		}
		else
		{
			tableMenu->addAction(ui.newTable);
			tableMenu->addAction(ui.deleteTable);
			tableMenu->addSeparator();
			tableMenu->addAction(ui.editTable);
			tableMenu->addAction(ui.insertTable);
			tableMenu->addAction(ui.saveTabel);
			tableMenu->addSeparator();
			tableMenu->addAction(ui.newField);

			connect(ui.newTable, SIGNAL(triggered()), this, SLOT(tableAction()));
			connect(ui.deleteTable, SIGNAL(triggered()), this, SLOT(tableAction()));
			connect(ui.editTable, SIGNAL(triggered()), this, SLOT(tableAction()));
			connect(ui.insertTable, SIGNAL(triggered()), this, SLOT(tableAction()));
			connect(ui.saveTabel, SIGNAL(triggered()), this, SLOT(tableAction()));
			connect(ui.newField, SIGNAL(triggered()), this, SLOT(fieldAction()));
		}
		// 菜单出现的位置为当前鼠标的位置
		tableMenu->exec(QCursor::pos());
		event->accept();
	}

	/*if (tableItem != NULL)
	{
		tableMenu->addAction(ui.editTable);
		tableMenu->addAction(ui.insertTable);
		tableMenu->addAction(ui.saveTabel);

		connect(ui.editTable, SIGNAL(triggered()), this, SLOT(tableAction()));

		// 菜单出现的位置为当前鼠标的位置
		tableMenu->exec(QCursor::pos());
		event->accept();
	}
	else if (treeItem != NULL)
	{
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
	}*/
}

void DBMS::sysAction()
{
	QString s = sender()->objectName();
	if (s == "refresh")
		initTree();
	else if (s == "commit")
		for (auto s : preSql)
		{
			string res = cp.sqlCheck(s);
			ui.cmdLine->append(QString::fromLocal8Bit(res.c_str()));
		}
	else
		this->close();
}

void DBMS::dbAction()
{
	QString s = sender()->objectName();
	if (s == "newDb")
	{
		QTreeWidgetItem *db = new QTreeWidgetItem(ui.tree, QStringList() << QString::fromLocal8Bit("输入库名"));
		ui.tree->editItem(db);
		connect(ui.tree, SIGNAL(returnPressed()), this, SLOT(createDb()));
	}
	else
	{
		string ss = "drop database " + ui.tree->currentItem()->text(0).toStdString() + ";";
		preSql.push_back(ss);
	}
}

void DBMS::createDb()
{
	disconnect(ui.tree, SIGNAL(returnPressed()), this, SLOT(createDb()));
	string s = "create database " + ui.tree->currentItem()->text(0).toStdString() + ";";
	preSql.push_back(s);
}

void DBMS::tableAction()
{
	QString s = sender()->objectName();
	QTreeWidgetItem *parent = ui.tree->currentItem()->parent();
	string ss = "use database " + parent->text(0).toStdString() + ";";
	preSql.push_back(ss);
	if (s == "newTable")
	{
		QTreeWidgetItem *table = new QTreeWidgetItem(parent, QStringList() << QString::fromLocal8Bit("输入表名"));
		ui.tree->editItem(table);
		connect(ui.tree, SIGNAL(returnPressed()), this, SLOT(createTable()));
	}
	else
	{
		ss = "drop table " + ui.tree->currentItem()->text(0).toStdString() + ";";
		preSql.push_back(ss);
	}
}

void DBMS::createTable()
{
	disconnect(ui.tree, SIGNAL(returnPressed()), this, SLOT(createTable()));

	ui.table->setRowCount(1);
	ui.table->setColumnCount(8);
	QStringList header{ QString::fromLocal8Bit("列名"),
						QString::fromLocal8Bit("类型"),
						QString::fromLocal8Bit("长度"),
						QString::fromLocal8Bit("主键"),
						QString::fromLocal8Bit("唯一"),
						QString::fromLocal8Bit("非空"),
						QString::fromLocal8Bit("默认值"),
						QString::fromLocal8Bit("其他约束") };
	ui.table->setHorizontalHeaderLabels(header);
	insertTableRow();
}

void DBMS::insertTableRow()
{
	ui.table->insertRow(ui.table->rowCount());

	ui.table->setItem(ui.table->rowCount() - 1, 0, new QTableWidgetItem());
	QStringList typeList{ "integer", "bool", "double", "varchar", "datetime" };
	QComboBox *type = new QComboBox();
	type->addItems(typeList);
	ui.table->setCellWidget(ui.table->rowCount() - 1, 1, type);
	ui.table->setItem(ui.table->rowCount() - 1, 2, new QTableWidgetItem());

	QTableWidgetItem *checkPri = new QTableWidgetItem();
	checkPri->setCheckState(Qt::Unchecked);
	ui.table->setItem(ui.table->rowCount() - 1, 3, checkPri);
	QTableWidgetItem *checkUni = new QTableWidgetItem();
	checkUni->setCheckState(Qt::Unchecked);
	ui.table->setItem(ui.table->rowCount() - 1, 4, checkUni);
	QTableWidgetItem *checkNull = new QTableWidgetItem();
	checkNull->setCheckState(Qt::Unchecked);
	ui.table->setItem(ui.table->rowCount() - 1, 5, checkNull);

	ui.table->setItem(ui.table->rowCount() - 1, 6, new QTableWidgetItem());
	ui.table->setItem(ui.table->rowCount() - 1, 7, new QTableWidgetItem());

	//connect();
}

void DBMS::saveTable()
{
	string s = "create table " + ui.tree->currentItem()->text(0).toStdString() + " (";
	for (int i = 0; i < ui.table->rowCount(); i++)
	{
		if (ui.table->item(i, 0)->text() == "")
		{
			ui.cmdLine->append(QString::fromLocal8Bit("错误：列名不能为空"));
			return;
		}
		s += ui.table->item(i, 0)->text().toStdString() + " ";
		s += ui.table->item(i, 1)->text().toStdString();
		if (ui.table->item(i, 2)->text() != "")
			s += "(" + ui.table->item(i, 2)->text().toStdString() + ")";
		if (ui.table->item(i, 3)->checkState() == Qt::Checked)
			s += " primary key ";
		else
		{
			if (ui.table->item(i, 4)->checkState() == Qt::Checked)
				s += " unique ";
			if (ui.table->item(i, 5)->checkState() == Qt::Checked)
				s += " not null ";
		}
		if (ui.table->item(i, 6)->text() != "")
			s += " default " + ui.table->item(i, 6)->text().toStdString();
		if (ui.table->item(i, 7)->text() != "")
			s += " " + ui.table->item(i, 7)->text().toStdString();
		s += ",";
	}
	s.substr(0, s.size() - 1);
	s += ");";
	preSql.push_back(s);
}

void DBMS::fieldAction()
{
	QString s = sender()->objectName();
	QTreeWidgetItem *parent = ui.tree->currentItem()->parent();
	string ss = "use database " + parent->parent()->text(0).toStdString() + ";";
	if (s == "newField")
	{
		ss = "alter table " + parent->text(0).toStdString() + " add column " + ui.tree->currentItem()->text(0).toStdString() + ";";
		preSql.push_back(ss);
	}
	else
	{
		ss = "alter table " + parent->text(0).toStdString() + " drop column " + ui.tree->currentItem()->text(0).toStdString() + ";";
		preSql.push_back(ss);
	}
}
