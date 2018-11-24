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
	CmdParse();
	~CmdParse() {};
	string sqlCheck(string s);
	vector<vector<string>> getDbs();
	vector<vector<string>> getTableInfo(string db,string table);
	vector<vector<string>> getField(string db,string table,string col);

private:
	string dbShow();
	string dbUse();
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
	string dbName;
};
