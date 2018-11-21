#pragma once
#include<string>
#include<vector>
#include<list>
using namespace std;


class TableManage
{
public:
	char name[128];
	int record_num;
	int field_num;
	char tdf[256];
	char tic[256];
	char trd[256];
	char tid[256];
	string crtime;
	string mtime;

	TableManage(vector<vector<string>> s, string db);
	~TableManage() {};
	string CharToStr(char * contentChar);
	void ListDatebase(string s);	//列出表
	string getCurenttime();      //得到当前时间
	int SaveInfo(string s);			//保存信息
	int SearchDatebase(string s);	//验证表唯一性
	int AlterDatebase(string s);	//修改表
	void getFiles(string path, vector<string>& files);
	int CheckExist(string s);
	int CreatDatebase( string & str);	//创建表
	int ValidDatebase(string s);	//表明小于128
	int AlterDatebase();
	int DeleteDatebase(string & str);	//删除表
	int DeleteDir(string s);		//删除文件夹

private:
	vector<vector<string>> sql;
	string dbName; //当前使用的数据库名称
	char DataBaseName[128];
	char Table[128];
};

typedef	list<string> StringList;
typedef	list<string> ColumnList;

class Record {
private:
	const char *tableName;
public:
	Record(const char *tableName);
	void writeRecord(list<list<string>> recordList);
	list<ColumnList> getRecord();
	void updateRecord(string sourceContext, string rContext); //update
	list<list<string>> getLines();




};