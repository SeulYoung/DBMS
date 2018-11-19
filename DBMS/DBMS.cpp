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
	/*QTreeWidgetItem* B = new QTreeWidgetItem(QStringList() << "B");
	QTreeWidgetItem* C = new QTreeWidgetItem(QStringList() << "C");
	ui.tree->addTopLevelItem(A);
	ui.tree->addTopLevelItem(B);
	ui.tree->addTopLevelItem(C);*/
	for (int j = 0; j < 3; j++)
	{
		QTreeWidgetItem* A = new QTreeWidgetItem(QStringList() << "A");
		for (int i = 0; i < 3; ++i)
		{
			QStringList columItemList;
			QTreeWidgetItem *child;
			QString key, value;
			key += "a" + QString::number(i);
			value += QString::number(i);
			columItemList << key << value;
			child = new QTreeWidgetItem(columItemList);
			A->addChild(child);
		}
		ui.tree->addTopLevelItem(A);
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
