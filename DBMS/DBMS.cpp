#include "DBMS.h"

DBMS::DBMS(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->width(), this->height());
	ui.table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	tableMenu = new QMenu();
	isCreateTable = false;
	isPriKey = false;
	addRecNum = 0;

	connect(ui.refresh, SIGNAL(triggered()), this, SLOT(sysAction()));
	connect(ui.commit, SIGNAL(triggered()), this, SLOT(sysAction()));
	connect(ui.undo, SIGNAL(triggered()), this, SLOT(sysAction()));
	connect(ui.redo, SIGNAL(triggered()), this, SLOT(sysAction()));
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
		if (add.find(db[0]) == add.end()) // 未添加
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

void DBMS::initType()
{
	QTreeWidgetItem *parent = ui.tree->currentItem()->parent();
	if (parent == NULL)
		return;

	ifstream in("./data/" + parent->text(0).toStdString() + "/" + ui.tree->currentItem()->text(0).toStdString() + ".tdf");
	if (!in.is_open())
		ui.cmdLine->append(QString::fromLocal8Bit("错误，未找到字段信息"));

	charType.clear();
	char buff[512];
	while (true)
	{
		in.getline(buff, sizeof(buff));
		strtok(buff, " ");
		char *n = strtok(NULL, " ");
		if (n == NULL)
			break;
		char *t = strtok(NULL, " ");
		if (string(t) == "varchar")
			charType.insert(n);
	}
	in.close();
}

void DBMS::disConnAll()
{
	disconnect(ui.newDb, SIGNAL(triggered()), this, SLOT(dbAction()));
	disconnect(ui.deleteDb, SIGNAL(triggered()), this, SLOT(dbAction()));

	disconnect(ui.newTable, SIGNAL(triggered()), this, SLOT(tableAction()));
	disconnect(ui.deleteTable, SIGNAL(triggered()), this, SLOT(tableAction()));
	disconnect(ui.editTable, SIGNAL(triggered()), this, SLOT(tableAction()));
	disconnect(ui.insertTable, SIGNAL(triggered()), this, SLOT(insertField()));
	disconnect(ui.saveTabel, SIGNAL(triggered()), this, SLOT(saveTable()));
	disconnect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));

	disconnect(ui.newField, SIGNAL(triggered()), this, SLOT(fieldAction()));
	disconnect(ui.deleteField, SIGNAL(triggered()), this, SLOT(fieldAction()));
	disconnect(ui.editField, SIGNAL(triggered()), this, SLOT(fieldAction()));
	disconnect(ui.saveField, SIGNAL(triggered()), this, SLOT(fieldAction()));

	disconnect(ui.newRec, SIGNAL(triggered()), this, SLOT(recordAction()));
	disconnect(ui.deleteRec, SIGNAL(triggered()), this, SLOT(recordAction()));
	disconnect(ui.saveRec, SIGNAL(triggered()), this, SLOT(saveRecord()));
	disconnect(ui.table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(recordChanged(QTableWidgetItem*)));
}

