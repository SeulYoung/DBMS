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
			return "数据长度不符合约束";
		}
		string con_c = con_check();
		if (con_c != "约束检查成功") {
			return con_c;
		}

		if (sql.at(0).size() > 2) {									//若有指定列
			for (int i = 0; i < vec2.size(); i++) {					//从tdf文件中的列检查
				isNull = true;
				for (int j = 2; j < sql.at(0).size(); j++) {		//检查sql中对应的列
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
		else {														//若无指定列
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
		return "插入的列中有不存在的列";
	return "数据插入成功";
}





string DataManage::data_delete()
{
	vector<vector<string>> judge;

	//条件判断字符串
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

	return "删除的列不存在";
	}*/
	//取出数据表中的值
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
	//取值更新
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
		//按行判断数据
		int ptemp = 1;
		for (int i = 0; i < rst.size() - 1; i++) {
			if ("'" + judge.at(0).at(1) + "'" == rst.at(i).at(pos)) {
				ptemp = 0;
				rst.erase(rst.begin() + i);
			}
		}if (ptemp) {
			return "未选定行";
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

		//按行判断数据
		int ptemp = 1;
		for (int i = 0; i < rst.size() - 1; i++) {
			if (("'" + judge.at(0).at(1) + "'" == rst.at(i).at(pos)) && ("'" + judge.at(1).at(1) + "'" == rst.at(i).at(poss))) {
				ptemp = 0;
				rst.erase(rst.begin() + i);
			}
		}if (ptemp) {
			return "未选定行";
		}
	}

	//判断约束条件
	//default ,not null, unique ,check ,primary key,foreign key




	//写入文件
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
	return "数据删除成功";
}

string DataManage::data_update()
{
	vector<vector<string>> modify;

	vector<vector<string>> judge;


	//要更新字段字符串
	for (int i = 0; i < sql[1].size(); i++) {
		vector<string> temp;
		size_t pos = sql.at(1).at(i).find("=");
		string temp1 = sql.at(1).at(i).substr(0, pos);
		string temp2 = sql.at(1).at(i).substr(pos + 1, sql.at(1).at(i).size());
		temp.push_back(temp1);
		temp.push_back(temp2);
		modify.push_back(temp);
	}
	//条件判断字符串
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

	//检查要插入的列是否已经存在
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

	return "修改的列不存在";
	}*/
	//取出数据表中的值
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
	//取值更新
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

		//按行判断数据
		int ptemp = 1;
		for (int i = 0; i < rst.size(); i++) {
			if ("'" + judge.at(0).at(1) + "'" == rst.at(i).at(pos)) {
				ptemp = 0;
				rst.at(i).at(pos1) = "'" + modify.at(0).at(1) + "'";
			}
		}if (ptemp) {
			return "未选定行";
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

		//按行判断数据
		int ptemp = 1;
		for (int i = 0; i < rst.size(); i++) {
			if ("'" + judge.at(0).at(1) + "'" == rst.at(i).at(pos)) {
				ptemp = 0;
				rst.at(i).at(pos1) = "'" + modify.at(0).at(1) + "'";
				rst.at(i).at(pos2) = "'" + modify.at(1).at(1) + "'";
			}
		}if (ptemp) {
			return "未选定行";
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

		//按行判断数据
		int ptemp = 1;
		for (int i = 0; i < rst.size() - 1; i++) {

			if (("'" + judge.at(0).at(1) + "'" == rst.at(i).at(pos)) && ("'" + judge.at(1).at(1) + "'" == rst.at(i).at(poss))) {
				ptemp = 0;
				rst.at(i).at(pos1) = "'" + modify.at(0).at(1) + "'";
			}
		}if (ptemp) {
			return "未选定行";
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

		//按行判断数据
		int ptemp = 1;
		for (int i = 0; i < rst.size() - 1; i++) {

			if (("'" + judge.at(0).at(1) + "'" == rst.at(i).at(pos)) && ("'" + judge.at(1).at(1) + "'" == rst.at(i).at(poss))) {
				ptemp = 0;
				rst.at(i).at(pos1) = "'" + modify.at(0).at(1) + "'";
				rst.at(i).at(pos2) = "'" + modify.at(1).at(1) + "'";

			}
		}if (ptemp) {
			return "未选定行";
		}


	}


	//判断约束条件
	//default ,not null, unique ,check ,primary key,foreign key




	//写入文件
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
	return "数据更新成功";


}

string DataManage::data_select()
{
	//判断表是否存在
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
			return "查找的表不存在";
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
		vector<string> v = contents1[i];//v代表某tdf内容 
		for (size_t j = 0; j < v.size(); j++) {
			line = this->explode(v[j], ' ');//line代表某行内容
			for (size_t k = 1; k < sql[0].size(); k++) {
				if (sql[0][k] == line[1]) {
					//判断是否在其他表中读到
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
		return "查找的列不存在";
	}
	string str = r_slct.str();
	return str;
}

//判断是否是已存在的列
bool DataManage::isColumn()
{
	//读取.tdf文件
	getfieldV();

	//检查要插入的列是否已经存在
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
		return"请求表不存在。";
	}
	//生成vec3
	while (!in.eof())
	{
		char buffer[100];
		in.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec3.push_back(buffer);
	}
	in.close();

	//生成vec4
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

	//检查约束
	string isSuc;
	for (int i = 2; i < sql.at(0).size(); i++) {
		for (int j = 0; j < vec4.size(); j++) {
			if (sql.at(0).at(i) == vec4.at(j).at(1)) {
				isSuc = con_parse(i, j, vec4);
				if (isSuc != "约束检查成功")
					return isSuc;
			}
		}
	}

	//检查not null;
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
			return "数据不符合非空约束";
	}

	return "约束检查成功";
}

string DataManage::con_parse(int pos1, int pos2, vector<vector<string>> vec4)
{
	string con_re = "约束检查成功";

	if (vec4.at(pos2).at(2) == "primary")
	{

	}
	else if (vec4.at(pos2).at(2) == "foreign")
	{

	}
	else if (vec4.at(pos2).at(2) == "default")
	{
		if (sql.at(1).at(pos1 - 2) != vec4.at(pos2).at(3))
			con_re = "数据不符合default约束";

	}
	else
	{
		con_re = "约束检查成功";
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
	//生成vec1
	while (!in.eof())
	{
		char buffer[100];
		in.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec1.push_back(buffer);
	}
	in.close();

	//生成vec2
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
