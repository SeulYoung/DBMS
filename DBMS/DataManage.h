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
	DataManage(vector<vector<string>> s, string db);
	~DataManage() {};
	string manage();
	const vector<string> explode(const string& s, const char& c);

private:
	vector<vector<string>> sql;
	vector<string> vec1;
	vector<vector<string>> vec2;
	vector<vector<string>> contents1;//select��¼tdf��������
	vector<vector<string>> contents2;//select��¼���ݱ�������
	stringstream r_slct;//select return str

	string dbName; //��ǰʹ�õ����ݿ�����
	string data_insert();
	string data_delete();
	string data_update();
	string data_select();
	bool isColumn();
	string con_check();
	string con_parse(int, int, vector<vector<string>>);
	bool len_check();
	void getfieldV();
};
