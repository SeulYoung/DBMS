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
}

vector<vector<string>> DbManage::getDbs()
{
	return vector<vector<string>>();
}

void DbManage::CreateDatabase()
{

	string d_name;
	//SYSTEMTIME d_time;
	string d_path;
	string d_type;

	cout << "Enter database name:";
	getline(cin, d_name);
	d_name += ".db";
	cout << "Enter save path:";
	getline(cin, d_path);
	cout << "Enter database type:";
	getline(cin, d_type);

	//GetLocalTime(&d_time);
	time_t now;
	struct tm today;
	time(&now);
	char d_time[128];
	localtime_s(&today, &now);
	strftime(d_time, 128, "%D %H:%M:%S", &today);//获取当前时间

												 //cout << d_name.size() << endl;
	if (d_name.size() > 128)
	{
		cout << "Size of Database name should less than 128" << endl;
		return;
	}
	FILE *file;
	FILE *new_file;
	if ((fopen_s(&file, "ruanko.db", "r")) != 0) {
		fopen_s(&file, "ruanko.db", "w");
		const char *str = "Name\t\tPath\t\tCreate_at\t\tType\n";
		fwrite(str, sizeof(char), strlen(str), file);
	}
	else {
		fclose(file);
		fopen_s(&file, "ruanko.db", "ab+");
	}


	char new_name[256];
	if (d_path != "") {
		string d_file = d_path + "//" + d_name;
		strcpy_s(new_name, d_file.c_str());
	}
	else {
		d_path = " ";
		strcpy_s(new_name, d_name.c_str());
	}

	if ((fopen_s(&new_file, new_name, "r")) == 0) {
		cout << "Existed database!" << endl;
		//fclose(new_file);
		fclose(file);
		return;
	}
	else {
		fopen_s(&new_file, new_name, "w");
		string s1 = d_name + "\t" + d_path + "\t" + d_time + "\t" + d_type + "\n";
		char str[1024];
		strcpy_s(str, s1.c_str());
		fwrite(str, sizeof(char), strlen(str), file);
	}
	fclose(file);
	fclose(new_file);
	return;
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
