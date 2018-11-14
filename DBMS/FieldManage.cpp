#include "FieldManage.h"



FieldManage::FieldManage(vector<vector<string>> s)
{
	sql1 = s;
	sql = "dddd";
}

FieldManage::~FieldManage()
{
}

void FieldManage::manage()
{
	if (sql.find("add") != string::npos || sql.find("ADD") != string::npos)
		field_Add();
	else if (sql.find("modify") != string::npos || sql.find("MODIFY") != string::npos)
		field_Modify();
	else if (sql.find("drop") != string::npos || sql.find("DROP") != string::npos)
		field_Delete();
}

void FieldManage::field_Add()
{
	vector<char*> vec;
	int i = 1;
	ofstream out_file;
	char tb[100] = "";
	char *sqlc = (char*)sql.data();
	char *temp;
	
	temp = strtok(sqlc, " ");
	while (temp) {
		if (i == 3)
			vec.push_back(temp);
		else if (i == 5)
			vec.push_back(temp);	
		else if (i >= 6)
			vec.push_back(temp);
		
		temp = strtok(NULL, " ");
		i++;
	}
	//生成输出信息
	strcpy(tb, vec.at(1));
	for (size_t i = 2; i < vec.size(); i++) {
		strcat(tb, " ");
		strcat(tb, vec.at(i));	
	}
	strcat(tb, "\n");

	char *file_Path = strcat(vec.at(0), ".tdf");
	
	out_file.open(file_Path, ios::out | ios::app);
	if (out_file.is_open())
	{
		out_file << tb;
	}

	out_file.close();
}

void FieldManage::field_Modify()
{

	int i = 1;
	vector<char*> vec;
	char tb[100] = "";
	char *sqlc = (char*)sql.data();
	char *temp;

	temp = strtok(sqlc, " ");
	while (temp) {
		if (i == 3)
			vec.push_back(temp);
		else if (i == 5)
			vec.push_back(temp);
		else if (i >= 6)
			vec.push_back(temp);

		temp = strtok(NULL, " ");
		i++;
	}

	char *file_Path = strcat(vec.at(0), ".tdf");

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
		for (i = 0; i<vec1.at(j).length(); i++)
			        temp4[i] = vec1.at(j)[i];
		 temp4[vec1.at(j).length()] = '\0';

		temp3 = strtok(temp4, " ");
		while (temp3) {			
			temp2.push_back(temp3);
			temp3 = strtok(NULL, " ");
		}
		vec2.push_back(temp2);
	}
	
	for (size_t j = 0; j < vec2.size(); j++) {
		if (vec2.at(j).at(0) == vec.at(1))
		{
			vec1.erase(vec1.begin() + j);
			//生成新条件
			string s;
			s = vec.at(1);
			for (size_t i = 1; i < vec2.at(j).size(); i++) {
				if (i == 1){
					//strcat(s, " ");
					//strcat(s, (char*)vec1.at(2).data());
					s = s + " ";
					s = s + vec.at(2);
				}
				else {
					//strcat(s, " ");
					//strcat(s, (char*)vec2.at(j).at(i).data());
					s = s + " ";
					s = s + vec2.at(j).at(i);
				}
			}

			vec1.insert(vec1.begin() + j, s);
		}
			
	}
	
	//生成输出信息
	strcpy(tb, (char*)vec1.at(0).data());
	for (size_t i = 1; i < vec1.size(); i++) {
		strcat(tb, "\n");
		strcat(tb, (char*)vec1.at(i).data());
	}

	//修改文件
	ofstream out_file;
	out_file.open(file_Path);
	if (out_file.is_open())
	{
		out_file << tb;
	}

	out_file.close();
}

void FieldManage::field_Delete()
{
	vector<char*> vec;
	vector<string> vec1;
	int i = 1;
	ofstream out_file;
	char tb[100] = "";
	string sqll;
	for (int i = 0; i<sql.size() - 1; i++)
		sqll += sql[i];

	char *sqlc = (char*)sqll.data();
	char *temp;

	temp = strtok(sqlc, " ");
	while (temp) {
		if (i == 3)
			vec.push_back(temp);
		else if (i == 5)
			vec.push_back(temp);
		else if (i >= 6)
			vec.push_back(temp);

		temp = strtok(NULL, " ");
		i++;
	}

	char *file_Path = strcat(vec.at(0), ".tdf");

	ifstream in(file_Path);
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

	for (int i = 0; i < vec1.size(); i++)
		if (vec1.at(i).find(vec.at(2)) != string::npos)
			vec1.erase(vec1.begin() + i);
		
	//生成输出信息
	if (vec.size() < 1) {
		strcpy(tb, (char*)vec1.at(0).data());
		for (size_t i = 1; i < vec1.size(); i++) {
			strcat(tb, "\n");
			strcat(tb, (char*)vec1.at(i).data());
		}
	}
	

	out_file.open(file_Path);
	if (out_file.is_open())
	{
		if(tb!="")
			out_file << tb;
	}

	out_file.close();
}
