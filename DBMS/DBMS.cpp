#include "DBMS.h"

DBMS::DBMS(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->width(), this->height());

	//initTree();
}

void DBMS::initTree()
{
	vector<vector<string>> dbs = cp.getDbs();
	for (auto db : dbs)
	{
		QTreeWidgetItem *root = new QTreeWidgetItem(ui.tree, QStringList() << QString::fromStdString(db[0]));
		QTreeWidgetItem *table = new QTreeWidgetItem(root, QStringList() << QString::fromStdString(db[1]));
		for (int i = 2; i < db.size(); i++)
			QTreeWidgetItem *col = new QTreeWidgetItem(table, QStringList() << QString::fromStdString(db[i]));
	}
}

void DBMS::getCmd()
{
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
	QTreeWidgetItem *parent = item->parent();
	if (parent == NULL)
		return;
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
	}
}
