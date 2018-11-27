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
	vector<vector<string>> fin_s;

	string dbName; //当前使用的数据库名称
	string data_insert();
	string data_delete();
	string data_update();
	string data_select();
	string isColumn();
	string con_check();
	string con_parse(int, int, vector<vector<string>>);
	bool type_check();
	bool len_check();
	bool getfieldV();

	string compare(int pos,string s3);
};
