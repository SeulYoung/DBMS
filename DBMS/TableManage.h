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
	char DataBaseName[128];
	char Table[128];

};
