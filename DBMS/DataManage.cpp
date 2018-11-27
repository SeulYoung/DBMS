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
	out_file.open("./data/" + dbName + "/" + sql.at(0).at(1) + + ".trd", ios::out | ios::app);
	bool isNull = true;
	string msg;

	msg = isColumn();
	if (msg=="存在")
	{
		if (!type_check()) {
			return "数据类型不符合约束";
		}
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
		return msg;
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
	string path = "./data/" + dbName + "/" + sql.at(0).at(1) + ".trd";
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
			if ( judge.at(0).at(1)  == rst.at(i).at(pos)) {
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
			if (( judge.at(0).at(1)  == rst.at(i).at(pos)) && ( judge.at(1).at(1)  == rst.at(i).at(poss))) {
				ptemp = 0;
				rst.erase(rst.begin() + i);
			}
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
	out_file.open("./data/" + dbName + "/" + sql.at(0).at(1) + ".trd", ios::out | ios::binary);
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
	string path = "./data/" + dbName + "/" + sql.at(0).at(1)  + ".trd";
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
		for (int i = 0; i < rst.size() -1; i++) {
				if (judge.at(0).at(1) == rst.at(i).at(pos)) {
					ptemp = 0;
					rst.at(i).at(pos1) = modify.at(0).at(1) ;
				}
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
		for (int i = 0; i < rst.size() -1; i++) {
			if ( judge.at(0).at(1) == rst.at(i).at(pos)) {
				ptemp = 0;
				rst.at(i).at(pos1) = modify.at(0).at(1) ;
				rst.at(i).at(pos2) = modify.at(1).at(1) ;
			}
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
			if (( judge.at(0).at(1)  == rst.at(i).at(pos)) && ( judge.at(1).at(1)  == rst.at(i).at(poss))) {
				ptemp = 0;
				rst.at(i).at(pos1) = modify.at(0).at(1) ;
			}
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
			if (( judge.at(0).at(1) == rst.at(i).at(pos)) && ( judge.at(1).at(1)  == rst.at(i).at(poss))) {
				ptemp = 0;
				rst.at(i).at(pos1) = modify.at(0).at(1) ;
				rst.at(i).at(pos2) = modify.at(1).at(1) ;

			}
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
	vector<string> vec3;
	vector<vector<string>> vec4;

	ifstream in("./data/" + dbName + "/" + sql.at(0).at(1)  + ".tic");
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
	int curtemp = 1;
	//约束表逐行判断
	/*for (int i = 0; i < vec4.size(); i++) {
		for (int j = 0; j < vec2.size(); j++) {
			if (vec4.at(i).at(1) == vec2.at(j).at(1)) {
				if (vec4.at(i).at(2) == "unique") {
					for (int k = 0; k < rst.size(); k++){
						for (int h = 0; h < rst.size(); h++)
							if (rst.at(k).at(j) == rst.at(k).at(j+h))
								return "违反唯一约束";
					}
				}
				else if (vec4.at(i).at(2) == "primary")
				{

				}
				else if (vec4.at(i).at(2) == "foreign")
				{

				}
				else if (vec4.at(i).at(2) == "not") {
					for (int k = 0; k < rst.size(); k++) {
						if (rst.at(k).at(j) == "null") {
							return "违反非空约束";
						}
					}
				}
			}
		
		}
	}*/



	//写入文件
	ofstream out_file;
	string s = "";
	out_file.open("./data/" + dbName + "/" + sql.at(0).at(1) + ".trd", ios::out | ios::binary);
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
	string s = "";
	//获得.tdf内容
	ifstream in("./data/" + dbName + "/" + sql.at(1).at(0) + ".tdf");
	if (!in.is_open())
		return false;

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

	//获得.trd内容
	vector<string> vec3;
	vector<vector<string>> content;
	ifstream in1("./data/" + dbName + "/" + sql.at(1).at(0) + ".trd");
	if (!in1.is_open())
		return "请求表不存在";

	while (!in1.eof())
	{
		char buffer[100];
		in1.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec3.push_back(buffer);
	}
	in1.close();

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
		content.push_back(temp_vec);
	}

	fin_s = content;

	if (sql.size() != 2) {

		//where中比较的判断
		if (sql.at(2).at(0) == "where") {
			int sig;
			for (int i = 1; i < sql.at(2).size(); i++) {

				if (sql[2][i].find("and") != string::npos && sql[2][i].find("between") == string::npos) {
					size_t pos = sql.at(2).at(i).find("and");
					string temp = sql.at(2).at(i).substr(0, pos - 1);
					sql.at(2).erase(sql.at(2).begin() + i);
					sql.at(2).insert(sql.at(2).begin() + i, temp);
					
					sig = 0;
				}
				else if (sql[2][i].find("or") != string::npos) {
					size_t pos = sql.at(2).at(i).find("or");
					string temp = sql.at(2).at(i).substr(0, pos - 1);
					sql.at(2).erase(sql.at(2).begin() + i);
					sql.at(2).insert(sql.at(2).begin() + i , temp);
					
					sig = 1;
				}
				if (sql[2][i].find(">") != string::npos || sql[2][i].find("<") != string::npos || sql[2][i].find("=") != string::npos || sql[2][i].find("<>") != string::npos)
				{
					if (sig == 1) {
						if (compare(i, sql.at(2).at(2)) == "列不存在")
							return "列不存在";
						break;
					}
					else
						if (compare(i, "null") == "列不存在")
							return "列不存在";
				}

				//where中其他的判断
				if (sql[2][i].find("between") != string::npos)
				{
					vector<string>  condition = explode(sql[2][1], ' ');
					int pos1 = -1;
					for (int j = 0; j < vec2.size(); j++) {
						if (condition.at(0) == vec2.at(j).at(1)) {
							pos1 = j;
							break;
						}
					}
					if (pos1 == -1)	return "列不存在";

					for (int n = 0; n < fin_s.size(); n++)
						if (atoi(fin_s.at(n).at(pos1).c_str()) < atoi(condition.at(2).c_str()) || atoi(fin_s.at(n).at(pos1).c_str()) > atoi(sql.at(2).at(2).c_str()))
						{
							fin_s.erase(fin_s.begin() + n);
							n--;
						}
					break;
				}
				else if (sql[2][i].find("in") != string::npos) {
					size_t p = sql.at(2).at(i).find("(");
					string fieldS;
					if (sql[2][i].find("not") != string::npos)
						fieldS = sql.at(2).at(i).substr(0, p - 7);
					else
						fieldS = sql.at(2).at(i).substr(0, p - 3);
					string tem = sql.at(2).at(i).substr(p + 1, sql.at(2).at(i).size());
					vector<string>  condition = explode(tem, ',');

					int pos1 = -1;
					for (int j = 0; j < vec2.size(); j++) {
						if (fieldS == vec2.at(j).at(1)) {
							pos1 = j;
							break;
						}
					}
					if (pos1 == -1)
						return "列不存在";
					if (sql[2][i].find("not") != string::npos) {
						
							for (int m = 0; m < condition.size(); m++) {
								for (int n = 0; n < fin_s.size(); n++) {
									if (fin_s.at(n).at(pos1) == condition.at(m)) {
										fin_s.erase(fin_s.begin() + n);
										n--;
									}
								}
						}
					}
					else {
						bool signal = false;
						for (int n = 0; n < fin_s.size(); n++) {
							signal = false;
							for (int m = 0; m < condition.size(); m++) {
								if (fin_s.at(n).at(pos1) == condition.at(m)) {
									signal = true;
									break;
								}
							}
							if (!signal) {
								fin_s.erase(fin_s.begin() + n);
								n--;
							}
								
						}
					}
					break;
				}
				else if (sql[2][i].find("like") != string::npos) {
					size_t p = sql.at(2).at(i).find("like");
					string fieldS;
					fieldS = sql.at(2).at(i).substr(0, p - 1);
					string tem = sql.at(2).at(i).substr(p + 5, sql.at(2).at(i).size());
					vector<string>  condition = explode(tem, '%');

					int pos1 = -1;
					for (int j = 0; j < vec2.size(); j++) {
						if (fieldS == vec2.at(j).at(1)) {
							pos1 = j;
							break;
						}
					}
					if (pos1 == -1)
						return "列不存在";
					//匹配like
					for (int n = 0; n < fin_s.size(); n++) {
						size_t position;
						if (condition.at(0) == "'") {
							position = fin_s.at(n).at(pos1).find(condition.at(1));
							if (position != string::npos && position > 1);
							else {
								fin_s.erase(fin_s.begin() + n);
								n--;
							}
								
						}
						else if (condition.at(1) == "'") {
							position = fin_s.at(n).at(pos1).find(condition.at(0));
							if (position != string::npos&&position < fin_s.at(n).at(pos1).size());
							else {
								fin_s.erase(fin_s.begin() + n);
								n--;
							}
								
						}
					}
				}
				else if (sql[2][i].find("is null") != string::npos) {
					vector<string>  condition = explode(sql[2][i], ' ');
					int pos1 = -1;
					for (int j = 0; j < vec2.size(); j++) {
						if (condition[0] == vec2.at(j).at(1)) {
							pos1 = j;
							break;
						}
					}
					if (pos1 == -1)
						return "列不存在";
					for (int n = 0; n < fin_s.size(); n++) {
						if (fin_s.at(n).at(pos1) != "NULL") {
							fin_s.erase(fin_s.begin() + n);
							n--;
						}
							
					}
				}
			}
			if (sql.size() > 3) {
				//group by
				if (sql.at(3).at(0) == "group by") {
									
					vector<vector<string>> temp;
					
					int pos = -1;
					for (int j = 0; j < vec2.size(); j++) {
						if (sql.at(3).at(1) == vec2.at(j).at(1)) {
							pos = j;
							break;
						}
					}
					if (pos == -1)
						return "列不存在";
					
					vector<string> group_type;
					vector<vector<string>> pre_temp = fin_s;
					vector<int> num;			//记录第一次分组每个组的数量
					int number = 0;
					
					for (int m = 0; m < pre_temp.size(); m++) {
						group_type.push_back(pre_temp.at(m).at(pos));
						string g = pre_temp.at(m).at(pos);
						for (int i = 0; i < pre_temp.size(); i++) {
							if (pre_temp.at(i).at(pos) == g) {
								pre_temp.erase(pre_temp.begin() + i);
								i--;
								number++;
							}
						}
						num.push_back(number);
						number = 0;
						m--;
					}
					for (int i = 0; i < num.size(); i++) {
						for (int j = 0; j < fin_s.size(); j++) {
							if (fin_s.at(j).at(pos) == group_type.at(i))
								temp.push_back(fin_s.at(j));
						}
					}
					fin_s = temp;
					//若有第二个group
					if (sql.at(3).size() > 2) {
						int pos = -1;
						for (int j = 0; j < vec2.size(); j++) {
							if (sql.at(3).at(2) == vec2.at(j).at(1)) {
								pos = j;
								break;
							}
						}
						if (pos == -1)
							return "列不存在";

						vector<vector<string>> temp1;
						vector<vector<string>> pre_temp1 = fin_s;
						for (int n = 0; n < num.size(); n++) {
							vector<string> group_type1;
							
							vector<int> num1;			//记录第一次分组每个组的数量
							int number1 = 0;

							int l = num.at(n);
							for (int m = 0; m < l; m++) {
								group_type1.push_back(pre_temp1.at(m).at(pos));
								string g = pre_temp1.at(m).at(pos);
								
								for (int i = 0; i < l ; i++) {
									if (pre_temp1.at(i).at(pos) == g) {
										pre_temp1.erase(pre_temp1.begin() + i);
										i--;
										number1++;
										l--;
									}
								}
								
								num1.push_back(number1);
								number1 = 0;
								m--;
							}
							int sum = 0;
							for (int d = 0; d < n; d++)
								sum += num.at(d);
							for (int i = 0; i < num1.size(); i++) {
								for (int j = 0; j < num.at(n); j++) {
									if (fin_s.at(j+sum).at(pos) == group_type1.at(i))
										temp1.push_back(fin_s.at(j+sum));
								}
							}
						}
						fin_s = temp1;
					}
				}
				else if (sql.at(3).at(0) == "order by") {
					int pos = -1;
					for (int j = 0; j < vec2.size(); j++) {
						if (sql.at(2).at(1) == vec2.at(j).at(1)) {
							pos = j;
							break;
						}
					}
					if (pos == -1)
						return "列不存在";

					vector<vector<string>> temp;
					vector<string> temp1;
					if (sql.at(2).size() == 2) {

						int leng = fin_s.size();

						for (int j = 0; j < leng; j++) {
							int pos1 = 0;
							temp1 = fin_s.at(0);
							for (int i = 0; i < fin_s.size() - 1; i++) {
								if (temp1.at(pos) > fin_s.at(i + 1).at(pos)) {
									temp1 = fin_s.at(i + 1);
									pos1 = i + 1;
								}
							}
							fin_s.erase(fin_s.begin() + pos1);
							temp.push_back(temp1);
						}
					}
					else {
						if (sql.at(2).at(2) == "asc") {
							int leng = fin_s.size();

							for (int j = 0; j < leng; j++) {
								int pos1 = 0;
								temp1 = fin_s.at(0);
								for (int i = 0; i < fin_s.size() - 1; i++) {
									if (temp1.at(pos) > fin_s.at(i + 1).at(pos)) {
										temp1 = fin_s.at(i + 1);
										pos1 = i + 1;
									}
								}
								fin_s.erase(fin_s.begin() + pos1);
								temp.push_back(temp1);
							}
						}
						else if (sql.at(2).at(2) == "desc") {
							int leng = fin_s.size();

							for (int j = 0; j < leng; j++) {
								int pos1 = 0;
								temp1 = fin_s.at(0);
								for (int i = 0; i < fin_s.size() - 1; i++) {
									if (temp1.at(pos) < fin_s.at(i + 1).at(pos)) {
										temp1 = fin_s.at(i + 1);
										pos1 = i + 1;
									}
								}
								fin_s.erase(fin_s.begin() + pos1);
								temp.push_back(temp1);
							}
						}
					}
					fin_s = temp;
				}
				//有了group by之后的东西
				if (sql.size() > 4) {

				}
			}


		}
		else if (sql.at(2).at(0) == "order by") {
			int pos = -1;
			for (int j = 0; j < vec2.size(); j++) {
				if (sql.at(2).at(1) == vec2.at(j).at(1)) {
					pos = j;
					break;
				}
			}
			if (pos == -1)
				return "列不存在";

			vector<vector<string>> temp;
			vector<string> temp1;
			if (sql.at(2).size() == 2) {

				int leng = fin_s.size();

				for (int j = 0; j < leng; j++) {
					int pos1 = 0;
					temp1 = fin_s.at(0);
					for (int i = 0; i < fin_s.size() - 1; i++) {
						if (temp1.at(pos) > fin_s.at(i + 1).at(pos)) {
							temp1 = fin_s.at(i + 1);
							pos1 = i + 1;
						}
					}
					fin_s.erase(fin_s.begin() + pos1);
					temp.push_back(temp1);
				}
			}
			else {
				if (sql.at(2).at(2) == "asc") {
					int leng = fin_s.size();

					for (int j = 0; j < leng; j++) {
						int pos1 = 0;
						temp1 = fin_s.at(0);
						for (int i = 0; i < fin_s.size() - 1; i++) {
							if (temp1.at(pos) > fin_s.at(i + 1).at(pos)) {
								temp1 = fin_s.at(i + 1);
								pos1 = i + 1;
							}
						}
						fin_s.erase(fin_s.begin() + pos1);
						temp.push_back(temp1);
					}
				}
				else if (sql.at(2).at(2) == "desc") {
					int leng = fin_s.size();

					for (int j = 0; j < leng; j++) {
						int pos1 = 0;
						temp1 = fin_s.at(0);
						for (int i = 0; i < fin_s.size() - 1; i++) {
							if (temp1.at(pos) < fin_s.at(i + 1).at(pos)) {
								temp1 = fin_s.at(i + 1);
								pos1 = i + 1;
							}
						}
						fin_s.erase(fin_s.begin() + pos1);
						temp.push_back(temp1);
					}
				}

			}
			fin_s = temp;
		}


	}
	//判断是否为*
	if (sql.at(0).size() == 2 && sql.at(0).at(1) == "*")
	{
		for (int i = 0; i < vec2.size(); i++) {
			s += vec2.at(i).at(1);
			s += "\t";
		}
		s += "\n";
		for (int i = 0; i < fin_s.size(); i++) {
			for (int j = 0; j < fin_s.at(i).size(); j++) {
				s += fin_s.at(i).at(j);
				s += "\t";
			}
			s += "\n";
		}
	}
	else {
		vector<int> sec_co;
		for (int i = 1; i < sql.at(0).size(); i++)
		{
			int pos = -1;
			for (int j = 0; j < vec2.size(); j++) {
				if (sql.at(0).at(i) == vec2.at(j).at(1)) {
					pos = j;
					break;
				}
			}
			if (pos == -1)
				return "列不存在";

			sec_co.push_back(pos);
			s += sql.at(0).at(i);
			s += "\t";
		}
		s += "\n";
		for (int i = 0; i < fin_s.size(); i++)
		{
			for (int j = 0; j < sec_co.size(); j++) {
				s += fin_s.at(i).at(sec_co.at(j));
				s += "\t";
			}
			s += "\n";
		}
	}
	return s;

}

string DataManage::compare(int pos, string s3) {
	int size = fin_s.size();
	if (s3 == "null") {
		if (sql.at(2).at(pos).find("=") != string::npos) {
			vector<string>  condition = explode(sql[2][pos], '=');
			int pos1 = -1;
			for (int j = 0; j < vec2.size(); j++) {
				if (condition.at(0) == vec2.at(j).at(1)) {
					pos1 = j;
					break;
				}
			}
			if (pos1 == -1)	return "列不存在";
			for (int i = 0; i < size; i++)
				if (fin_s.at(i).at(pos1) != condition.at(1)) {
					fin_s.erase(fin_s.begin() + i);
					size--;
					i--;
				}
					
		}
		else if (sql.at(2).at(pos).find("<>") != string::npos) {
			vector<string>  condition = explode(sql[2][pos], '<>');
			string str = condition.at(0).substr(0, condition.at(0).size() - 1);
			int pos1 = -1;
			for (int j = 0; j < vec2.size(); j++) {
				if (str == vec2.at(j).at(1)) {
					pos1 = j;
					break;
				}
			}
			if (pos1 == -1)	return "列不存在";
			for (int i = 0; i < size; i++)
				if (fin_s.at(i).at(pos1) == condition.at(1)) {
					fin_s.erase(fin_s.begin() + i);
					size--;
					i--;
				}
		}
		else if (sql.at(2).at(pos).find(">") != string::npos) {
			vector<string>  condition = explode(sql[2][pos], '>');
			int pos1 = -1;
			for (int j = 0; j < vec2.size(); j++) {
				if (condition.at(0) == vec2.at(j).at(1)) {
					pos1 = j;
					break;
				}
			}
			if (pos1 == -1)	return "列不存在";
			string s1;
			string s2 = condition.at(1).substr(1, condition.at(1).size() - 1);
			for (int i = 0; i < size; i++) {
				s1 = fin_s.at(i).at(pos1).substr(1, fin_s.at(i).at(pos1).size());
				if (atoi(s1.c_str()) <= atoi(s2.c_str())) {
					fin_s.erase(fin_s.begin() + i);
					size--;
					i--;
				}
			}
				
		}
		else if (sql.at(2).at(pos).find("<") != string::npos) {
			vector<string>  condition = explode(sql[2][pos], '<');
			int pos1 = -1;
			for (int j = 0; j < vec2.size(); j++) {
				if (condition.at(0) == vec2.at(j).at(1)) {
					pos1 = j;
					break;
				}
			}
			if (pos1 == -1)	return "列不存在";
			string s1;
			string s2 = condition.at(1).substr(1, condition.at(1).size() - 1);
			for (int i = 0; i < size; i++) {
				s1 = fin_s.at(i).at(pos1).substr(1, fin_s.at(i).at(pos1).size());
				if (atoi(s1.c_str()) >= atoi(s2.c_str())) {
					fin_s.erase(fin_s.begin() + i);
					size--;
					i--;
				}
			}
				
		}
	}
	else {
		if (sql.at(2).at(pos).find("=") != string::npos) {
			vector<string>  condition1 = explode(sql[2][pos], '=');
			vector<string>  condition2 = explode(s3, '=');
			int pos1 = -1;
			for (int j = 0; j < vec2.size(); j++) {
				if (condition1.at(0) == vec2.at(j).at(1) || condition2.at(0) == vec2.at(j).at(1)) {
					pos1 = j;
					break;
				}
			}
			if (pos1 == -1)	return "列不存在";
			for (int i = 0; i < size; i++)
				if (fin_s.at(i).at(pos1) != condition1.at(1) && fin_s.at(i).at(pos1) != condition2.at(1)) {
					fin_s.erase(fin_s.begin() + i);
					size--;
					i--;
				}
		}
		else if (sql.at(2).at(pos).find("<>") != string::npos) {
			vector<string>  condition1 = explode(sql[2][pos], '<>');
			vector<string>  condition2 = explode(s3, '<>');
			string str1 = condition1.at(0).substr(0, condition1.at(0).size() - 1);
			string str2 = condition2.at(0).substr(0, condition2.at(0).size() - 1);
			int pos1 = -1;
			for (int j = 0; j < vec2.size(); j++) {
				if (str1 == vec2.at(j).at(1) || str2 == vec2.at(j).at(1)) {
					pos1 = j;
					break;
				}
			}
			if (pos1 == -1)	return "列不存在";
			for (int i = 0; i < size; i++)
				if (fin_s.at(i).at(pos1) == condition1.at(1) || fin_s.at(i).at(pos1) == condition2.at(1)) {
					fin_s.erase(fin_s.begin() + i);
					size--;
					i--;
				}
		}
		else if (sql.at(2).at(pos).find(">") != string::npos) {
			vector<string>  condition1 = explode(sql[2][pos], '>');
			vector<string>  condition2 = explode(s3, '>');
			int pos1 = -1;
			for (int j = 0; j < vec2.size(); j++) {
				if (condition1.at(0) == vec2.at(j).at(1) || condition2.at(0) == vec2.at(j).at(1)) {
					pos1 = j;
					break;
				}
			}
			if (pos1 == -1)	return "列不存在";
			string s1;
			string s2 = condition1.at(1).substr(1, condition1.at(1).size() - 1);
			string s3 = condition2.at(1).substr(1, condition2.at(1).size() - 1);
			for (int i = 0; i < size; i++) {
				s1 = fin_s.at(i).at(pos1).substr(1, fin_s.at(i).at(pos1).size());
				if (atoi(s1.c_str()) <= atoi(s2.c_str()) && atoi(s1.c_str()) <= atoi(s3.c_str())) {
					fin_s.erase(fin_s.begin() + i);
					size--;
					i--;
				}
			}
				
		}
		else if (sql.at(2).at(pos).find("<") != string::npos) {
			vector<string>  condition1 = explode(sql[2][pos], '<');
			vector<string>  condition2 = explode(s3, '<');
			int pos1 = -1;
			for (int j = 0; j < vec2.size(); j++) {
				if (condition1.at(0) == vec2.at(j).at(1) || condition2.at(0) == vec2.at(j).at(1)) {
					pos1 = j;
					break;
				}
			}
			if (pos1 == -1)	return "列不存在";
			string s1;
			string s2 = condition1.at(1).substr(1, condition1.at(1).size() - 1);
			string s3 = condition2.at(1).substr(1, condition2.at(1).size() - 1);
			for (int i = 0; i < size; i++) {
				s1 = fin_s.at(i).at(pos1).substr(1, fin_s.at(i).at(pos1).size());
				if (atoi(s1.c_str()) >= atoi(s2.c_str()) || atoi(s1.c_str()) >= atoi(s3.c_str())) {
					fin_s.erase(fin_s.begin() + i);
					size--;
					i--;
				}
			}
				
		}
	}
	return "比较成功";

}

//判断是否是已存在的列
string DataManage::isColumn()
{
	//读取.tdf文件
	bool isTdf = getfieldV();
	if (!isTdf)
		return "请求表不存在";

	//检查要插入的列是否已经存在
	bool signal = false;
	if (sql.at(0).size() > 2) {
		for (int i = 2; i < sql.at(0).size(); i++) {
			for (int j = 0; j < vec2.size(); j++) {
				if (sql.at(0).at(i) == vec2.at(j).at(1))
					signal = true;
			}
			if (!signal)
				return "存在";
		}
	}
	else
		signal = true;
	return "存在";

}

string DataManage::con_check()
{
	vector<string> vec3;
	vector<vector<string>> vec4;

	ifstream in("./data/" + dbName + "/" + sql.at(0).at(1) + ".tic");
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
	if (sql.at(0).size() > 2) {
		for (int i = 2; i < sql.at(0).size(); i++) {
			for (int j = 0; j < vec4.size(); j++) {
				if (vec4.at(j).at(1).find(',') != string::npos) {
					vector<string> temp= explode(vec4.at(j).at(1),',');
					for (int m = 0; m < temp.size(); m++) {
						if (sql.at(0).at(i) == temp.at(m)) {
							isSuc = con_parse(i - 2, j, vec4);
							if (isSuc != "约束检查成功")
								return isSuc;
						}
					}
				}
				else {
					if (sql.at(0).at(i) == vec4.at(j).at(1)) {
						isSuc = con_parse(i - 2, j, vec4);
						if (isSuc != "约束检查成功")
							return isSuc;
					}
				}
				
			}
		}
	}
	else {
		for (int i = 0; i < vec2.at(i).size(); i++) {
			for (int j = 0; j < vec4.size(); j++) {
				if (vec4.at(j).at(1).find(',') != string::npos) {
					vector<string> temp = explode(vec4.at(j).at(1), ',');
					for (int m = 0; m < temp.size(); m++) {
						if (vec2.at(i).at(1) == temp.at(m)) {
							isSuc = con_parse(i, j, vec4);
							if (isSuc != "约束检查成功")
								return isSuc;
						}
					}
				}
				else {
					if (vec2.at(i).at(1) == vec4.at(j).at(1)) {
						isSuc = con_parse(i, j, vec4);
						if (isSuc != "约束检查成功")
							return isSuc;
					}
				}
				
			}
		}
	}
	
	//检查not null;
	bool isNull = false;
	if (sql.at(0).size() > 2) {
		for (int j = 0; j < vec4.size(); j++) {
			if (vec4.at(j).at(2) == "not"|| vec4.at(j).at(2) == "primary") {
				for (int i = 2; i < sql.at(0).size(); i++) {
					if (sql.at(0).at(i) == vec4.at(j).at(1)) {
						isNull = true;
						break;
					}
				}
				if (!isNull)
				return "数据不符合非空约束";
				isNull = false;
			}
			
		}
		
	}

	return "约束检查成功";
}

string DataManage::con_parse(int pos1, int pos2, vector<vector<string>> vec4)
{
	//读取.trd文件中的记录
	vector<string> vec5;
	vector<vector<string>> vec6;
	ifstream in("./data/" + dbName + "/" + sql.at(0).at(1) + ".trd");
	if (!in.is_open())
		return "请求表不存在";

	//生成vec5
	while (!in.eof())
	{
		char buffer[100];
		in.getline(buffer, sizeof(buffer));
		if (strlen(buffer) != 0)
			vec5.push_back(buffer);
	}
	in.close();

	//生成vec6
	for (size_t j = 0; j < vec5.size(); j++) {
		vector<string> temp_vec;
		char *temp3;
		char temp4[100];
		for (int i = 0; i < vec5.at(j).length(); i++)
			temp4[i] = vec5.at(j)[i];
		temp4[vec5.at(j).length()] = '\0';

		temp3 = strtok(temp4, " ");
		while (temp3) {
			temp_vec.push_back(temp3);
			temp3 = strtok(NULL, " ");
		}
		vec6.push_back(temp_vec);
	}

	string con_re = "约束检查成功";

	if (vec4.at(pos2).at(2) == "primary"|| vec4.at(pos2).at(2)=="unique")
	{
		int order;
		for (int i = 0; i < vec2.size(); i++) {
			if (vec4.at(pos2).at(1) == vec2.at(i).at(1))
			{
				order = i;
				break;
			}
		}
		for (int i = 0; i < vec6.size(); i++) {
			if (sql.at(1).at(pos1) == vec6.at(i).at(order))
				return "数据不符合unique约束";
		}
	}
	else if (vec4.at(pos2).at(2) == "checkbetween")
	{
		if (sql.at(1).at(pos1) < vec4.at(pos2).at(3)|| sql.at(1).at(pos1) > vec4.at(pos2).at(4))
			con_re = "数据不符合between约束";
	}
	else if (vec4.at(pos2).at(2) == "checkin")
	{
		bool sig = false;
		for (int i = 3; i < vec4.at(pos2).size(); i++) {
			if (sql.at(1).at(pos1) == vec4.at(pos2).at(i)) {
				sig = true;
				break;
			}
		}
		if (!sig)
			con_re = "数据不符合check in约束";
	}
	else if (vec4.at(pos2).at(2) == "check>=<")
	{
		if (vec4.at(pos2).size() < 10) {
			for (int i = 3; i < vec4.at(pos2).size(); i = i + 3) {
				if (vec4.at(pos2).at(i + 1) == "="){
					if (sql.at(1).at(pos1) != vec4.at(pos2).at(i + 2))
						con_re = "数据不符合check>=<约束";
				}
				else if (vec4.at(pos2).at(i + 1) == ">") {
					if (sql.at(1).at(pos1) <= vec4.at(pos2).at(i + 2))
						con_re = "数据不符合check>=<约束";
				}
				else if (vec4.at(pos2).at(i + 1) == "<") {
					if (sql.at(1).at(pos1) >= vec4.at(pos2).at(i + 2))
						con_re = "数据不符合check>=<约束";
				}
			}
		}
		
	}
	else
	{
		con_re = "约束检查成功";
	}
	return con_re;
}

bool DataManage::type_check()
{	
	if (sql.at(0).size() > 2) {
		for (int i = 2; i < sql.at(0).size(); i++) {
			for (int j = 0; j < vec2.size(); j++) {
				if (sql.at(0).at(i) == vec2.at(j).at(1))
				{
					if (vec2.at(j).at(2) == "varchar"&&sql.at(1).at(i - 2).find("'") == string::npos)
						return false;
					else if (vec2.at(j).at(2) == "varchar2"&&sql.at(1).at(i-2).find("'") == string::npos)
						return false;
					else if (vec2.at(j).at(2) == "integer"&&sql.at(1).at(i - 2).find("'") != string::npos)
						return false;
					else if (vec2.at(j).at(2) == "bool") {
						if (sql.at(1).at(i - 2) != "true" || sql.at(1).at(i - 2) != "false")
							return false;
					}
					else if (vec2.at(j).at(2) == "date"&&sql.at(1).at(i - 2).find("to_date") == string::npos)
						return false;	
				}
			}
		}
	}
	else {
		for (int i = 0; i < sql.at(1).size(); i++) {

			if (vec2.at(i).at(2) == "varchar"&&sql.at(1).at(i).find("'") == string::npos)
				return false;
			else if (vec2.at(i).at(2) == "varchar2"&&sql.at(1).at(i).find("'") == string::npos)
				return false;
			else if (vec2.at(i).at(2) == "integer"&&sql.at(1).at(i).find("'") != string::npos)
				return false;
			else if (vec2.at(i).at(2) == "bool") {
				if (sql.at(1).at(i) != "true" || sql.at(1).at(i) != "false")
					return false;
			}
			else if (vec2.at(i).at(2) == "date"&&sql.at(1).at(i).find("to_date") == string::npos)
				return false;
		}
	}
	return true;
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
			int size;
			if (vec2.at(i).at(3) != "NULL")
			{
				size = atoi(vec2.at(i).at(3).c_str())+2;
				if (sql.at(1).at(i).size() > size)
					return false;
			}
				
		}
	}
	return true;
}

bool DataManage::getfieldV()
{
	ifstream in("./data/" + dbName + "/" + sql.at(0).at(1) + ".tdf");
	if (!in.is_open())
		return false;
	
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
	return true;
}

