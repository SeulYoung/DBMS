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
	//读取tdf文件
	getfieldV();
	return "数据删除成功";
}

string DataManage::data_update()
{
	ofstream out_file;
	string s = "";
	out_file.open(sql.at(0).at(1) + ".trd", ios::out | ios::app | ios::binary);
	bool isNull = true;
	//截取字符串 得到指定行

	//要更新字段字符串
	for (int i = 0; i < sql[1].size(); i++) {
		size_t pos = sql.at(1).at(i).find("=");
		string temp1 = sql.at(1).at(i).substr(0, pos);
		string temp2 = sql.at(1).at(i).substr(pos + 1, sql.at(1).at(i).size());
	}
	//条件判断字符串




	//判断指定行是否存在
	//判断约束条件
	//取值更新



	return "ok";

}

string DataManage::data_select()
{
	return string();
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
			if (!isNull)
				return "数据不符合非空约束";
		}
	}
	
	return "约束检查成功";
}

string DataManage::con_parse(int pos1, int pos2,vector<vector<string>> vec4)
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
		if (sql.at(1).at(pos1-2)!=vec4.at(pos2).at(3))
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
