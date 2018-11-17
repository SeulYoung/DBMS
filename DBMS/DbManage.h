#pragma once
#include<stdio.h>
#include<string.h>
#include<string>
#include<iostream>
#include<io.h>
#include<fstream>
#include<time.h>
#include<vector>
//#include<afx.h>

using namespace std;
class DbManage
{
public:
	DbManage(vector<vector<string>> s);
	~DbManage();
	void CreateDatabase();
	void DeleteDatabase();
	const vector<string> explode(const string& s, const char& c);
	struct database
	{
	public:
		char name[128];
		char path[128];
		char type[12];
	};
private:
	vector<vector<string>> sql;
};

