#pragma once
#include<stdio.h>
#include<string.h>
#include<string>
#include<iostream>
#include<io.h>
#include<fstream>
#include<time.h>
#include<vector>
#include<direct.h>
#include<sstream>
#include<tchar.h>
#include<Windows.h>
//#include<afx.h>

using namespace std;
class DbManage
{
public:
	DbManage(vector<vector<string>> s);
	~DbManage() {};
	string CreateDatabase();
	string DeleteDatabase();
	string ShowDatabase();
	string UseDatabase();
	const vector<string> explode(const string& s, const char& c);
	BOOL IsDirectory(const char * pDir);
	BOOL DeleteDirectory(const char * DirName);
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