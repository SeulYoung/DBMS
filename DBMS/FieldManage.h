#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<string>

using namespace std;

class FieldManage
{
public:
	FieldManage(vector<vector<string>> s, string db);
	~FieldManage() {};
	string manage();

private:
	string field_Add();
	string field_Add1();
	string field_Modify();
	string field_drop();
	string constraint_Add();
	string constraint_drop();

	vector<vector<string>> sql;
	string dbName; //当前使用的数据库名称
	string isSuc;
};

