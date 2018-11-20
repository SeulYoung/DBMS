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
	out_file.open(sql.at(0).at(1) + ".trd", ios::out | ios::app | ios::binary);
	bool isNull = true;
	string msg;
	
	if (isColumn())
	{
		len_check();
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
	//读取tdf文件
	getfieldV();
	return "数据删除成功";
}

string DataManage::data_update()
{
	ofstream out_file;
	string s = "";
	vector<vector<string>> modify;
	
	vector<vector<string>> judge;
	out_file.open(sql.at(0).at(1) + ".trd", ios::out | ios::app | ios::binary);
	bool isNull = true;
	//截取字符串 得到指定行

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

	//判断约束条件



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
	//取值更新
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
	int s_num=sql[0].size();//select column number;
	vector<string> get;
	bool judge = false;
	for (size_t i = 0; i < contents1.size(); i++) {
		vector<string> v = contents1[i];//v代表某tdf内容 
		for (size_t j = 0; j < v.size(); j++) {
			line = this->explode(v[j], ' ');//line代表某行内容
			for (size_t k = 1; k < sql[0].size(); k++) {
				if (sql[0][k]==line[1]) {
					//判断是否在其他表中读到
					if (std::count(get.begin(),get.end(),line[1])==0) {
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
	string str=r_slct.str();
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
	return false;
}

bool DataManage::len_check()
{
	//读取文件
	if (sql.at(0).size() > 2) {
		for (int i = 2; i < sql.at(0).size(); i++) {

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
		exit(1);
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
