#include "FieldManage.h"

FieldManage::FieldManage(vector<vector<string>> s, string db)
{
	sql = s;
	dbName = db;
}

string FieldManage::manage()
{
	if (sql.at(1).at(0).find("add") != string::npos && sql.at(1).at(0).find("constraint") != string::npos)
		isSuc = constraint_Add();
	else if (sql.at(1).at(0).find("ADD") != string::npos && sql.at(1).at(0).find("constraint") != string::npos)
		isSuc = constraint_Add();
	else if (sql.at(1).at(0).find("add") != string::npos || sql.at(1).at(0).find("ADD") != string::npos)
		isSuc = field_Add();
	else if (sql.at(1).at(0).find("modify") != string::npos || sql.at(1).at(0).find("MODIFY") != string::npos)
		isSuc = field_Modify();
	else if (sql.at(1).at(0).find("drop") != string::npos && sql.at(1).at(0).find("constraint") != string::npos)
		isSuc = constraint_drop();
	else if (sql.at(1).at(0).find("DROP") != string::npos && sql.at(1).at(0).find("constraint") != string::npos)
		isSuc = constraint_drop();
	else if (sql.at(1).at(0).find("drop") != string::npos || sql.at(1).at(0).find("DROP") != string::npos)
		isSuc = field_drop();
	else if (sql.at(0).at(0) == "create")
		isSuc = field_Add1();
	return isSuc;
}

string FieldManage::field_Add()
{
	ofstream out_file;
	string s;
	string file_Path = "./data/" + dbName +"/"+ sql.at(0).at(1)+ ".tdf";

	//���δ��������Ƿ��ظ�
	for (int i = 2; i < sql.size(); i++)
		for (int j = i + 1; j < sql.size(); j++)
			if (sql.at(i).at(0) == sql.at(j).at(0))
				return "����������ӵ��ֶ������ظ���";

	//����ļ����Ƿ�����Ϊδ�������
	vector<string> vec1;
	ifstream in(file_Path);
	if (!in.is_open())
	{
		return "���������";
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
				return "����������ӵ��ֶ��д�������ӵ��ֶΡ�";
		}
	}
	//����Ƿ�ȱ������
	for (int i = 2; i < sql.size(); i++) {
		if (sql.at(i).size() < 2)
			return "������ӵ���ȱ�����ԡ�";
	}

	//������Ϣ
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
	return "�ֶ���ӳɹ�";
}

string FieldManage::field_Add1()
{
	ofstream out_file;
	ofstream out_file1;
	string s;
	string file_Path = "./data/" + dbName + "/" + sql.at(0).at(1) + ".tdf";

	//���δ��������Ƿ��ظ�
	for (int i = 1; i < sql.size(); i++)
		for (int j = i + 1; j < sql.size(); j++)
			if (sql.at(i).at(0) == sql.at(j).at(0))
				return "����������ӵ��ֶ������ظ���";

	//����ļ����Ƿ�����Ϊδ�������
	vector<string> vec1;
	ifstream in(file_Path);
	if (!in.is_open())
	{
		return "���������";
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
		for (int i = 1; i < sql.size(); i++) {
			if (vec1.at(j).find(sql.at(i).at(0)) != string::npos)
				return "����������ӵ��ֶ��д�������ӵ��ֶΡ�";
		}
	}
	//����Ƿ�ȱ������
	for (int i = 2; i < sql.size(); i++) {
		if (sql.at(i).size() < 2)
			return "������ӵ���ȱ�����ԡ�";
	}

	//������Ϣ
	int order;
	for (int i = 1; i < sql.size(); i++) {
		order = i;
		order += vec1.size();
		s += std::to_string(order);
		s += " ";
		for (int j = 0; j < 2; j++) {
			s += sql.at(i).at(j);
			s += " ";
		}
		if (sql.at(i).size() > 2) {
			int isNum = atoi(sql.at(i).at(2).c_str());
			if (isNum == 0)
				s += "NULL";
			else
				s += sql.at(i).at(2);
		}
		else
			s += "NULL";
		
		s = s + "\n";
	}
	//����Լ��
	string con = "";
	for (int i = 1; i < sql.size(); i++) {
		if (sql.at(i).size() > 2) {
			int isNum = atoi(sql.at(i).at(2).c_str());
			if (sql.at(i).size() > 3 || isNum == 0) {
				con += sql.at(i).at(0);
				con += "_con";
				con += " ";
				con += sql.at(i).at(0);
				if (sql.at(i).size() > 3)
					con += sql.at(i).at(3);
				else {
					con += " ";
					con += sql.at(i).at(2);
				}

				con = con + "\n";
			}
		}
	}
	//д��tic
	out_file1.open("./data/" + dbName + "/" + sql.at(0).at(1) + ".tic", ios::out | ios::app);
	if (out_file1.is_open())
	{
		out_file1 << (char*)con.data();
	}
	out_file1.close();

	//д��tdf
	out_file.open(file_Path, ios::out | ios::app);
	if (out_file.is_open())
	{
		out_file << (char*)s.data();
	}
	out_file.close();
	return "������ɹ�";
}

