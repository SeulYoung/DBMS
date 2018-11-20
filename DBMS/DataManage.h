#pragma once
#include <iostream>
#include <fstream>
#include <vector>


using namespace std;

class DataManage
{
public:
	DataManage(vector<vector<string>> s);
	~DataManage();
	string manage();
	
private:
	vector<vector<string>> sql;
	vector<string> vec1;
	vector<vector<string>> vec2;

	string data_insert();
	string data_delete();
	string data_update();
	string data_select();
	bool isColumn();	
	string con_check();
	bool len_check();
	void getfieldV();
};

