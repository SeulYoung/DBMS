// mymemory.cpp: �������̨Ӧ�ó������ڵ㡣
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
//int main()
//{
//	string s = "test";
//	string str = "hello";
//	TableManage tb;
//	//tb.CheckExist(s);
//	tb.CreatDatebase(s, str);
//	//tb.DeleteDatebase(s);
//	//tb.DeleteDir(s);
//	//tb.SaveInfo(s);
//	//tb.SearchDatebase(s);
//	return 0;
//}

TableManage::TableManage(vector<vector<string>> s, string db)
{
	sql = s;
	dbName = db;
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
	ifstream fin("test.tb");   //��ȡ�ļ�
	if (!fin)              // �����ȡʧ�ܣ���ӡfail
	{
		cerr << "fail" << endl;
		return -1;
	}
	//fin >> a >> c >> b;   //��ȡ�Ķ���д�������
	fin.close();       //�ر��ļ�
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	ofstream ofile;               //��������ļ�
	ofile.open("test.tb");     //��Ϊ����ļ���
	ofile << a << b << c << endl;   //����д���ļ�

	ofile.close();
	getchar();
	return 0;
}

int TableManage::SearchDatebase(string path,string name)
{

	vector<string> files;
	getFiles(path, files);
	string temp = path +"\\" + name;
	//char str[30];
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		if (files[i].c_str() == temp) {


			return 1;
		}
		cout << files[i].c_str() << endl;

	}
	getchar();
	return 0;
}

int TableManage::AlterDatebase(string s)
{

	/*if (SearchDatebase(s) == 0) {
		cout << "not exist";
		return 0;
	}*/
	string path = s + ".tb";
	string path1 = s + ".tdf";
	string path2 = s + ".trd";
	ifstream fin1(path1), fin2(path2);
	string line;
	int i = 0, j = 0;
	if (fin1) //���±����ֶ���   
	{
		while (getline(fin1, line)) // line�в�����ÿ�еĻ��з�  
		{
			i++;
			//cout << line << endl;
		}
	}
	else // û�и��ļ�  
	{
		cout << "no such file" << endl;
	}
	fin1.close();
	if (fin2) //���±��м�¼��   
	{
		while (getline(fin2, line)) // line�в�����ÿ�еĻ��з�  
		{
			j++;
			//cout << line << endl;
		}
	}
	else // û�и��ļ�  
	{
		cout << "no such file" << endl;
	}
	fin2.close();
	ifstream in;
	in.open(path);
	string strFileData = "";
	string num_1, num_2, num_3 = "";
	int Cline = 1, lineNum_1 = 1, lineNum_2 = 2, lineNum_3 = 3;
	num_1 = std::to_string(i);
	num_2 = std::to_string(j);
	num_3 = getCurenttime();


	char tmpLineData[1024] = { 0 };

	while (in.getline(tmpLineData, sizeof(tmpLineData)))
	{
		if (Cline == lineNum_1)
		{
			strFileData += num_1;
			strFileData += "\n";
		}
		else if (Cline == lineNum_2)
		{
			strFileData += num_2;
			strFileData += "\n";
		}
		else if (Cline == lineNum_3)
		{
			strFileData += num_3;
			strFileData += "\n";
		}
		else
		{
			strFileData += CharToStr(tmpLineData);
			strFileData += "\n";
		}
		Cline++;
	}
	in.close();
	//д���ļ�
	ofstream out;
	out.open(path);
	out.flush();
	out << strFileData;
	out.close();


	getchar();
	return 1;
}