string FieldManage::field_Modify()
{
	vector<char*> vec;

	string file_Path = "./data/" + dbName + "/" + sql.at(0).at(1) + ".tdf";

	//��ȡ�ļ�

	vector<string> vec1;
	vector<vector<string>> vec2;
	ifstream in(file_Path);
	if (!in.is_open())
	{
		return "���������";
	}

	while (!in.eof())
	{
		char buffer[100];
		in.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec1.push_back(buffer);

	}
	in.close();

	for (size_t j = 0; j < vec1.size(); j++) {
		vector<string> temp2;
		int m = 0;
		char *temp3;
		char temp4[100];
		for (int i = 0; i < vec1.at(j).length(); i++)
			temp4[i] = vec1.at(j)[i];
		temp4[vec1.at(j).length()] = '\0';

		temp3 = strtok(temp4, " ");
		while (temp3) {
			temp2.push_back(temp3);
			temp3 = strtok(NULL, " ");
		}
		vec2.push_back(temp2);
	}

	//���Ҫ���µ����Ƿ��Ѵ���
	bool isExist = false;

	for (int i = 2; i < sql.size(); i++) {
		for (int j = 0; j < vec1.size(); j++)
			if (vec1.at(j).find(sql.at(i).at(0)) != string::npos)
				isExist = true;
		if (!isExist)
			return "����������µ��ֶ����в�����";
		isExist = false;
	}

	/*����Ƿ�ȱ������
	for (int i = 2; i < sql.size(); i++) {
		if (sql.at(i).size() < 2)
			return "������ӵ���ȱ�����ԡ�";
	}*/


	//�޸���Ϣ
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

	//���������Ϣ
	string s;
	for (int i = 0; i < vec1.size(); i++) {
		s += vec1.at(i);
		s += "\n";
	}

	//�޸��ļ�
	ofstream out_file;
	out_file.open(file_Path);
	if (out_file.is_open())
	{
		out_file << (char*)s.data();
	}

	out_file.close();
	return "�ֶ��޸ĳɹ�";
}

string FieldManage::field_drop()
{
	string file_Path = "./data/" + dbName + "/" + sql.at(0).at(1) + ".tdf";

	//��ȡ�ļ�
	ifstream in(file_Path);
	vector<string> vec1;
	if (!in.is_open())
	{
		return "���������";
	}

	while (!in.eof())
	{
		char buffer[100];
		in.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec1.push_back(buffer);
	}
	in.close();

	//���Ҫɾ�������Ƿ��Ѵ���
	bool isExist = false;
	for (int i = 2; i < sql.size(); i++) {
		for (int j = 0; j < vec1.size(); j++)
			if (vec1.at(j).find(sql.at(i).at(0)) != string::npos)
				isExist = true;
		if (!isExist)
			return "��������ɾ�����ֶ����в����ڵġ�";
		isExist = false;
	}

	//ɾ����
	int order;
	for (int i = 0; i < vec1.size(); i++)
		for (int j = 2; j < sql.size(); j++)
			if (vec1.at(i).find(sql.at(j).at(0)) != string::npos) {
				vec1.erase(vec1.begin() + i);
				order = i;
			}
	for (int i = order; i < vec1.size(); i++) {
		string subS = std::to_string(i + 1);
		subS += vec1.at(i).substr(1, vec1.at(i).size() - 1);
		vec1.at(i) = subS;
	}

	//���������Ϣ
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
	return "�ֶ�ɾ���ɹ�";
}

