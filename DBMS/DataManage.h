#pragma once

#include<vector>

using namespace std;

class DataManage
{
public:
	DataManage(vector<vector<string>> s);
	~DataManage();
	void select();

private:
	vector<vector<string>> sql;
};

