#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DBMS.h"

class DBMS : public QMainWindow
{
	Q_OBJECT

public:
	DBMS(QWidget *parent = Q_NULLPTR);

private:
	Ui::DBMSClass ui;
};
