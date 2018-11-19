#include "FieldManage.h"



FieldManage::FieldManage(vector<vector<string>> s)
{
	sql = s;
}

FieldManage::~FieldManage()
{
}

string FieldManage::manage()
{
	if (sql.at(1).at(0).find("add") != string::npos || sql.at(1).at(0).find("ADD") != string::npos)
		isSuc = field_Add();
	else if (sql.at(1).at(0).find("modify") != string::npos || sql.at(1).at(0).find("MODIFY") != string::npos)
		isSuc = field_Modify();
	else if (sql.at(1).at(0).find("drop") != string::npos || sql.at(1).at(0).find("DROP") != string::npos)
		isSuc = field_Delete();
	return isSuc;
}

string FieldManage::field_Add()
{
	ofstream out_file;
	string s;
	string file_Path = sql.at(0).at(1)+".tdf";

	//检查未加入的列是否重复
	for (int i = 2; i < sql.size(); i++) 
		for (int j = i + 1; j < sql.size(); j++) 
			if (sql.at(i).at(0) == sql.at(j).at(0))
				return "错误！请求添加的字段名称重复。";

	//检查文件中是否已有为未加入的列
	vector<string> vec1;
	ifstream in(file_Path);
	if (!in.is_open())
	{
		cout << "Error opening file"; 
		exit(1);
	}
	while (!in.eof())
	{
		char buffer[100];
		in.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec1.push_back(buffer);
	}
	in.close();
	for (int j = 0; j < vec1.size(); j++) {
		for (int i = 2; i < sql.size(); i++) {
			if (vec1.at(j).find(sql.at(i).at(0)) != string::npos)
				return "错误！请求添加的字段中存在已添加的字段。";
		}
	}

	//生成信息
	int order;
	for (int i = 2; i < sql.size(); i++) {
		order = i - 1;
		order += vec1.size();
		s += std::to_string(order);
		s += " ";
		for (int j = 0; j < sql.at(i).size(); j++) {		
				s += sql.at(i).at(j);
				s += " ";
		}
		if (sql.at(i).size() == 2)
			s += "NULL";
		s = s + "\n";
	}
	
	out_file.open(file_Path, ios::out | ios::app);
	if (out_file.is_open())
	{
		out_file << (char*)s.data();
	}
	out_file.close();
	return "字段添加成功";
}

string FieldManage::field_Modify()
{
	vector<char*> vec;

	string file_Path = sql.at(0).at(1) + ".tdf";

	//读取文件
	
	vector<string> vec1;
	vector<vector<string>> vec2;
	ifstream in(file_Path);
	if (!in.is_open())
	{
		cout << "Error opening file"; exit(1);
	}
	
	while (!in.eof())
	{
		char buffer[100];
		in.getline(buffer, sizeof(buffer));
		if(strlen(buffer)!=0)
			vec1.push_back(buffer);
		
	}
	in.close();

	for (size_t j = 0; j < vec1.size(); j++) {
		vector<string> temp2;
		int m = 0;
		char *temp3;
		char temp4[100];
		for (int i = 0; i<vec1.at(j).length(); i++)
			        temp4[i] = vec1.at(j)[i];
		 temp4[vec1.at(j).length()] = '\0';

		temp3 = strtok(temp4, " ");
		while (temp3) {			
			temp2.push_back(temp3);
			temp3 = strtok(NULL, " ");
		}
		vec2.push_back(temp2);
	}

	//检查要更新的列是否已存在
	bool isExist = false;
	
	for (int i = 2; i < sql.size(); i++) {
		for (int j = 0; j < vec1.size(); j++)
			if (vec1.at(j).find(sql.at(i).at(0)) != string::npos)
				isExist = true;
		if(!isExist)
			return "错误！请求更新的字段中有不存在的。";
		isExist = false;
	}
	
	
	//修改信息
	for (int i = 2; i < sql.size(); i++) {
		for (int j = 0; j < vec2.size(); j++) {
			if (vec2.at(j).at(1) == sql.at(i).at(0))
			{
				vec1.erase(vec1.begin() + j);

				string s;
				s = std::to_string(j + 1);
				s += " ";
				s += sql.at(i).at(0);
				for (int m = 2; m < vec2.at(j).size(); m++) {
					if (m == 2) {
						s += " ";
						s += sql.at(i).at(1);
					}
					else if (m == 3) {
						s += " ";
						if (sql.at(i).size() > 2)
							s += sql.at(i).at(2);
						else
							s += "NULL";
					}
					else {
						s = s + " ";
						s = s + vec2.at(j).at(m);
					}
				}
				vec1.insert(vec1.begin() + j, s);
			}
		}
	}
	
	//生成输出信息
	string s;
	for (int i = 0; i < vec1.size(); i++) {
		s += vec1.at(i);
		s += "\n";
	}

	//修改文件
	ofstream out_file;
	out_file.open(file_Path);
	if (out_file.is_open())
	{
		out_file << (char*)s.data();
	}

	out_file.close();
	return "字段修改成功";
}

string FieldManage::field_Delete()
{
	string file_Path = sql.at(0).at(1) + ".tdf";

	//读取文件
	ifstream in(file_Path);
	vector<string> vec1;
	if (!in.is_open())
	{
		cout << "Error opening file"; exit(1);
	}

	while (!in.eof())
	{
		char buffer[100];
		in.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec1.push_back(buffer);
	}
	in.close();

	//检查要删除的列是否已存在
	bool isExist = false;
	for (int i = 2; i < sql.size(); i++) {
		for (int j = 0; j < vec1.size(); j++)
			if (vec1.at(j).find(sql.at(i).at(0)) != string::npos)
				isExist = true;
		if (!isExist)
			return "错误！请求删除的字段中有不存在的。";
		isExist = false;
	}

	//删除列
	for (int i = 0; i < vec1.size(); i++)
		for (int j = 2; j < sql.size(); j++) 
			if (vec1.at(i).find(sql.at(j).at(0)) != string::npos)
				vec1.erase(vec1.begin() + i);

	//生成输出信息
	string s;	
	for (size_t i = 0; i < vec1.size(); i++) {
		s += vec1.at(i);
		s += "\n";
	}

	ofstream out_file;
	out_file.open(file_Path);
	if (out_file.is_open())
	{
		if (s != "")
			out_file << (char*)s.data();;
	}
	out_file.close();
	return "字段删除成功";
}
