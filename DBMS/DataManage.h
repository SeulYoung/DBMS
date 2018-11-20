#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<sstream>
using namespace std;

class DataManage
{
public:
	DataManage(vector<vector<string>> s);
	~DataManage();
	string manage();
	const vector<string> explode(const string& s, const char& c);
	
private:
	vector<vector<string>> sql;
	vector<string> vec1;
	vector<vector<string>> vec2;
	vector<vector<string>> contents1;//select记录tdf表中内容
	vector<vector<string>> contents2;//select记录数据表中内容
	stringstream r_slct;//select return str

	string data_insert();
	string data_delete();
	string data_update();
	string data_select();
	bool isColumn();	
	string con_check();
	bool len_check();
	void getfieldV();
};

