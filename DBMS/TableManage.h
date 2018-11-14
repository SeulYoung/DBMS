#pragma once
#include<string>
#include"io.h"
#include<vector>
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

	TableManage(vector<vector<string>> s);
	~TableManage();
	string CharToStr(char * contentChar);
	void ListDatebase(string s);	//�г���
	string getCurenttime();      //�õ���ǰʱ��
	int SaveInfo(string s);			//������Ϣ
	int SearchDatebase(string s);	//��֤��Ψһ��
	int AlterDatebase(string s);	//�޸ı�
	void getFiles(string path, vector<string>& files);
	int CheckExist(string s);
	int CreatDatebase(string s, string & str);	//������
	int ValidDatebase(string s);	//����С��128
	int AlterDatebase();
	int DeleteDatebase(string s, string & str);	//ɾ����
	int DeleteDir(string s);		//ɾ���ļ���

private:
	vector<vector<string>> sql;
	char DataBaseName[128];
	char Table[128];
};
