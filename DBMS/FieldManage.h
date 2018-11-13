#pragma once
#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

class FieldManage
{
public:
	FieldManage(string s,string db_name);
	~FieldManage();
	void manage();
	
private:
	void field_Add();
	void field_Modify();
	void field_Delete();

	string sql;
	string db_name;
	
};

