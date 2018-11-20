#include "DataManage.h"

DataManage::DataManage(vector<vector<string>> s)
{
	sql = s;
}

DataManage::~DataManage()
{
}

string DataManage::manage()
{
	string msg;
	if (sql.at(0).at(0) == "insert" || sql.at(0).at(0) == "INSERT")
		msg = data_insert();
	else if (sql.at(0).at(0) == "delete" || sql.at(0).at(0) == "DELETE")
		msg = data_delete();
	else if (sql.at(0).at(0) == "update" || sql.at(0).at(0) == "UPDATE")
		msg = data_update();
	else if (sql.at(0).at(0) == "select" || sql.at(0).at(0) == "SELECT")
		msg = data_select();
	return string();
}

string DataManage::data_insert()
{
	ofstream out_file;
	string s = "";
	out_file.open(sql.at(0).at(1) + ".trd", ios::out | ios::app | ios::binary);
	bool isNull = true;
	string msg;
	
	if (isColumn())
	{
		if (sql.at(0).size() > 2) {									//����ָ����
			for (int i = 0; i < vec2.size(); i++) {					//��tdf�ļ��е��м��
				isNull = true;
				for (int j = 2; j < sql.at(0).size(); j++) {		//���sql�ж�Ӧ����
					if (vec2.at(i).at(1) == sql.at(0).at(j)) {
						s = s + sql.at(1).at(j - 2);
						s = s + " ";
						isNull = false;
					}
				}
				if (isNull) {
					s = s + "NULL";
					s = s + " ";
					isNull = true;
				}
			}
			s = s + "\n";
			if (out_file.is_open())
			{
				out_file << (char*)s.data();
			}
			out_file.close();
		}
		else {														//����ָ����
			for (int i = 0; i < sql.at(1).size(); i++) {
				s = s + sql.at(1).at(i);
				s = s + " ";
			}
			s = s + "\n";
			if (out_file.is_open())
			{
				out_file << (char*)s.data();
			}
			out_file.close();
		}
	}
	else
		return "����������в����ڵ���";
	return "���ݲ���ɹ�";
}

string DataManage::data_delete()
{
	//��ȡtdf�ļ�
	getfieldV();
	return "����ɾ���ɹ�";
}

