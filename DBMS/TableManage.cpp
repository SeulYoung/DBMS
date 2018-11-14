// mymemory.cpp: 定义控制台应用程序的入口点。
//


#include "TableManage.h"
#include<io.h>
#include <stdio.h>
#include"iostream"
#include"fstream"
#include<windows.h>
#include <direct.h>
#include<ctime>

using namespace std;
int main()
{
	string s = "test";
	string str = "hello";
	TableManage tb;
	//tb.CheckExist(s);
	tb.CreatDatebase(s, str);
	//tb.DeleteDatebase(s);
	//tb.DeleteDir(s);
	//tb.SaveInfo(s);
	//tb.SearchDatebase(s);
	return 0;
}

TableManage::TableManage()
{
}

TableManage::~TableManage()
{
}

void TableManage::ListDatebase(string s)
{
	string path = "C:\\Users\\lxy94\\Desktop";
	vector<string> files;
	getFiles(path, files);
	//char str[30];
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		cout << files[i].c_str() << endl;

	}
	getchar();
}

string TableManage::getCurenttime()
{
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	printf("%d-%02d-%02d %02d:%02d:%02d\n",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond);
	string time = std::to_string(st.wYear) + "-" + to_string(st.wMonth) + "-" + to_string(st.wDay) + " " + to_string(st.wHour) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond);
	return time;
}

int TableManage::SaveInfo(string s)
{
	string datebasename;
	string tablename;
	string path1, path2, path3, path4;
	string createtime, modifytime;

	string endpath1 = ".tdf";
	string endpath2 = ".tic";
	string endpath3 = ".trd";
	string endpath4 = ".tid";
	char   buffer[80];
	_getcwd(buffer, 80);
	cout << buffer << endl;
	path1 = buffer + s + endpath1;
	path2 = buffer + s + endpath2;
	path3 = buffer + s + endpath3;
	path4 = buffer + s + endpath4;

	/*fstream _file, file1, file2, file3, file4, file5;
	_file.open(path1, ios::out);
	if (!_file)
	{
	cout  << "have not created" << endl;
	}
	else
	{
	cout << "already exits" << endl;
	getchar();
	return 1;
	}*/

	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	printf("%d-%02d-%02d %02d:%02d:%02d\n",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond);
	string time = std::to_string(st.wYear) + "-" + to_string(st.wMonth) + "-" + to_string(st.wDay) + " " + to_string(st.wHour) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond);
	cout << time;

	int a = 2, b = 3, c = 1;
	ifstream fin("test.tb");   //读取文件
	if (!fin)              // 如果读取失败，打印fail
	{
		cerr << "fail" << endl;
		return -1;
	}
	//fin >> a >> c >> b;   //读取的东西写入给变量
	fin.close();       //关闭文件
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	ofstream ofile;               //定义输出文件
	ofile.open("test.tb");     //作为输出文件打开
	ofile << a << b << c << endl;   //标题写入文件

	ofile.close();
	getchar();
	return 0;
}

int TableManage::SearchDatebase(string s)
{
	string path = "D:\\Memory\\mymemory";
	vector<string> files;
	getFiles(path, files);
	string temp = "D:\\Memory\\mymemory\\" + s + ".tb";
	//char str[30];
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		if (files[i].c_str() == temp) {

			cout << "haha" << endl;
			return 1;
		}
		cout << files[i].c_str() << endl;

	}
	getchar();
	return 0;
}

int TableManage::AlterDatebase(string s)
{

	if (SearchDatebase(s) == 0) {
		cout << "not exist";
		return 0;
	}


	return 1;
}

void TableManage::getFiles(string path, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);

	}
}

int TableManage::CheckExist(string s)
{

	//char *filePath = new char[20];
	//strcpy(filePath, "D:\\sample");
	//filePath = "D:\\sample";
	//	char * filePath = "D:\\sample";
	string Filename = s + ".txt";
	fstream _file;
	_file.open(Filename, ios::in);
	//当前目录
	/*char   buffer[80];
	_getcwd(buffer, 80);
	cout << buffer << endl;*/
	if (!_file)
	{
		cout << Filename << "have not created" << endl;
	}
	else
	{
		cout << Filename << "already exits" << endl;
		return 1;
	}

	getchar();
	return 0;
}

int TableManage::CreatDatebase(string s, string & str)
{
	string frontpath;
	string endpath = ".txt";
	string endpath1 = ".tb";
	string endpath2 = ".tdf";
	string endpath3 = ".tic";
	string endpath4 = ".trd";
	string endpath5 = ".tid";
	string path = s + endpath;
	string path1 = s + endpath1;
	string path2 = s + endpath2;
	string path3 = s + endpath3;
	string path4 = s + endpath4;
	string path5 = s + endpath5;
	int temp = SearchDatebase(s);
	/*if (temp == 1) {
	str = "table already exist";
	return false;
	}*/
	if (ValidDatebase(s) == -1) {
		str = "tablename over 128";
		return false;
	}
	fstream file, file1, file2, file3, file4, file5;
	file.open(path, ios::out);
	file1.open(path1, ios::app);
	file2.open(path2, ios::out);
	file3.open(path3, ios::out);
	file4.open(path4, ios::out);
	file5.open(path5, ios::out);
	if (!file || !file1 || !file2 || !file3 || !file4 || !file5)
	{
		str = "create table false";
		/*cout << "bad time" << endl;*/
		return false;
	}
	str = "create table successful";
	/*cout << "good time" << endl;*/
	file.close();
	file1.close();

	string datebasename;
	ofstream ofile;               //定义输出文件
	ofile.open(path1);     //作为输出文件打开
	ofile << s << endl;
	ofile << path2 << endl;
	ofile << path3 << endl;
	ofile << path4 << endl;
	ofile << path5 << endl;
	ofile << getCurenttime() << endl;
	ofile.close();
	getchar();
	return 1;
}

int TableManage::ValidDatebase(string s)
{
	if (s.length() > 128)
		return -1;

	return 0;
}

int TableManage::AlterDatebase()
{
	return 0;
}

int TableManage::DeleteDatebase(string s, string & str)
{

	//string path = m_strPath + '\\' + filename;
	// int remove(char *filename);
	// 删除文件，成功返回0，否则返回-1
	if (SearchDatebase(s) == 0) {
		str = "not exist";
		//cout << "not exist";
		return 0;
	}
	string frontpath;
	string endpath = ".txt";
	string path = "dic" + endpath;
	string endpath1 = ".tdf";
	string endpath2 = ".tic";
	string endpath3 = ".trd";
	string endpath4 = ".tid";
	string path1 = s + endpath1;
	string path2 = s + endpath2;
	string path3 = s + endpath3;
	string path4 = s + endpath4;
	if (-1 == remove(path1.c_str()) || -1 == remove(path2.c_str()) || -1 == remove(path3.c_str()) || -1 == remove(path4.c_str()))
	{
		str = "delete false";
		cout << "delete false" << endl;
		getchar();
		return false;
	}
	str = "delete table successful";
	/*cout << "delete table successful" << endl;*/
	getchar();
	return 1;
}

int TableManage::DeleteDir(string s)
{
	string dirName = "D:\\atest";

	system("rd /s/q D:\\atest");


	cout << " next good time" << endl;
	getchar();
	return 0;
}
