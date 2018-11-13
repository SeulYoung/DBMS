#pragma once

#include <iostream>
#include <regex>

using namespace std;

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

	vector<vector<string>> result;
	string sql;
};
