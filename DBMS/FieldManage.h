#pragma once
#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

class FieldManage
{
public:
	FieldManage(vector<vector<string>> s);
	~FieldManage();
	void manage();
	
private:
	void field_Add();
	void field_Modify();
	void field_Delete();

	vector<vector<string>> sql1;
	string sql;
};

