#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<string>

using namespace std;

class FieldManage
{
public:
	FieldManage(vector<vector<string>> s);
	~FieldManage();
	string manage();
	
private:
	string field_Add();
	string field_Modify();
	string field_Delete();

	vector<vector<string>> sql;
	string isSuc;
};

