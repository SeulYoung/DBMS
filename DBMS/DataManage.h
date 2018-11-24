#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<sstream>
#include<algorithm>
using namespace std;

class DataManage
{
public:
	DataManage(vector<vector<string>> s, string db);
	~DataManage() {};
	string manage();
	const vector<string> explode(const string& s, const char& c);

private:
	vector<vector<string>> sql;
	vector<string> vec1;
	vector<vector<string>> vec2;
	vector<vector<string>> contents1;//select记录tdf表中内容
	stringstream r_slct;//select return str

	string dbName; //当前使用的数据库名称
	string data_insert();
	string data_delete();
	string data_update();
	string data_select();
	string isColumn();
	string con_check();
	string con_parse(int, int, vector<vector<string>>);
	bool len_check();
	bool getfieldV();
};
