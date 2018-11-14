#pragma once

#include<vector>

using namespace std;

class DbManage
{
public:
	DbManage(vector<vector<string>> s);
	~DbManage();

private:
	vector<vector<string>> sql;
};

