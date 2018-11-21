#include "DbManage.h"

DbManage::DbManage(vector<vector<string>> s)
{
	sql = s;
}

void DbManage::CreateDatabase()
{

	char d_name[128];
	std::string name = sql[0][1];
	strcpy_s(d_name, name.c_str());
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
		out.open("ruanko.db", ios::binary);
		string str;
		str = "ruanko\tdata//ruanko//\t" + (string)d_time + "\ttrue\n";
		out << str;
	}
	else {
		fclose(file);
		out.open("ruanko.db", ios::binary | ios::app);
	}


	char new_name[256];
	char new_name1[256];
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
	strcat_s(new_name, "//");
	strcpy_s(d_path, new_name);
	strcat_s(new_name, d_name);
	strcpy_s(new_name1, new_name);
	strcat_s(new_name, ".log");
	strcat_s(new_name1, ".tb");
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
		fopen_s(&new_file, new_name1, "w");
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
	//检测是否有人正在使用库
	/*CString strFilename = _T(new_name);
	CFile file;
	file.Open(strFilename, CFile::modeReadWrite);
	if (!file.GetFileName().IsEmpty())
	{
	cout << "文件被占用" << endl;
	return;
	}*/
	//file.Close();

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

string DbManage::ShowDatabase()
{
	ifstream in("ruanko.db");
	vector<string> show;
	char buffer[128];
	while (!in.eof()) {
		in.getline(buffer, sizeof(buffer));
		if (buffer[0] == '\0')break;
		show.push_back(buffer);
	}
	stringstream ssr;
	for (int i = 0; i < show.size(); i++) {
		ssr << (this->explode(show[i], '\t')).at(0);
		ssr << "\n";
	}
	ssr << "数据库中存在库个数为:";
	ssr << show.size() ;
	
	string str=ssr.str();
	in.close();
	return str;
}

string DbManage::UseDatabase()
{
	ifstream in("ruanko.db");
	vector<string> use;
	char buffer[128];
	while (!in.eof()) {
		in.getline(buffer, sizeof(buffer));
		if (buffer[0] == '\0')break;
		vector<string> v =this->explode(buffer,'\t');
		use.push_back(v[0]);
	}
	in.close();
	string str;
	if (std::count(use.begin(), use.end(), sql.at(0).at(1)) == 0) {
		return "false";
	}
	else {
		str = sql.at(0).at(1);
	}
	return str;
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