string FieldManage::constraint_Add()
{


	//����Ƿ�����ظ���Լ����
	vector<string> vec3;
	ifstream in1("./data/" + dbName + "/" + sql.at(0).at(1) + ".tic");
	if (!in1.is_open())
	{
		return "���������";
	}
	while (!in1.eof())
	{
		char buffer[100];
		in1.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec3.push_back(buffer);
	}
	in1.close();

	for (int i = 0; i < vec3.size(); i++) {
		if (vec3.at(i).find(sql.at(1).at(1)) != string::npos)
			return "������ӵ�Լ�����Ѵ���";
	}

	//����vec1
	vector<string> vec1;
	ifstream in("./data/" + dbName + "/" + sql.at(0).at(1) + ".tdf");
	if (!in.is_open())
		return "���������";

	while (!in.eof())
	{
		char buffer[100];
		in.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec1.push_back(buffer);
	}
	in.close();

	string s;
	bool isExist = false;
	if (sql.at(2).at(1).find("default") != string::npos || sql.at(2).at(1) == "not null")
	{
		//����ļ����Ƿ�����Ҫ��ӵ��ֶ�
		for (int j = 0; j < vec1.size(); j++) {
			if (vec1.at(j).find(sql.at(2).at(0)) != string::npos) {
				isExist = true;
				break;
			}
		}
		if (!isExist)
			return "����������ӵ��ֶβ�����";

		s = sql.at(1).at(1);
		s += " ";
		for (int i = 0; i < sql.at(2).size(); i++) {
			s += sql.at(2).at(i);
			s += " ";
		}
		s += "\n";

	}
	else if (sql.at(2).at(0).find("primary") != string::npos || sql.at(2).at(0).find("unique") != string::npos)
	{
		if (sql.at(2).at(1).find(",") != string::npos)
		{
			size_t pos = sql.at(2).at(1).find(",");
			string temp1 = sql.at(2).at(1).substr(0, pos);
			string temp2 = sql.at(2).at(1).substr(pos + 1, sql.at(2).at(1).size());
			sql.at(2).erase(sql.at(2).begin() + 1);
			sql.at(2).push_back(temp1);
			sql.at(2).push_back(temp2);
		}
		//����ļ����Ƿ�����Ҫ��ӵ��ֶ�
		for (int i = 1; i < sql.at(2).size(); i++) {
			for (int j = 0; j < vec1.size(); j++) {
				if (vec1.at(j).find(sql.at(2).at(i)) != string::npos) {
					isExist = true;
					break;
				}
			}
			if (!isExist)
				return "����������ӵ��ֶβ�����";
			isExist = false;
		}
		s = "";
		for (int i = 1; i < sql.at(2).size(); i++) {
			s += sql.at(1).at(1);
			s += " ";
			s += sql.at(2).at(i);
			s += " ";
			s += sql.at(2).at(0);
			s += "\n";
		}
	}
	else if (sql.at(2).at(0) == "foreign key")
	{
		//����ļ����Ƿ�����Ҫ��ӵ��ֶ�
		for (int j = 0; j < vec1.size(); j++) {
			if (vec1.at(j).find(sql.at(2).at(1)) != string::npos) {
				isExist = true;
				break;
			}
		}
		if (!isExist)
			return "����������ӵ��ֶβ�����";

		s = sql.at(1).at(1);
		s += " ";
		s += sql.at(2).at(1);
		s += " ";
		s += sql.at(2).at(0);
		s += " ";
		size_t pos = sql.at(3).at(0).find(" ");
		string temp = sql.at(3).at(0).substr(pos + 1, sql.at(3).at(0).size());
		s += temp;
		s += "\n";
	}
	else if (sql.at(2).at(0) == "check")
	{
		string ty;

		//>,=,<�����
		vector<string> judge;
		judge.push_back(">");
		judge.push_back("<");
		judge.push_back("=");
		if (sql.at(2).at(1).find(">") != string::npos || sql.at(2).at(1).find("=") != string::npos || sql.at(2).at(1).find("<") != string::npos) {
			if (sql.size() <= 3) {
				if (sql.at(2).at(1).find("and") != string::npos) {
					size_t pos1 = sql.at(2).at(1).find("and");
					string temp1 = sql.at(2).at(1).substr(0, pos1 - 1);
					string temp2 = sql.at(2).at(1).substr(pos1+4, sql.at(2).at(1).size());
					sql.at(2).erase(sql.at(2).begin() + 1);
					sql.at(2).push_back(temp1);
					sql.at(2).push_back(temp2);

					for (int i = 1; i < sql.at(2).size(); i++) {
						if (i == 2)
							i = 4;
						else if (i == 5)
							i = sql.at(2).size() - 1;
						for (int j = 0; j < judge.size(); j++) {
							if (sql.at(2).at(i).find(judge.at(j)) != string::npos)
							{
								size_t pos2 = sql.at(2).at(i).find(judge.at(j));
								string temp3 = sql.at(2).at(i).substr(0, pos2);
								string temp4 = sql.at(2).at(i).substr(pos2+1, sql.at(2).at(i).size());
								sql.at(2).erase(sql.at(2).begin() + i);
								sql.at(2).insert(sql.at(2).begin() + i,temp3);
								sql.at(2).insert(sql.at(2).begin() + i + 1, judge.at(j));
								sql.at(2).insert(sql.at(2).begin() + i + 2, temp4);
							}
						}
					}
				}
				else {
					for (int j = 0; j < judge.size(); j++) {
						if (sql.at(2).at(1).find(judge.at(j)) != string::npos)
						{
							size_t pos1 = sql.at(2).at(1).find(judge.at(j));
							string temp3 = sql.at(2).at(1).substr(0, pos1);
							string temp4 = sql.at(2).at(1).substr(pos1 + 1, sql.at(2).at(1).size());
							sql.at(2).erase(sql.at(2).begin() + 1);
							sql.at(2).insert(sql.at(2).begin() + 1, temp3);
							sql.at(2).insert(sql.at(2).begin() + 2, judge.at(j));
							sql.at(2).insert(sql.at(2).begin() + 3, temp4);
						}
					}	
				}
			}
			//��or
			else {
				for (int m = 2; m < sql.size(); m++) {

					size_t pos1 = sql.at(m).at(1).find("and");
					string temp1;
					if (m == 3)
						temp1 = sql.at(m).at(1).substr(0, pos1 - 1);
					else
						temp1 = sql.at(m).at(1).substr(1, pos1 - 2);
					string temp2 = sql.at(m).at(1).substr(pos1 + 4, sql.at(m).at(1).size());
					sql.at(m).erase(sql.at(m).begin() + 1);
					sql.at(m).push_back(temp1);
					sql.at(m).push_back(temp2);

					for (int i = 1; i < sql.at(m).size(); i++) {
						if (i == 2)
							i = 4;
						else if (i == 5)
							break;
						for (int j = 0; j < judge.size(); j++) {
							if (sql.at(m).at(i).find(judge.at(j)) != string::npos)
							{
								size_t pos2 = sql.at(m).at(i).find(judge.at(j));
								string temp3 = sql.at(m).at(i).substr(0, pos2);
								string temp4 = sql.at(m).at(i).substr(pos2 + 1, sql.at(m).at(i).size());
								sql.at(m).erase(sql.at(m).begin() + i);
								sql.at(m).insert(sql.at(m).begin() + i, temp3);
								sql.at(m).insert(sql.at(m).begin() + i + 1, judge.at(j));
								sql.at(m).insert(sql.at(m).begin() + i + 2, temp4);
							}
						}
					}

				}
			}
			ty = ">=<";
		}
		if (ty == ">=<") {
			s = sql.at(1).at(1);
			s += " ";
			for (int n = 2; n < sql.size(); n++) {
				if (n == 2) {
					s += sql.at(n).at(1);
					if (sql.at(n).size() > 5) {
						s += ",";
						s += sql.at(n).at(4);
					}
					s += " ";
					s += "ckeck";
					s += ty;
					s += " ";
				}
				if (n == 3) {
					s += "or";
					s += " ";
				}
				for (int j = 1; j < sql.at(n).size(); j++) {
					s += sql.at(n).at(j);
					s += " ";
				}
			}
		}

		
		//�ָ�check�ֶ�
		if (sql.at(2).at(1).find("between") != string::npos) {
			size_t pos1 = sql.at(2).at(1).find("between");
			size_t pos2 = sql.at(2).at(1).find("and");
			string field_n = sql.at(2).at(1).substr(0, pos1 - 1);
			string temp1 = sql.at(2).at(1).substr(pos1 + 8, pos2 - pos1 - 9);
			string temp2 = sql.at(2).at(1).substr(pos2 + 4, sql.at(2).at(1).size() - pos2 - 3);
			sql.at(2).erase(sql.at(2).begin() + 1);
			sql.at(2).push_back(field_n);
			sql.at(2).push_back(temp1);
			sql.at(2).push_back(temp2);
			ty = "between";
		}
		else if (sql.at(2).at(1).find("in") != string::npos) {
			size_t pos1 = sql.at(2).at(1).find("in");
			size_t pos2 = sql.at(2).at(1).find(",");
			string field_n = sql.at(2).at(1).substr(0, pos1 - 1);
			string temp1 = sql.at(2).at(1).substr(pos1 + 3, pos2 - pos1 - 3);
			string temp2 = sql.at(2).at(1).substr(pos2 + 1, sql.at(2).at(1).size());
			sql.at(2).erase(sql.at(2).begin() + 1);
			sql.at(2).push_back(field_n);
			sql.at(2).push_back(temp1);
			sql.at(2).push_back(temp2);
			ty = "in";
		}
		if (ty == "in" || ty == "between") {
			s = sql.at(1).at(1);
			s += " ";
			s += sql.at(2).at(1);
			s += " ";
			s += sql.at(2).at(0);
			s += ty;
			s += " ";
			for (int i = 2; i < sql.at(2).size(); i++) {
				s += sql.at(2).at(i);
				s += " ";
			}
			s += "\n";
		}			
	}

	string file_Path = "./data/" + dbName + "/" + sql.at(0).at(1) + ".tic";
	ofstream out_file;
	out_file.open(file_Path, ios::out | ios::app);

	if (out_file.is_open())
		out_file << (char*)s.data();

	out_file.close();
	return "Լ����ӳɹ�";
}

