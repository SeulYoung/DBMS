#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<string>

using namespace std;

class FieldManage
{
public:
	FieldManage() {};
	FieldManage(vector<vector<string>> s);
	~FieldManage() {};
	vector<string> getField(string db, string table, string col);
	string manage();
	
private:
	string field_Add();
	string field_Add1();
	string field_Modify();
	string field_drop();
	string constraint_Add();
	string constraint_drop();

	vector<vector<string>> sql;
	string isSuc;
};

