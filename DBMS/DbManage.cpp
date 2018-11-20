#include "DbManage.h"

/*int main() {
	string count;
	while (1) {
		cout << "Create a database use '-c' and delete a database use '-d'" << endl;
		cout << "command:";
		cin >> count;
		DbManage db;

		if (count == "-c")
			db.CreateDatabase();
		else if (count == "-d")
			db.DeleteDatabase();
	}

	return 0;
}*/


DbManage::DbManage(vector<vector<string>> s)
{
	sql = s;
}

DbManage::~DbManage()
{

}


void DbManage::CreateDatabase()
{

	char d_name[128];
	std::string name = sql[0][1];
	strcpy_s(d_name,name.c_str());
	char d_path[256];
	bool d_type = false;

	time_t now;
	struct tm today;
	time(&now);
	char d_time[128];
	localtime_s(&today, &now);
	strftime(d_time, 128, "%D %H:%M:%S", &today);//获取当前时间

												 //cout << d_name.size() << endl;
	if (sizeof(d_name) > 128)
	{
		//cout << "Size of Database name should less than 128" << endl;
		return;
	}

	ofstream out;
	FILE *file;
	FILE *new_file;
	if ((fopen_s(&file, "ruanko.db", "r")) != 0) {
		_mkdir("data//ruanko");
		fopen_s(&file, "data//ruanko//ruanko.tb", "w");
		fclose(file);
		fopen_s(&file, "data//ruanko//ruanko.log", "w");
		fclose(file);
		fopen_s(&file, "ruanko.db", "w");
		fclose(file);
		out.open("ruanko.db",ios::binary);
		string str = "Name\t\tPath\t\tCreate_at\t\tType\n";
		out << str;
		str ="ruanko\t\tdata//ruanko\t\t"+(string)d_time+"\t\ttrue\n";
		out << str;
	}
	else {
		fclose(file);
		out.open("ruanko.db",ios::binary|ios::app);
	}


	char new_name[256];
/*	if (d_path != "") {
		string d_file = d_path + "//" + d_name;
		strcpy_s(new_name, d_file.c_str());
	}
	else {
		d_path = " ";
		strcpy_s(new_name, d_name.c_str());
	}
*/
	strcpy_s(new_name, "data//");
	strcat_s(new_name, d_name);
	strcpy_s(d_path, new_name);
	strcat_s(new_name, "//");
	strcat_s(new_name, d_name);
	strcat_s(new_name, ".log");
	if ((fopen_s(&new_file, new_name, "r")) == 0) {
		cout << "Existed database!" << endl;
		//fclose(new_file);
		fclose(file);
		return;
	}
	else {
		//string command;
		string s = d_path;
		_mkdir(s.c_str());
		fopen_s(&new_file, new_name, "w");
		fclose(new_file);
		fopen_s(&new_file, new_name, "w");
		fclose(new_file);
		fopen_s(&new_file, new_name, "w");
		fclose(new_file);
		string a1 = d_name;
		string a2 = d_path;
		string a3 = d_time;

		string s1 = a1 + "\t" + a2 + "\t" + a3 + "\t" + "false" + "\n";
		out << s1;
		/*char str[1024];
		strcpy_s(str, s1.c_str());
		fwrite(str, sizeof(char), strlen(str), file);*/
	}
	out.close();
	fclose(file);
	fclose(new_file);
}

void DbManage::DeleteDatabase()
{
	//vector<database> vd ;
	char new_name[256];
	string d_name;
	//SYSTEMTIME d_time;
	string d_path;
	string d_type;
	getchar();
	cout << "Enter database name:";
	getline(cin, d_name);
	d_name += ".db";
	cout << "Enter save path:";
	getline(cin, d_path);
	if (d_path == "")d_path = " ";
	cout << "Enter database type:";
	getline(cin, d_type);

	database del_database;
	strcpy_s(del_database.name, d_name.c_str());
	strcpy_s(del_database.path, d_path.c_str());
	strcpy_s(del_database.type, d_type.c_str());

	FILE *f;
	FILE *f1;
	if ((fopen_s(&f, "ruanko.db", "r")) != 0) {
		cout << "No database in this system" << endl;
		return;
	}
	fclose(f);

	string d_file;
	if (d_path != " ") {
		d_file = d_path + "//" + d_name;
	}
	else {
		d_file = d_name;
	}
	strcpy_s(new_name, d_file.c_str());
	//检测库是否存在
	if ((fopen_s(&f1, new_name, "r")) != 0) {
		cout << "This database is not existed" << endl;
		fclose(f);
		return;
	}
	fclose(f1);

	//删除ruanko.db中关于该数据库的相关信息
	ifstream ifs;
	streampos pos;
	string strFile = "";
	ifs.open("ruanko.db");
	string str;
	pos = ifs.tellg();
	getline(ifs, str);
	while (!ifs.eof()) {
		vector<string> v{ this->explode(str,'\t') };
		database d;
		strcpy_s(d.name, v[0].c_str());
		strcpy_s(d.path, v[1].c_str());
		strcpy_s(d.type, v[3].c_str());
		if (d.name == d_name) {
			while (!ifs.eof()) {
				getline(ifs, str);
				strFile += str;
				if (str != "")
					strFile += "\n";
			}
			break;
		}
		//vd.push_back(d);
		strFile += str;
		strFile += "\n";
		pos = ifs.tellg();
		getline(ifs, str);
	}
	ifs.close();

	ofstream ofs("ruanko.db", ios::binary);
	if (!ofs) {
		cout << "fail to open file" << endl;
		return;
	}
	ofs << strFile;
	ofs.close();
	//进度到这需要改成文件指针操作删除
	if (remove(new_name) == 0) {
		cout << "success" << endl;
	}
}

const vector<string> DbManage::explode(const string & s, const char & c)
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