string DataManage::data_update()
{
	ofstream out_file;
	string s = "";
	vector<vector<string>> modify;
	
	vector<vector<string>> judge;
	out_file.open(sql.at(0).at(1) + ".trd", ios::out | ios::app | ios::binary);
	bool isNull = true;
	//��ȡ�ַ��� �õ�ָ����

	//Ҫ�����ֶ��ַ���
	for (int i = 0; i < sql[1].size(); i++) {
		vector<string> temp;
		size_t pos = sql.at(1).at(i).find("=");
		string temp1 = sql.at(1).at(i).substr(0, pos);
		string temp2 = sql.at(1).at(i).substr(pos + 1, sql.at(1).at(i).size());
		temp.push_back(temp1);
		temp.push_back(temp2);
		modify.push_back(temp);
	}
	//�����ж��ַ���
	for (int i = 0; i < sql[2].size(); i++) {
		vector<string> temp;
		if (sql.at(2).at(i).find("=") != string::npos) {
			if (sql.at(2).at(i).find("and") != string::npos) {
				size_t pos1 = sql.at(2).at(i).find("and");
				size_t pos = sql.at(2).at(i).find("=");
				string temp1 = sql.at(2).at(i).substr(0, pos);
				string temp2 = sql.at(2).at(i).substr(pos + 1, pos1-3);
				temp.push_back(temp1);
				temp.push_back(temp2);
				temp.push_back(std::to_string(1));
				judge.push_back(temp);
			}
			else {
				size_t pos = sql.at(2).at(i).find("=");
				string temp1 = sql.at(2).at(i).substr(0, pos);
				string temp2 = sql.at(2).at(i).substr(pos + 1, sql.at(1).at(i).size());
				temp.push_back(temp1);
				temp.push_back(temp2);
				temp.push_back(std::to_string(1));
				judge.push_back(temp);
			}
		}
		else if (sql.at(2).at(i).find("<") != string::npos) {
			size_t pos = sql.at(2).at(i).find("<");
			string temp1 = sql.at(2).at(i).substr(0, pos);
			string temp2 = sql.at(2).at(i).substr(pos + 1, sql.at(1).at(i).size());
			temp.push_back(temp1);
			temp.push_back(temp2);
			temp.push_back(std::to_string(2));
			judge.push_back(temp);
		}
		else if (sql.at(2).at(i).find(">") != string::npos) {
			size_t pos = sql.at(2).at(i).find(">");
			string temp1 = sql.at(2).at(i).substr(0, pos);
			string temp2 = sql.at(2).at(i).substr(pos + 1, sql.at(1).at(i).size());
			temp.push_back(temp1);
			temp.push_back(temp2);
			temp.push_back(std::to_string(3));
			judge.push_back(temp);
		}
	}
	getfieldV();
	int modify_flag = 0, judge_flag = 0;

	//���Ҫ��������Ƿ��Ѿ�����
	for (int i = 0; i < modify.size(); i++) {
		bool signal = false;
		for (int j = 0; j < vec1.size(); j++) {
			if (modify.at(i).at(0) == vec2.at(j).at(1)) {
				signal = true;
				break;
			}
		}
		if (!signal)
			modify_flag = 1;
	}
	for (int i = 0; i < judge.size(); i++) {
		bool signal = false;
		for (int j = 0; j < vec1.size(); j++) {
			if (judge.at(i).at(0) == vec2.at(j).at(1)) {
				signal = true;
				break;
			}
		}
		if (!signal)
			judge_flag = 1;
	}

	/*if (modify_flag || judge_flag) {
		
		return "�޸ĵ��в�����";
	}*/

	//�ж�Լ������



	//ȡ�����ݱ��е�ֵ
	string name = sql[0][1];
	string path = name + ".trd";
	vector<vector<string>> rst;
	ifstream fin1(path);
	string line;
	int fieldNum = vec1.size();
	while (!fin1.eof()) {
		vector<string> oneLine;
		for (int i = 0; i < fieldNum; i++) {
			string temp;
			fin1 >> temp;
			if (!temp.compare(""))
				break;
			oneLine.push_back(temp);
		}
		rst.push_back(oneLine);
	}
	//ȡֵ����
	/*int signal = 0;
	for (int i = 0; i < judge.size(); i++) {
		for (int j = 0; j < vec1.size(); j++) {
			if (judge.at(i).at(0) == vec2.at(j).at(1) && judge.at(i).at(1)= " "  ) {
				signal = true;
			}
		}
	}*/
	
	int pos1 ,pos2;
	
	for (int i = 0; i < judge.size(); i++) {
		for (int j = 0; j < vec2.size(); j++) {
			if (vec2.at(j).at(1) == judge.at(0).at(0)) {
				pos1 = i;
			}
			if (vec2.at(j).at(1) == judge.at(1).at(0)) {
				pos2 = i;
			}
		}
	}
	


	return "ok";

}

string DataManage::data_select()
{
	return string();
}

//�ж��Ƿ����Ѵ��ڵ���
bool DataManage::isColumn()
{
	//��ȡ.tdf�ļ�
	getfieldV();

	//���Ҫ��������Ƿ��Ѿ�����
	bool signal = false;
	if (sql.at(0).size() > 2) {
		for (int i = 2; i < sql.at(0).size(); i++) {
			for (int j = 0; j < vec2.size(); j++) {
				if (sql.at(0).at(i) == vec2.at(j).at(1))
					signal = true;
			}
			if (!signal)
				return signal;
		}
	}
	else
		signal = true;
	return signal;
	
}

string DataManage::con_check()
{
	return false;
}

string DataManage::len_check()
{
	//��ȡ�ļ�
	if (sql.at(0).size() > 2) {

	}
	else {
		for (int i = 0; i < sql.at(1).size(); i++) {

		}
	}
	return "true";
}

void DataManage::getfieldV()
{

	ifstream in(sql.at(0).at(1) + ".tdf");
	if (!in.is_open())
	{
		cout << "Error opening file";
		exit(1);
	}
	//����vec1
	while (!in.eof())
	{
		char buffer[100];
		in.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec1.push_back(buffer);

	}
	in.close();

	//����vec2
	for (size_t j = 0; j < vec1.size(); j++) {
		vector<string> temp_vec;
		char *temp3;
		char temp4[100];
		for (int i = 0; i<vec1.at(j).length(); i++)
			temp4[i] = vec1.at(j)[i];
		temp4[vec1.at(j).length()] = '\0';

		temp3 = strtok(temp4, " ");
		while (temp3) {
			temp_vec.push_back(temp3);
			temp3 = strtok(NULL, " ");
		}
		vec2.push_back(temp_vec);
	}
}
