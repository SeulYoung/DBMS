#include "DBMS.h"

DBMS::DBMS(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

/*void DBMS:: cmdCursorPositionChanged()
{
	//当前光标
	qDebug() << "coming" << endl;           //可以看到行号随着光标的改变而改变

	QTextCursor tc = ui.text->textCursor();
	QTextLayout *pLayout = tc.block().layout();
	//当前光标在本BLOCK内的相对位置
	int nCurpos = tc.position() - tc.block().position();
	int nTextline = pLayout->lineForTextPosition(nCurpos).lineNumber() + tc.block().firstLineNumber();
	qDebug() << nTextline << endl;           //可以看到行号随着光标的改变而改变
}*/