void DBMS::clearTable()
{
	ui.table->clear();
	while (ui.table->rowCount() > 0)
		ui.table->removeRow(0);
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
	isCreateTable = false;
	isPriKey = false;
	QTreeWidgetItem *parent = item->parent();
	if (parent == NULL)
	{
		connect(ui.newDb, SIGNAL(triggered()), this, SLOT(dbAction()));
		connect(ui.deleteDb, SIGNAL(triggered()), this, SLOT(dbAction()));

		connect(ui.newTable, SIGNAL(triggered()), this, SLOT(tableAction()));
	}
	else if (item->childCount() == 0)
	{
		if (parent->parent() == NULL)
			return;
		string d = parent->parent()->text(0).toStdString();
		string t = parent->text(0).toStdString();
		string f = item->text(0).toStdString();
		vector<vector<string>> field = cp.getField(d, t, f);

		clearTable();
		if (field.size() == 1)
			ui.table->setRowCount(1);
		else
			ui.table->setRowCount(field.size() - 1);
		ui.table->setColumnCount(4);
		QStringList header{ QString::fromLocal8Bit("列名"), QString::fromLocal8Bit("类型"), QString::fromLocal8Bit("约束名"), QString::fromLocal8Bit("约束内容") };
		ui.table->setHorizontalHeaderLabels(header);

		if (field.empty())
			return;
		ui.table->setItem(0, 0, new QTableWidgetItem(QString::fromStdString(field[0][0])));
		ui.table->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(field[0][1])));
		for (int i = 1; i < field.size(); i++)
			for (int j = 0; j < 2; j++)
				ui.table->setItem(i - 1, j + 2, new QTableWidgetItem(QString::fromStdString(field[i][j])));

		connect(ui.newField, SIGNAL(triggered()), this, SLOT(fieldAction()));
		connect(ui.deleteField, SIGNAL(triggered()), this, SLOT(fieldAction()));
		connect(ui.editField, SIGNAL(triggered()), this, SLOT(fieldAction()));
		connect(ui.saveField, SIGNAL(triggered()), this, SLOT(fieldAction()));
	}
	else
	{
		string d = parent->text(0).toStdString();
		string t = item->text(0).toStdString();
		vector<vector<string>> table = cp.getTableInfo(d, t);

		clearTable();
		if (table.empty())
			return;
		ui.table->setRowCount(table.size() - 1);
		ui.table->setColumnCount(table[0].size());
		QStringList header;
		for (auto s : table[0])
			header << QString::fromStdString(s);
		ui.table->setHorizontalHeaderLabels(header);
		for (int i = 1; i < table.size(); i++)
			for (int j = 0; j < table[i].size(); j++)
			{
				string s = table[i][j];
				if (s[0] == '\'')
					s = s.substr(1, s.size() - 2);
				ui.table->setItem(i - 1, j, new QTableWidgetItem(QString::fromStdString(s)));
			}
		initType();

		connect(ui.newTable, SIGNAL(triggered()), this, SLOT(tableAction()));
		connect(ui.deleteTable, SIGNAL(triggered()), this, SLOT(tableAction()));
		connect(ui.editTable, SIGNAL(triggered()), this, SLOT(tableAction()));
		connect(ui.insertTable, SIGNAL(triggered()), this, SLOT(insertField()));
		connect(ui.saveTabel, SIGNAL(triggered()), this, SLOT(saveTable()));

		connect(ui.newRec, SIGNAL(triggered()), this, SLOT(recordAction()));
		connect(ui.deleteRec, SIGNAL(triggered()), this, SLOT(recordAction()));
		connect(ui.saveRec, SIGNAL(triggered()), this, SLOT(saveRecord()));
		connect(ui.table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(recordChanged(QTableWidgetItem*)));
	}
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
		else if (treeItem->childCount() == 0 && parent->parent() != NULL)
		{
			tableMenu->addAction(ui.newField);
			tableMenu->addAction(ui.deleteField);
			tableMenu->addSeparator();
			tableMenu->addAction(ui.editField);
			tableMenu->addAction(ui.saveField);

			connect(ui.newField, SIGNAL(triggered()), this, SLOT(fieldAction()));
			connect(ui.deleteField, SIGNAL(triggered()), this, SLOT(fieldAction()));
			connect(ui.editField, SIGNAL(triggered()), this, SLOT(fieldAction()));
			connect(ui.saveField, SIGNAL(triggered()), this, SLOT(fieldAction()));
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
			tableMenu->addAction(ui.newRec);
			tableMenu->addAction(ui.deleteRec);
			tableMenu->addAction(ui.saveRec);

			connect(ui.newTable, SIGNAL(triggered()), this, SLOT(tableAction()));
			connect(ui.deleteTable, SIGNAL(triggered()), this, SLOT(tableAction()));
			connect(ui.editTable, SIGNAL(triggered()), this, SLOT(tableAction()));
			connect(ui.insertTable, SIGNAL(triggered()), this, SLOT(insertField()));
			connect(ui.saveTabel, SIGNAL(triggered()), this, SLOT(saveTable()));

			if (!isCreateTable)
			{
				connect(ui.newRec, SIGNAL(triggered()), this, SLOT(recordAction()));
				connect(ui.deleteRec, SIGNAL(triggered()), this, SLOT(recordAction()));
				connect(ui.saveRec, SIGNAL(triggered()), this, SLOT(saveRecord()));
			}
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
	else if (s == "commit")
	{
		for (auto s : preSql)
		{
			string res = cp.sqlCheck(s);
			ui.cmdLine->append(QString::fromLocal8Bit(res.c_str()));
		}
		preSql.clear();
		backup.clear();
	}
	else if (s == "undo")
	{
		if (!preSql.empty())
		{
			backup.push_back(preSql.back());
			preSql.pop_back();
			ui.cmdLine->append(QString::fromLocal8Bit(("撤销：" + backup.back()).c_str()));
		}
	}
	else if (s == "redo")
	{
		if (!backup.empty())
		{
			preSql.push_back(backup.back());
			backup.pop_back();
			ui.cmdLine->append(QString::fromLocal8Bit(("重做：" + preSql.back()).c_str()));
		}
	}
	else
		this->close();
}