string FieldManage::constraint_drop()
{
	vector<string> vec1;
	ifstream in("./data/" + dbName + "/" + sql.at(0).at(1) + ".tdf");
	if (!in.is_open())
	{
		return "���������";
	}
	while (!in.eof())
	{
		char buffer[100];
		in.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec1.push_back(buffer);
	}
	in.close();

	//��ȡԼ���ļ�
	vector<string> vec3;
	ifstream in1("./data/" + dbName + "/" + sql.at(0).at(1) + ".tic");
	if (!in1.is_open())
	{
		return "���������";
	}
	while (!in1.eof())
	{
		char buffer[100];
		in1.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec3.push_back(buffer);
	}
	in1.close();

	//ɾ��Լ��
	bool isExist = false;
	for (int i = 0; i < vec3.size(); i++)
		if (vec3.at(i).find(sql.at(1).at(1)) != string::npos) {
			vec3.erase(vec3.begin() + i);
			isExist = true;
			break;
		}
	if (!isExist)
		return "��������ɾ����Լ��������";

	//���������Ϣ
	string s;
	for (int i = 0; i < vec3.size(); i++) {
		s += vec3.at(i);
		s += "\n";
	}

	ofstream out_file;
	out_file.open("./data/" + dbName + "/" + sql.at(0).at(1) + ".tic");
	if (out_file.is_open())
	{
		if (s != "")
			out_file << (char*)s.data();;
	}
	out_file.close();
	return "Լ��ɾ���ɹ�";

}