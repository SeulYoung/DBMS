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
	string ForCreate();
	string ForDrop();
	string ForInsert();
	string ForDelete();
	string ForUpdate();
	string ForSelect();
	string preSql(string s);

	vector<vector<string>> result;
	string sql;
};