void DBMS::dbAction()
{
	QString s = sender()->objectName();
	if (s == "newDb")
	{
		disconnect(ui.table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(recordChanged(QTableWidgetItem*)));
		QTreeWidgetItem *db = new QTreeWidgetItem(ui.tree, QStringList() << QString::fromLocal8Bit("输入库名"));
		db->setFlags(db->flags() | Qt::ItemIsEditable);
		ui.tree->editItem(db);
		connect(ui.tree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(createDb(QTreeWidgetItem*, int)));
	}
	else
	{
		QTreeWidgetItem *db = ui.tree->currentItem();
		if (db != NULL)
		{
			string ss = "drop database " + db->text(0).toStdString() + ";";
			while (db->childCount() > 0)
			{
				QTreeWidgetItem *table = db->takeChild(0);
				while (table->childCount() > 0)
					delete table->takeChild(0);
				delete table;
			}
			ui.tree->takeTopLevelItem(ui.tree->currentIndex().row());
			delete db;
			preSql.push_back(ss);
		}
	}
}

void DBMS::createDb(QTreeWidgetItem *item, int col)
{
	disconnect(ui.tree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(createDb(QTreeWidgetItem*, int)));
	item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	string s = "create database " + item->text(0).toStdString() + ";";
	preSql.push_back(s);
}

void DBMS::tableAction()
{
	QString s = sender()->objectName();
	QTreeWidgetItem *parent = ui.tree->currentItem()->parent();
	if (parent == NULL)
		parent = ui.tree->currentItem();
	if (parent->text(0).toStdString() != cp.getDbName())
		cp.setDbName(parent->text(0).toStdString());
	if (s == "newTable")
	{
		disconnect(ui.table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(recordChanged(QTableWidgetItem*)));
		QTreeWidgetItem *table = new QTreeWidgetItem(parent, QStringList() << QString::fromLocal8Bit("输入表名"));
		table->setFlags(table->flags() | Qt::ItemIsEditable);
		ui.tree->scrollToItem(table);
		ui.tree->editItem(table);
		isCreateTable = true;
		connect(ui.tree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(createTable(QTreeWidgetItem*, int)));
	}
	else if (s == "deleteTable")
	{
		QTreeWidgetItem *table = ui.tree->currentItem();
		if (table != NULL)
		{
			string ss = "drop table " + table->text(0).toStdString() + ";";
			while (table->childCount() > 0)
			{
				QTreeWidgetItem *field = table->takeChild(0);
				delete field;
			}
			delete table;
			preSql.push_back(ss);
		}
	}
	else
	{
		QTableWidgetItem *item = ui.table->currentItem();
		if (item != NULL)
			ui.table->editItem(item);
	}
}

void DBMS::createTable(QTreeWidgetItem *item, int col)
{
	disconnect(ui.tree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(createTable(QTreeWidgetItem*, int)));
	item->setFlags(item->flags() & ~Qt::ItemIsEditable);

	clearTable();
	ui.table->setColumnCount(7);
	QStringList header{ QString::fromLocal8Bit("列名"),
						QString::fromLocal8Bit("类型"),
						QString::fromLocal8Bit("长度"),
						QString::fromLocal8Bit("主键"),
						QString::fromLocal8Bit("唯一"),
						QString::fromLocal8Bit("非空"),
						QString::fromLocal8Bit("其他约束") };
	ui.table->setHorizontalHeaderLabels(header);
	insertField();

	connect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));
}

