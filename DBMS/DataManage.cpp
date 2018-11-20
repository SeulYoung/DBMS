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
	return msg;
}

string DataManage::data_insert()
{
	ofstream out_file;
	string s = "";
	out_file.open(sql.at(0).at(1) + ".trd", ios::out | ios::app );
	bool isNull = true;
	string msg;
	
	if (isColumn())
	{
		if (!len_check()) {
			return "���ݳ��Ȳ�����Լ��";
		}
		string con_c = con_check();
		if (con_c != "Լ�����ɹ�") {
			return con_c;
		}

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
	out_file.open(sql.at(0).at(1) + ".trd", ios::out | ios::app | ios::binary);
	bool isNull = true;
	//��ȡ�ַ��� �õ�ָ����

	//Ҫ�����ֶ��ַ���
	for (int i = 0; i < sql[1].size(); i++) {
		size_t pos = sql.at(1).at(i).find("=");
		string temp1 = sql.at(1).at(i).substr(0, pos);
		string temp2 = sql.at(1).at(i).substr(pos + 1, sql.at(1).at(i).size());
	}
	//�����ж��ַ���




	//�ж�ָ�����Ƿ����
	//�ж�Լ������
	//ȡֵ����



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
	vector<string> vec3;
	vector<vector<string>> vec4;

	ifstream in(sql.at(0).at(1) + ".tic");
	if (!in.is_open())
	{
		return"��������ڡ�";
	}
	//����vec3
	while (!in.eof())
	{
		char buffer[100];
		in.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec3.push_back(buffer);
	}
	in.close();

	//����vec4
	for (size_t j = 0; j < vec3.size(); j++) {
		vector<string> temp_vec;
		char *temp3;
		char temp4[100];
		for (int i = 0; i<vec3.at(j).length(); i++)
			temp4[i] = vec3.at(j)[i];
		temp4[vec3.at(j).length()] = '\0';

		temp3 = strtok(temp4, " ");
		while (temp3) {
			temp_vec.push_back(temp3);
			temp3 = strtok(NULL, " ");
		}
		vec4.push_back(temp_vec);
	}

	//���Լ��
	string isSuc;
	for (int i = 2; i < sql.at(0).size(); i++) {
		for (int j = 0; j < vec4.size(); j++) {
			if (sql.at(0).at(i) == vec4.at(j).at(1)) {
				isSuc = con_parse(i, j, vec4);
				if (isSuc != "Լ�����ɹ�")
					return isSuc;
			}
		}
	}

	//���not null;
	bool isNull = false;
	if (sql.at(0).size() > 2) {
		for (int j = 0; j < vec4.size(); j++) {
			if (vec4.at(j).at(2) == "not") {
				for (int i = 2; i < sql.at(0).size(); i++) {
					if (sql.at(0).at(i) == vec4.at(j).at(1)) {
						isNull = true;
						break;
					}
				}
			}
			if (!isNull)
				return "���ݲ����Ϸǿ�Լ��";
		}
	}
	
	return "Լ�����ɹ�";
}

string DataManage::con_parse(int pos1, int pos2,vector<vector<string>> vec4)
{
	string con_re = "Լ�����ɹ�";

	if (vec4.at(pos2).at(2) == "primary")
	{

	}
	else if (vec4.at(pos2).at(2) == "foreign")
	{

	}
	else if (vec4.at(pos2).at(2) == "default")
	{
		if (sql.at(1).at(pos1-2)!=vec4.at(pos2).at(3))
			con_re = "���ݲ�����defaultԼ��";
	}
	else
	{
		con_re = "Լ�����ɹ�";
	}
	return con_re;
}

bool DataManage::len_check()
{
	if (sql.at(0).size() > 2) {
		for (int i = 2; i < sql.at(0).size(); i++) {
			for (int j = 0; j < vec2.size(); j++) {
				if (sql.at(0).at(i) == vec2.at(j).at(1))
				{
					int size = atoi(vec2.at(j).at(3).c_str());
					if (sql.at(1).at(i-2).size() > size)
						return false;
					else
						break;
				}
			}
		}
	}
	else {
		for (int i = 0; i < sql.at(1).size(); i++) {
			int size = atoi(vec2.at(i).at(3).c_str());
			if (sql.at(1).at(i).size() > size)
				return false;
		}
	}
	return true;
}

void DataManage::getfieldV()
{
	ifstream in(sql.at(0).at(1) + ".tdf");
	if (!in.is_open())
	{
		cout << "Error opening file";
		//exit(1);
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
