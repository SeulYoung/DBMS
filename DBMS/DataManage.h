#pragma once

#include<vector>

using namespace std;

class DataManage
{
public:
	DataManage(vector<vector<string>> s);
	~DataManage();

private:
	vector<vector<string>> sql;
};

