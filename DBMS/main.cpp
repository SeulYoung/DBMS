#include "DBMS.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DBMS w;
	w.show();
	return a.exec();
}
