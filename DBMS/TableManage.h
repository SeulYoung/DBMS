#pragma once
#include<string>
#include"io.h"
#include<vector>
using namespace std;


class TableManage
{
public:
	TableManage();
	~TableManage();
	void ListDatebase(string s);	//列出表
	string getCurenttime();      //得到当前时间
	int SaveInfo(string s);			//保存信息
	int SearchDatebase(string s);	//验证表唯一性
	int AlterDatebase(string s);	//修改表
	void getFiles(string path, vector<string>& files);
	int CheckExist(string s);
	int CreatDatebase(string s, string & str);	//创建表
	int ValidDatebase(string s);	//表明小于128
	int AlterDatebase();
	int DeleteDatebase(string s, string & str);	//删除表
	int DeleteDir(string s);		//删除文件夹
private:
	char DataBaseName[128];
	char Table[128];

};
