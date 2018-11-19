#pragma once

#include <iostream>
#include <regex>
#include "DbManage.h"
#include "TableManage.h"
#include "FieldManage.h"
#include "DataManage.h"

class CmdParse
{
public:
	CmdParse(string s);
	~CmdParse() {};
	string sqlCheck();

private:
	string dbCreate();
	string dbDrop();
	string tableCreate();
	string tableAlter();
	string tableDrop();
	string tableInsert();
	string tableDelete();
	string tableUpdate();
	string tableSelect();
	string preSql(string s);
	string preWhere(string s);

	string sql;
};
