#include "DataManage.h"

DataManage::DataManage(vector<vector<string>> s,string db)
{
	sql = s;
	dbName = db;
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

const vector<string> DataManage::explode(const string & s, const char & c)
{
	string buff{ "" };
	vector<string> v;

	for (auto n : s)
	{
		if (n != c) buff += n;
		else if (n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if (buff != "") v.push_back(buff);

	return v;
}

string DataManage::data_insert()
{
	ofstream out_file;
	string s = "";
	out_file.open(sql.at(0).at(1) + ".trd", ios::out | ios::app);
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
	vector<vector<string>> judge;

	//�����ж��ַ���
	for (int i = 0; i < sql[1].size(); i++) {
		vector<string> temp;
		if (sql.at(1).at(i).find("=") != string::npos) {
			if (sql.at(1).at(i).find("and") != string::npos) {
				size_t pos1 = sql.at(1).at(i).find("and");
				size_t pos = sql.at(1).at(i).find("=");
				string temp1 = sql.at(1).at(i).substr(0, pos);
				string temp2 = sql.at(1).at(i).substr(pos + 1, pos1 - pos - 2);
				temp.push_back(temp1);
				temp.push_back(temp2);
				temp.push_back(std::to_string(1));
				judge.push_back(temp);
			}
			else {
				size_t pos = sql.at(1).at(i).find("=");
				string temp1 = sql.at(1).at(i).substr(0, pos);
				string temp2 = sql.at(1).at(i).substr(pos + 1, sql.at(1).at(i).size());
				temp.push_back(temp1);
				temp.push_back(temp2);
				temp.push_back(std::to_string(1));
				judge.push_back(temp);
			}
		}
		else if (sql.at(1).at(i).find("<") != string::npos) {
			size_t pos = sql.at(1).at(i).find("<");
			string temp1 = sql.at(1).at(i).substr(0, pos);
			string temp2 = sql.at(1).at(i).substr(pos + 1, sql.at(1).at(i).size());
			temp.push_back(temp1);
			temp.push_back(temp2);
			temp.push_back(std::to_string(2));
			judge.push_back(temp);
		}
		else if (sql.at(1).at(i).find(">") != string::npos) {
			size_t pos = sql.at(1).at(i).find(">");
			string temp1 = sql.at(1).at(i).substr(0, pos);
			string temp2 = sql.at(1).at(i).substr(pos + 1, sql.at(1).at(i).size());
			temp.push_back(temp1);
			temp.push_back(temp2);
			temp.push_back(std::to_string(3));
			judge.push_back(temp);
		}
	}
	getfieldV();
	int judge_flag = 0;

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
	/*if (judge_flag) {

	return "ɾ�����в�����";
	}*/
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
	fin1.close();
	//ȡֵ����
	if (judge.size() == 1) {
		int pos1 = 0;
		int pos = 0;
		for (int k = 0; k < vec2.size(); k++) {
			for (int m = 0; m < vec2.at(k).size(); m++) {
				if (vec2.at(k).at(m) == judge.at(0).at(0))
				{
					pos = k;
				}
			}
		}
		//�����ж�����
		int ptemp = 1;
		for (int i = 0; i < rst.size() - 1; i++) {
			if ("'" + judge.at(0).at(1) + "'" == rst.at(i).at(pos)) {
				ptemp = 0;
				rst.erase(rst.begin() + i);
			}
		}if (ptemp) {
			return "δѡ����";
		}
	}
	else if (judge.size() == 2) {
		int pos = 0, poss = 0;
		for (int k = 0; k < vec2.size(); k++) {
			for (int m = 0; m < vec2.at(k).size(); m++) {
				if (vec2.at(k).at(m) == judge.at(0).at(0))
				{
					pos = k;
				}
				else if (vec2.at(k).at(m) == judge.at(1).at(0)) {

					poss = k;
				}
			}

		}

		//�����ж�����
		int ptemp = 1;
		for (int i = 0; i < rst.size() - 1; i++) {
			if (("'" + judge.at(0).at(1) + "'" == rst.at(i).at(pos)) && ("'" + judge.at(1).at(1) + "'" == rst.at(i).at(poss))) {
				ptemp = 0;
				rst.erase(rst.begin() + i);
			}
		}if (ptemp) {
			return "δѡ����";
		}
	}

	//�ж�Լ������
	//default ,not null, unique ,check ,primary key,foreign key




	//д���ļ�
	ofstream out_file;
	string s = "";
	out_file.open(sql.at(0).at(1) + ".trd", ios::out | ios::binary);
	for (int i = 0; i < rst.size(); i++) {
		for (int j = 0; j < rst.at(i).size(); j++) {
			out_file << rst.at(i).at(j);
			out_file << " ";
		}
		out_file << "\r\n";
	}

	out_file.close();
	return "����ɾ���ɹ�";
}

string DataManage::data_update()
{
	vector<vector<string>> modify;

	vector<vector<string>> judge;


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
				string temp2 = sql.at(2).at(i).substr(pos + 1, pos1 - pos - 2);
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
			if (sql.at(2).at(i).find("and") != string::npos) {
				size_t pos1 = sql.at(2).at(i).find("and");
				size_t pos = sql.at(2).at(i).find("<");
				string temp1 = sql.at(2).at(i).substr(0, pos);
				string temp2 = sql.at(2).at(i).substr(pos + 1, pos1 - pos - 2);
				temp.push_back(temp1);
				temp.push_back(temp2);
				temp.push_back(std::to_string(1));
				judge.push_back(temp);
			}
			else {
				size_t pos = sql.at(2).at(i).find("<");
				string temp1 = sql.at(2).at(i).substr(0, pos);
				string temp2 = sql.at(2).at(i).substr(pos + 1, sql.at(1).at(i).size());
				temp.push_back(temp1);
				temp.push_back(temp2);
				temp.push_back(std::to_string(1));
				judge.push_back(temp);
			}
		}
		else if (sql.at(2).at(i).find(">") != string::npos) {
			if (sql.at(2).at(i).find("and") != string::npos) {
				size_t pos1 = sql.at(2).at(i).find("and");
				size_t pos = sql.at(2).at(i).find(">");
				string temp1 = sql.at(2).at(i).substr(0, pos);
				string temp2 = sql.at(2).at(i).substr(pos + 1, pos1 - pos - 2);
				temp.push_back(temp1);
				temp.push_back(temp2);
				temp.push_back(std::to_string(1));
				judge.push_back(temp);
			}
			else {
				size_t pos = sql.at(2).at(i).find(">");
				string temp1 = sql.at(2).at(i).substr(0, pos);
				string temp2 = sql.at(2).at(i).substr(pos + 1, sql.at(1).at(i).size());
				temp.push_back(temp1);
				temp.push_back(temp2);
				temp.push_back(std::to_string(1));
				judge.push_back(temp);
			}
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
	fin1.close();
	//ȡֵ����
	if (judge.size() == 1 && modify.size() == 1) {
		int pos1 = 0;
		int pos = 0;
		for (int k = 0; k < vec2.size(); k++) {
			for (int m = 0; m < vec2.at(k).size(); m++) {

				if (vec2.at(k).at(m) == judge.at(0).at(0))
				{
					pos = k;
				}
				else if (vec2.at(k).at(m) == modify.at(0).at(0)) {
					pos1 = k;
				}
			}
		}

		//�����ж�����
		int ptemp = 1;
		for (int i = 0; i < rst.size(); i++) {
			if ("'" + judge.at(0).at(1) + "'" == rst.at(i).at(pos)) {
				ptemp = 0;
				rst.at(i).at(pos1) = "'" + modify.at(0).at(1) + "'";
			}
		}if (ptemp) {
			return "δѡ����";
		}
	}
	else if (judge.size() == 1 && modify.size() == 2) {
		int pos1 = 0, pos2 = 0;
		int pos = 0;
		for (int k = 0; k < vec2.size(); k++) {
			for (int m = 0; m < vec2.at(k).size(); m++) {

				if (vec2.at(k).at(m) == judge.at(0).at(0))
				{
					pos = k;
				}
				else if (vec2.at(k).at(m) == modify.at(0).at(0)) {
					pos1 = k;
				}
				else if (vec2.at(k).at(m) == modify.at(1).at(0)) {
					pos2 = k;
				}
			}
		}

		//�����ж�����
		int ptemp = 1;
		for (int i = 0; i < rst.size(); i++) {
			if ("'" + judge.at(0).at(1) + "'" == rst.at(i).at(pos)) {
				ptemp = 0;
				rst.at(i).at(pos1) = "'" + modify.at(0).at(1) + "'";
				rst.at(i).at(pos2) = "'" + modify.at(1).at(1) + "'";
			}
		}if (ptemp) {
			return "δѡ����";
		}
	}
	else if (judge.size() == 2 && modify.size() == 1) {
		int pos1 = 0;
		int pos = 0, poss = 0;
		for (int k = 0; k < vec2.size(); k++) {
			for (int m = 0; m < vec2.at(k).size(); m++) {
				if (vec2.at(k).at(m) == judge.at(0).at(0))
				{
					pos = k;
				}
				else if (vec2.at(k).at(m) == judge.at(1).at(0)) {

					poss = k;
				}
				else if (vec2.at(k).at(m) == modify.at(0).at(0)) {
					pos1 = k;
				}

			}

		}

		//�����ж�����
		int ptemp = 1;
		for (int i = 0; i < rst.size() - 1; i++) {

			if (("'" + judge.at(0).at(1) + "'" == rst.at(i).at(pos)) && ("'" + judge.at(1).at(1) + "'" == rst.at(i).at(poss))) {
				ptemp = 0;
				rst.at(i).at(pos1) = "'" + modify.at(0).at(1) + "'";
			}
		}if (ptemp) {
			return "δѡ����";
		}

	}
	else if (judge.size() == 2 && modify.size() == 2) {
		int pos1 = 0, pos2 = 0;
		int pos = 0, poss = 0;
		for (int k = 0; k < vec2.size(); k++) {
			for (int m = 0; m < vec2.at(k).size(); m++) {
				if (vec2.at(k).at(m) == judge.at(0).at(0))
				{
					pos = k;
				}
				else if (vec2.at(k).at(m) == judge.at(1).at(0)) {

					poss = k;
				}
				else if (vec2.at(k).at(m) == modify.at(0).at(0)) {
					pos1 = k;
				}
				else if (vec2.at(k).at(m) == modify.at(1).at(0)) {
					pos2 = k;
				}
			}

		}

		//�����ж�����
		int ptemp = 1;
		for (int i = 0; i < rst.size() - 1; i++) {

			if (("'" + judge.at(0).at(1) + "'" == rst.at(i).at(pos)) && ("'" + judge.at(1).at(1) + "'" == rst.at(i).at(poss))) {
				ptemp = 0;
				rst.at(i).at(pos1) = "'" + modify.at(0).at(1) + "'";
				rst.at(i).at(pos2) = "'" + modify.at(1).at(1) + "'";

			}
		}if (ptemp) {
			return "δѡ����";
		}


	}


	//�ж�Լ������
	//default ,not null, unique ,check ,primary key,foreign key




	//д���ļ�
	ofstream out_file;
	string s = "";
	out_file.open(sql.at(0).at(1) + ".trd", ios::out | ios::binary);
	for (int i = 0; i < rst.size(); i++) {
		for (int j = 0; j < rst.at(i).size(); j++) {
			out_file << rst.at(i).at(j);
			out_file << " ";
		}
		out_file << "\r\n";
	}

	out_file.close();
	return "���ݸ��³ɹ�";


}

string DataManage::data_select()
{
	//�жϱ��Ƿ����
	//string path="";
	//char p[256];
	//FILE *file;
	ifstream in;
	vector<string> line;
	for (size_t i = 0; i < sql[1].size(); i++) {
		/*path = "data//ku//";
		path += sql[1][i];
		path += ".tdf";
		strcpy_s(p, path.c_str());*/
		//in.open("data//kuming//" + sql[1][i] + ".tdf");
		in.open("aa.tdf");
		if (!in.is_open())
		{
			return "���ҵı�����";
		}
		char buffer[128];
		while (!in.eof()) {
			in.getline(buffer, sizeof(buffer));
			line.push_back(buffer);
		}
		contents1.push_back(line);
		line.clear();
		in.close();
	}
	int s_num = sql[0].size();//select column number;
	vector<string> get;
	bool judge = false;
	for (size_t i = 0; i < contents1.size(); i++) {
		vector<string> v = contents1[i];//v����ĳtdf���� 
		for (size_t j = 0; j < v.size(); j++) {
			line = this->explode(v[j], ' ');//line����ĳ������
			for (size_t k = 1; k < sql[0].size(); k++) {
				if (sql[0][k] == line[1]) {
					//�ж��Ƿ����������ж���
					if (std::count(get.begin(), get.end(), line[1]) == 0) {
						get.push_back(line[1]);
						r_slct << sql[0][k];
						r_slct << "/t";
						s_num--;
					}
				}
			}
		}
	}
	if (s_num == 0)judge = true;
	else {
		return "���ҵ��в�����";
	}
	string str = r_slct.str();
	return str;
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
		for (int i = 0; i < vec3.at(j).length(); i++)
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
		}
		if (!isNull)
			return "���ݲ����Ϸǿ�Լ��";
	}

	return "Լ�����ɹ�";
}

string DataManage::con_parse(int pos1, int pos2, vector<vector<string>> vec4)
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
		if (sql.at(1).at(pos1 - 2) != vec4.at(pos2).at(3))
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
					if (sql.at(1).at(i - 2).size() > size&&vec2.at(j).at(3) != "NULL")
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
		for (int i = 0; i < vec1.at(j).length(); i++)
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