void DBMS::insertField()
{
	if (isCreateTable)
	{
		disconnect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));
		ui.table->insertRow(ui.table->rowCount());

		ui.table->setItem(ui.table->rowCount() - 1, 0, new QTableWidgetItem());
		QStringList typeList{ "varchar", "integer", "bool", "double", "datetime" };
		QComboBox *type = new QComboBox();
		type->addItems(typeList);
		connect(type, SIGNAL(currentIndexChanged(int)), SLOT(typeCheck(int)));
		ui.table->setCellWidget(ui.table->rowCount() - 1, 1, type);
		ui.table->setItem(ui.table->rowCount() - 1, 2, new QTableWidgetItem());

		QTableWidgetItem *checkPri = new QTableWidgetItem();
		checkPri->setCheckState(Qt::Unchecked);
		if (isPriKey)
			checkPri->setFlags(Qt::NoItemFlags);
		ui.table->setItem(ui.table->rowCount() - 1, 3, checkPri);
		QTableWidgetItem *checkUni = new QTableWidgetItem();
		checkUni->setCheckState(Qt::Unchecked);
		ui.table->setItem(ui.table->rowCount() - 1, 4, checkUni);
		QTableWidgetItem *checkNull = new QTableWidgetItem();
		checkNull->setCheckState(Qt::Unchecked);
		ui.table->setItem(ui.table->rowCount() - 1, 5, checkNull);

		ui.table->setItem(ui.table->rowCount() - 1, 6, new QTableWidgetItem());
		connect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));
	}
	else
	{
		clearTable();
		ui.table->setColumnCount(3);
		QStringList header{ QString::fromLocal8Bit("列名"),
							QString::fromLocal8Bit("类型"),
							QString::fromLocal8Bit("长度") };
		ui.table->setHorizontalHeaderLabels(header);
		ui.table->insertRow(ui.table->rowCount());

		ui.table->setItem(ui.table->rowCount() - 1, 0, new QTableWidgetItem());
		QStringList typeList{ "integer", "bool", "double", "varchar", "datetime" };
		QComboBox *type = new QComboBox();
		type->addItems(typeList);
		ui.table->setCellWidget(ui.table->rowCount() - 1, 1, type);
		ui.table->setItem(ui.table->rowCount() - 1, 2, new QTableWidgetItem());
	}
}

void DBMS::consCheck(int row, int col)
{
	if (ui.table->item(row, col)->checkState() == Qt::Checked) // 选中
	{
		disconnect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));
		if (col == 3)
		{
			ui.table->item(row, 4)->setFlags(Qt::NoItemFlags);
			ui.table->item(row, 5)->setFlags(Qt::NoItemFlags);
			if (!isPriKey)
			{
				for (int i = 0; i < ui.table->rowCount(); i++)
					if (i != row)
						ui.table->item(i, col)->setFlags(Qt::NoItemFlags);
				isPriKey = true;
			}
		}
		else if (col == 4 || col == 5)
			ui.table->item(row, 3)->setFlags(Qt::NoItemFlags);
		connect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));
	}
	else if (col == 3 && ui.table->item(row, col)->checkState() == Qt::Unchecked) // 未选中
	{
		disconnect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));
		ui.table->item(row, 4)->setFlags((Qt::ItemFlags)63);
		ui.table->item(row, 5)->setFlags((Qt::ItemFlags)63);
		if (isPriKey)
		{
			for (int i = 0; i < ui.table->rowCount(); i++)
				if (ui.table->item(i, 4)->checkState() == Qt::Unchecked && ui.table->item(i, 5)->checkState() == Qt::Unchecked)
					ui.table->item(i, col)->setFlags((Qt::ItemFlags)63);
			isPriKey = false;
		}
		connect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));
	}
	else if (!isPriKey)
		if ((col == 4 || col == 5) && ui.table->item(row, 4)->checkState() == Qt::Unchecked && ui.table->item(row, 5)->checkState() == Qt::Unchecked) // 未选中
		{
			disconnect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));
			ui.table->item(row, 3)->setFlags((Qt::ItemFlags)63);
			connect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));
		}
}