void TableManage::getFiles(string path, vector<string>& files)
{
	//�ļ����  
	long   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮  
			//�������,�����б�  
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
	//��ǰĿ¼
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

int TableManage::CreatDatebase(string & str)
{
	string s = sql[0][1];
	string frontpath = "./data/" + dbName + "/";

	string endpath1 = ".tb";
	string endpath2 = ".tdf";
	string endpath3 = ".tic";
	string endpath4 = ".trd";
	string endpath5 = ".tid";
	string path1 = frontpath+dbName + endpath1;
	string path2 = frontpath+s + endpath2;
	string path3 = frontpath+s + endpath3;
	string path4 = frontpath+s + endpath4;
	string path5 = frontpath+s + endpath5;
	string testpath = "./data/" + dbName;
	string testpath2 = s + endpath2;
	int temp = SearchDatebase(frontpath,path2);
	if (temp == 1) {
		str = "���ѱ�����";
		return false;
	}
	if (ValidDatebase(s) == -1) {
		str = "tablename over 128";
		return false;
	}
	fstream file1, file2, file3, file4, file5;
	file1.open(path1, ios::app);
	file2.open(path2, ios::out);
	file3.open(path3, ios::out);
	file4.open(path4, ios::out);
	file5.open(path5, ios::out);
	if (!file1 || !file2 || !file3 || !file4 || !file5)
	{
		str = "create table false";
		/*cout << "bad time" << endl;*/
		return false;
	}
	str = "create table successful";
	/*cout << "good time" << endl;*/
	file1.close();

	string datebasename;

	if (SearchDatebase(frontpath, testpath2) == 0) {
		ofstream ofile;               //��������ļ�
		ofile.open(path1, ios::app);     //��Ϊ����ļ���
		ofile << s << '\t';
		ofile << path2 << '\t';
		ofile << path3 << '\t';
		ofile << path4 << '\t';
		ofile << path5 << '\t';
		ofile << getCurenttime() << endl;
		ofile.close();
	}
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

int TableManage::DeleteDatebase(string & str)
{
	string s = sql[0][1];
	//string path = m_strPath + '\\' + filename;
	// int remove(char *filename);
	// ɾ���ļ����ɹ�����0�����򷵻�-1
	//if (SearchDatebase(s) == 0) {
	//	str = "table not exist";
	//	//cout << "not exist";
	//	return 0;
	//}
	
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
		str = "ɾ����ʧ��";
		return false;
	}
	str = "delete table successful";
	/*cout << "delete table successful" << endl;*/

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

string TableManage::CharToStr(char * contentChar) {
	string tempStr;

	for (int i = 0; contentChar[i] != '\0'; i++)

	{

		tempStr += contentChar[i];

	}

	return tempStr;



}

Record::Record(const char * tableName)
{
	this->tableName = tableName;
}

void Record::writeRecord(list<list<string>> recordList)
{
	string name(tableName);
	string path = name + ".trd";
	ifstream fin1(path);
	string line;
	int i = 0, j = 0;
	if (fin1) //���±����ֶ���   
	{
		while (getline(fin1, line)) // line�в�����ÿ�еĻ��з�  
		{
			i++;
			//cout << line << endl;
		}
	}
	else // û�и��ļ�  
	{
		cout << "no such file" << endl;
	}
	fin1.close();

	getchar();
}

list<ColumnList> Record::getRecord()
{
	ColumnList theColumnList;
	list<ColumnList> resultList;

	list<list<string>> everyLine = getLines();

	for (list<list<string>>::iterator ci = everyLine.begin(); ci != everyLine.end(); ++ci) {
		//StringList temp = getStringsSpiteByBlank(*ci);
		/*for (StringList::const_iterator zz = temp.begin(); zz != temp.end(); ++zz) {
		theColumnList.push_back(*zz);
		}
		resultList.push_back(theColumnList);*/
	}
	return resultList;
}


list<list<string>> Record::getLines()
{
	string name(tableName);
	string path = name + ".trd";
	list<list<string>> rst;
	StringList returnlist;
	ifstream fin1(path);
	string line;
	int fieldNum = 5;
	while (!fin1.eof()) {
		list<string> oneLine;
		for (int i = 0; i < 5; i++) {
			string temp;
			fin1 >> temp;
			if (!temp.compare(""))
				break;
			oneLine.push_back(temp);
		}
		rst.push_back(oneLine);
	}
	return rst;
}

