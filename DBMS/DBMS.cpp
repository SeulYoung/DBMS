#include "DBMS.h"

DBMS::DBMS(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

/*void DBMS:: cmdCursorPositionChanged()
{
	//��ǰ���
	qDebug() << "coming" << endl;           //���Կ����к����Ź��ĸı���ı�

	QTextCursor tc = ui.text->textCursor();
	QTextLayout *pLayout = tc.block().layout();
	//��ǰ����ڱ�BLOCK�ڵ����λ��
	int nCurpos = tc.position() - tc.block().position();
	int nTextline = pLayout->lineForTextPosition(nCurpos).lineNumber() + tc.block().firstLineNumber();
	qDebug() << nTextline << endl;           //���Կ����к����Ź��ĸı���ı�
}*/