/*void DBMS::typeCheck(int index)
{
	disconnect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));
	QComboBox *type = (QComboBox*)sender();
	if (type->currentText().toStdString() != "varchar")
		ui.table->item(row, col + 1)->setFlags(Qt::ItemIsEnabled);
	else
		ui.table->item(row, col + 1)->setFlags((Qt::ItemFlags)63);
	connect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));
}*/

void DBMS::saveTable()
{
	if (isCreateTable)
	{
		disconnect(ui.table, SIGNAL(cellChanged(int, int)), this, SLOT(consCheck(int, int)));
		string s = "create table " + ui.tree->currentItem()->text(0).toStdString() + "(";
		for (int i = 0; i < ui.table->rowCount(); i++)
		{
			if (ui.table->item(i, 0)->text() == "")
			{
				ui.cmdLine->append(QString::fromLocal8Bit("错误：列名不能为空"));
				return;
			}
			s += ui.table->item(i, 0)->text().toStdString() + " ";

			QComboBox *type = (QComboBox *)ui.table->cellWidget(i, 1);
			s += type->currentText().toStdString();

			if (ui.table->item(i, 2)->text() != "")
				s += "(" + ui.table->item(i, 2)->text().toStdString() + ")";
			if (ui.table->item(i, 3)->checkState() == Qt::Checked)
				s += " primary key";
			else
			{
				if (ui.table->item(i, 4)->checkState() == Qt::Checked)
					s += " unique";
				if (ui.table->item(i, 5)->checkState() == Qt::Checked)
					s += " not null";
			}
			if (ui.table->item(i, 6)->text() != "")
				s += " " + ui.table->item(i, 6)->text().toStdString();
			s += ",";
		}
		s = s.substr(0, s.size() - 1) + ");";
		preSql.push_back(s);

		isCreateTable = false;
		isPriKey = false;
	}
	else
	{
		string s = "alter table " + ui.tree->currentItem()->text(0).toStdString() + " add column ";
		for (int i = 0; i < ui.table->rowCount(); i++)
		{
			if (ui.table->item(i, 0)->text() == "")
			{
				ui.cmdLine->append(QString::fromLocal8Bit("错误：列名不能为空"));
				return;
			}
			s += ui.table->item(i, 0)->text().toStdString() + " ";

			QComboBox *type = (QComboBox *)ui.table->cellWidget(i, 1);
			s += type->currentText().toStdString();

			if (ui.table->item(i, 2)->text() != "")
				s += "(" + ui.table->item(i, 2)->text().toStdString() + ")";
			s += ",";
		}
		s = s.substr(0, s.size() - 1) + ";";
		preSql.push_back(s);
	}
}

void DBMS::fieldAction()
{
	QString s = sender()->objectName();
	QTreeWidgetItem *parent = ui.tree->currentItem()->parent();
	if (parent->text(0).toStdString() != cp.getDbName())
		cp.setDbName(parent->parent()->text(0).toStdString());
	if (s == "newField")
	{
		disconnect(ui.table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(recordChanged(QTableWidgetItem*)));
		insertField();
	}
	else if(s=="deleteField")
	{
		QTreeWidgetItem *item = ui.tree->currentItem();
		string ss = "alter table " + parent->text(0).toStdString() + " drop column " + item->text(0).toStdString() + ";";
		delete(item);
		preSql.push_back(ss);
	}
	else if (s == "editField")
	{
		QTableWidgetItem *item = ui.table->currentItem();
		if (item != NULL)
			ui.table->editItem(item);
	}
	else
	{
		string s = "alter table " + ui.tree->currentItem()->parent()->text(0).toStdString() + " add column ";
		for (int i = 0; i < ui.table->rowCount(); i++)
		{
			if (ui.table->item(i, 0)->text() == "")
			{
				ui.cmdLine->append(QString::fromLocal8Bit("错误：列名不能为空"));
				return;
			}
			s += ui.table->item(i, 0)->text().toStdString() + " ";

			QComboBox *type = (QComboBox *)ui.table->cellWidget(i, 1);
			s += type->currentText().toStdString();

			if (ui.table->item(i, 2)->text() != "")
				s += "(" + ui.table->item(i, 2)->text().toStdString() + ")";
			s += ",";
		}
		s = s.substr(0, s.size() - 1) + ";";
		preSql.push_back(s);
	}
}

