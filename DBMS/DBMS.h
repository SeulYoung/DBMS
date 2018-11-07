#pragma once

#include <QtWidgets/QMainWindow>
#include <QTextCursor.h>
#include "ui_DBMS.h"

using namespace std;

class DBMS : public QMainWindow
{
	Q_OBJECT

public:
	DBMS(QWidget *parent = Q_NULLPTR);

private:
	Ui::DBMSClass ui;

public slots:
void cmdCursorPositionChanged();
	void getCmd();
};
