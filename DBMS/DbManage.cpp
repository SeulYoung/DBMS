#include "DbManage.h"

DbManage::DbManage(vector<vector<string>> s)
{
	sql = s;
}

string DbManage::CreateDatabase()
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
		return "库名超出长度";
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
	strcat_s(new_name, sql[0][1].c_str());
	strcat_s(new_name, "//");
	strcpy_s(d_path, new_name);
	strcat_s(new_name, d_name);
	strcpy_s(new_name1, new_name);
	strcat_s(new_name, ".log");
	strcat_s(new_name1, ".tb");
	if ((fopen_s(&new_file, new_name, "r")) == 0) {
		//cout << "Existed database!" << endl;
		//fclose(new_file);
		fclose(file);
		return "数据库已存在!";
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
	return "数据库创建成功";
}

string DbManage::DeleteDatabase()
{
	char new_name[256];
	string d_name;
	string d_path;

	d_name =sql[0][1];
	d_path = "data//"+sql[0][1]+"//";

	database del_database;
	strcpy_s(del_database.name, d_name.c_str());
	strcpy_s(del_database.path, d_path.c_str());

	FILE *f;
	FILE *f1;
	if ((fopen_s(&f, "ruanko.db", "r")) != 0) {
		return "No database in this system";
	}
	fclose(f);
	if (d_name == "ruanko") {
		return "不能删除系统数据库";
	}
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
	d_file = d_path + d_name + ".tb";
	strcpy_s(new_name, d_file.c_str());
	//检测库是否存在
	if ((fopen_s(&f1, new_name, "r")) != 0) {
		return "数据库不存在";
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
		//cout << "fail to open file" << endl;
		return "打开库管理文件失败";
	}
	ofs << strFile;
	ofs.close();
	//进度到这需要改成文件指针操作删除
	string s = "data//" + sql[0][1];
	strcpy_s(new_name,s.c_str());
	if (DeleteDirectory(new_name)) {
		return "删除成功";
	}
	return "删除失败";
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


BOOL  DbManage::IsDirectory(const char *pDir)
{
	char szCurPath[500];
	ZeroMemory(szCurPath, 500);
	sprintf_s(szCurPath, 500, "%s//*", pDir);
	WIN32_FIND_DATAA FindFileData;
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));

	HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData); /**< find first file by given path. */

	if (hFile == INVALID_HANDLE_VALUE)
	{
		FindClose(hFile);
		return FALSE; /** 如果不能找到第一个文件，那么没有目录 */
	}
	else
	{
		FindClose(hFile);
		return TRUE;
	}

}

BOOL  DbManage::DeleteDirectory(const char * DirName)
{
	//    CFileFind tempFind;        //声明一个CFileFind类变量，以用来搜索
	char szCurPath[MAX_PATH];        //用于定义搜索格式
	_snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName);    //匹配格式为*.*,即该目录下的所有文件
	WIN32_FIND_DATAA FindFileData;
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));
	HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData);
	BOOL IsFinded = TRUE;
	while (IsFinded)
	{
		IsFinded = FindNextFileA(hFile, &FindFileData);    //递归搜索其他的文件
		if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..")) //如果不是"." ".."目录
		{
			std::string strFileName = "";
			strFileName = strFileName + DirName + "//" + FindFileData.cFileName;
			std::string strTemp;
			strTemp = strFileName;
			if (IsDirectory(strFileName.c_str())) //如果是目录，则递归地调用
			{
				printf("目录为:%s/n", strFileName.c_str());
				DeleteDirectory(strTemp.c_str());
			}
			else
			{
				DeleteFileA(strTemp.c_str());
			}
		}
	}
	FindClose(hFile);

	BOOL bRet = RemoveDirectoryA(DirName);
	if (bRet == 0) //删除目录
	{
		printf("删除%s目录失败！/n", DirName);
		return FALSE;
	}
	return TRUE;
}