void DBMS::recordAction()
{
	QString s = sender()->objectName();
	QTreeWidgetItem *parent = ui.tree->currentItem()->parent();
	if (parent->text(0).toStdString() != cp.getDbName())
		cp.setDbName(parent->text(0).toStdString() + ";");
	if (s == "newRec")
	{
		disconnect(ui.table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(recordChanged(QTableWidgetItem*)));
		ui.table->insertRow(ui.table->rowCount());
		for (int i = 0; i < ui.table->columnCount(); i++)
			ui.table->setItem(ui.table->rowCount() - 1, i, new QTableWidgetItem());
		addRecNum++;
		connect(ui.saveRec, SIGNAL(triggered()), this, SLOT(saveRecord()));
	}
	else
	{
		disconnect(ui.table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(recordChanged(QTableWidgetItem*)));
		string ss = "delete from " + ui.tree->currentItem()->text(0).toStdString() + " where";
		for (int i = 0; i < ui.table->columnCount(); i++)
		{
			string col = ui.table->horizontalHeaderItem(i)->text().toStdString();
			if (charType.find(col) == charType.end()) // 类型不为字符串
				ss += " " + col + "=" + ui.table->item(ui.table->currentRow(), i)->text().toStdString() + " and";
			else
				ss += " " + col + "=\'" + ui.table->item(ui.table->currentRow(), i)->text().toStdString() + "\' and";
		}
		ui.table->removeRow(ui.table->currentRow());

		ss = ss.substr(0, ss.size() - 4) + ";";
		preSql.push_back(ss);
	}
}

void DBMS::recordChanged(QTableWidgetItem *item)
{
	string s = "update " + ui.tree->currentItem()->text(0).toStdString() + " set ";
	string col = ui.table->horizontalHeaderItem(item->column())->text().toStdString();
	if (charType.find(col) == charType.end()) // 类型不为字符串
		s += col + "=" + item->text().toStdString() + " where";
	else
		s += col + "=\'" + item->text().toStdString() + "\' where";

	for (int i = 0; i < ui.table->columnCount(); i++)
		if (i != item->column())
		{
			col = ui.table->horizontalHeaderItem(i)->text().toStdString();
			if (charType.find(col) == charType.end()) // 类型不为字符串
				s += " " + col + "=" + ui.table->item(item->row(), i)->text().toStdString() + " and";
			else
				s += " " + col + "=\'" + ui.table->item(item->row(), i)->text().toStdString() + "\' and";
		}

	s = s.substr(0, s.size() - 4) + ";";
	preSql.push_back(s);
}

void DBMS::saveRecord()
{
	disconnect(ui.saveRec, SIGNAL(triggered()), this, SLOT(saveRecord()));

	for (; addRecNum > 0; addRecNum--)
	{
		string s = "insert into " + ui.tree->currentItem()->text(0).toStdString() + "(";
		string v = " values(";
		for (int i = 0; i < ui.table->columnCount(); i++)
		{
			QTableWidgetItem *item = ui.table->item(ui.table->rowCount() - addRecNum, i);
			if (item->text() != "")
			{
				s += ui.table->horizontalHeaderItem(i)->text().toStdString() + ",";
				if (charType.find(ui.table->horizontalHeaderItem(i)->text().toStdString()) == charType.end()) // 类型不为字符串
					v += item->text().toStdString() + ",";
				else
					v += "\'" + item->text().toStdString() + "\'" + ",";
			}
		}
		s = s.substr(0, s.size() - 1) + ") " + v.substr(0, v.size() - 1) + ");";
		preSql.push_back(s);
	}
	connect(ui.table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(recordChanged(QTableWidgetItem*)));
}
