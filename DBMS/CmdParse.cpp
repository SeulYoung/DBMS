#include "CmdParse.h"

CmdParse::CmdParse()
{
	dbName = "ruanko";
}

string CmdParse::sqlCheck(string s)
{
	regex dShow("^show databases;$");
	regex dUse("^use \\w+;$");
	regex dCreate("^create database \\w+;$");
	regex dDrop("^drop database \\w+;$");
	regex tCreate("^create table \\w+\\s?\\(.+\\);$");
	regex tAlter("^alter table \\w+\\s(add|modify|drop)\\s(column|constraint)\\s.+;$");
	regex tDrop("^drop table \\w+;$");
	regex tInsert("^insert into \\w+\\s?(\\(.+\\))?\\svalues\\s?\\(.+\\);$");
	regex tDelete("^delete from \\w+\\swhere\\s?\\(?.+\\)?;$");
	regex tUpdate("^update \\w+\\sset\\s(\\s?\\w+=\\w+\\s?,)*(\\s?\\w+=\\w+\\s)(where.+)?;$");
	regex tSelect("^select.+from.+(where.+)?((group by.+)?|(having.+)?|(order by.+)?);$");

	sql = preSql(s);
	if (regex_match(sql, dShow))
		return dbShow();
	else if (regex_match(sql, dUse))
		return dbUse();
	else if (regex_match(sql, dCreate))
		return dbCreate();
	else if (regex_match(sql, dDrop))
		return dbDrop();
	else if (regex_match(sql, tCreate))
		return tableCreate();
	else if (regex_match(sql, tAlter))
		return tableAlter();
	else if (regex_match(sql, tDrop))
		return tableDrop();
	else if (regex_match(sql, tInsert))
		return tableInsert();
	else if (regex_match(sql, tDelete))
		return  tableDelete();
	else if (regex_match(sql, tUpdate))
		return  tableUpdate();
	else if (regex_search(sql, tSelect))
		return tableSelect();

	return "不支持的Sql语句";
}

vector<vector<string>> CmdParse::getDbs()
{
	vector<vector<string>> dbs;

	ifstream in("ruanko.db");
	if(!in.is_open())
		return dbs;

	char buff[512];
	while (true)
	{
		in.getline(buff, sizeof(buff));
		char *dname = strtok(buff, "\t");
		char *dpath = strtok(NULL, "\t");
		if (dname != NULL)
		{
			ifstream table(string(dpath) + string(dname) + ".tb");
			if (table.is_open())
			{
				while (true)
				{
					vector<string> db;
					db.push_back(dname); // 数据库名

					char buff[512];
					table.getline(buff, sizeof(buff));
					char *tname = strtok(buff, "\t");
					char *tpath = strtok(NULL, "\t");
					if (tname != NULL)
					{
						db.push_back(tname); // 表名
						ifstream field(tpath);
						if (field.is_open())
							while (true)
							{
								field.getline(buff, sizeof(buff));
								strtok(buff, " ");
								char *fname = strtok(NULL, " ");
								if (fname != NULL)
									db.push_back(fname); // 字段名
								else
									break;
							}
						field.close();
					}
					else
					{
						dbs.push_back(db); 
						break;
					}
					dbs.push_back(db);
				}
			}
			else
			{
				vector<string> db{ dname };
				dbs.push_back(db); // 空数据库
			}
			table.close();
		}
		else
			break;
	}
	in.close();

	return dbs;
}

vector<vector<string>> CmdParse::getTableInfo(string db, string table)
{
	vector<vector<string>> tableInfo;

	ifstream in("./data/" + db + "/" + table + ".tdf");
	if (!in.is_open())
		return tableInfo;

	vector<string> field;
	char buff[512];
	while (true)
	{
		in.getline(buff, sizeof(buff));
		strtok(buff, " ");
		char *name = strtok(NULL, " ");
		if (name != NULL)
			field.push_back(name);
		else
			break;
	}
	tableInfo.push_back(field);
	in.close();

	in.open("./data/" + db + "/" + table + ".trd");
	if (!in.is_open())
		return tableInfo;

	while (true)
	{
		in.getline(buff, sizeof(buff));
		vector<string> info;
		char *s = strtok(buff, " ");
		if (s == NULL)
			break;
		info.push_back(s);
		while ((s = strtok(NULL, " ")) != NULL)
			info.push_back(s);
		tableInfo.push_back(info);
	}
	in.close();

	return tableInfo;
}

vector<vector<string>> CmdParse::getField(string db, string table, string col)
{
	vector<vector<string>> field;

	ifstream in("./data/" + db + "/" + table + ".tdf");
	if (!in.is_open())
		return field;

	char buff[512];
	while (true)
	{
		in.getline(buff, sizeof(buff));
		strtok(buff, " ");
		char *n = strtok(NULL, " ");
		if (n == NULL)
			break;
		if (string(n) != col)
			continue;
		char *t = strtok(NULL, " ");
		vector<string> info{ n, t };
		field.push_back(info);
	}
	in.close();

	in.open("./data/" + db + "/" + table + ".tic");
	if (!in.is_open())
		return field;

	while (true)
	{
		in.getline(buff, sizeof(buff));
		char *n = strtok(buff, " ");
		char *f = strtok(NULL, " ");
		if (f == NULL)
			break;
		if (string(f) != col)
			continue;
		char *c = strtok(NULL, "");
		vector<string> info{ n, c };
		field.push_back(info);
	}
	in.close();

	return field;
}

string CmdParse::dbShow()
{
	vector<vector<string>> vShow;

	vector<string> name;
	name.push_back("show");
	string s = sql.substr(5, sql.size() - 6);
	name.push_back(s);
	vShow.push_back(name);

	DbManage dm(vShow);
	string str = dm.ShowDatabase();
	return str;
}

string CmdParse::dbUse()
{
	vector<vector<string>> vUse;

	vector<string> name;
	name.push_back("use");
	string s = sql.substr(4, sql.size() - 5);
	name.push_back(s);
	vUse.push_back(name);

	DbManage dm(vUse);
	dbName = dm.UseDatabase();
	if (dbName == "false") {
		dbName = "ruanko";
		return "不存在该数据库";
	}
	else {
		return "切换为使用" + dbName + "数据库";
	}
}

string CmdParse::dbCreate()
{
	vector<vector<string>> vCreate;

	vector<string> name;
	name.push_back("create");
	string s = sql.substr(16, sql.size() - 17);
	name.push_back(s);
	vCreate.push_back(name);

	DbManage dm(vCreate);
	string str = dm.CreateDatabase();
	return str;
}

string CmdParse::dbDrop()
{
	vector<vector<string>> vDrop;

	vector<string> name;
	name.push_back("drop");
	string s = sql.substr(14, sql.size() - 15);
	name.push_back(s);
	vDrop.push_back(name);

	DbManage dbManage(vDrop);
	string str = dbManage.DeleteDatabase();
	return str;
}

string CmdParse::tableCreate()
{
	vector<vector<string>> vCreate;

	int off1;
	if ((off1 = sql.find('(')) != string::npos)
	{
		vector<string> create;
		create.push_back("create");
		string s = sql.substr(13, off1 - 13);
		create.push_back(s);
		vCreate.push_back(create);
	}
	else
		return "create table语句后存在错误";

	int off2;
	if ((off2 = sql.rfind(')')) != string::npos)
	{
		regex rer("(.+?,)|(.+)");
		smatch rsm;
		string s = sql.substr(off1 + 1, off2 - off1);
		string::const_iterator st = s.begin();
		string::const_iterator en = s.end();

		while (regex_search(st, en, rsm, rer))
		{
			vector<string> create;
			string ss = rsm.str();
			bool isname = true;
			bool isatt = false;
			bool iscap = false;
			bool iscon = false;
			string name = "";
			string attribute = "";
			string capacity = "";
			string constraint = "";
			for (int i = 0; i < ss.size(); i++)
			{
				if (isname == true && ss[i] != ' ')
					name = name + ss[i];
				if (isname == true && name != "" && ss[i] == ' ')
				{
					isname = false;
					isatt = true;
					create.push_back(name);
					continue;
				}

				if (isatt == true && ss[i] != ' ' && ss[i] != '(' && ss[i] != ',')
					attribute = attribute + ss[i];
				if (isatt == true && attribute != "" && (ss[i] == '(' || ss[i] == ',' || ss[i] == ' ' || i == ss.size() - 1))
				{
					isatt = false;
					if (ss[i] == '(')
						iscap = true;
					else
						iscon = true;
					create.push_back(preWhere(attribute));
					continue;
				}

				if (iscap == true && ss[i] != ' ' && ss[i] != '(' && ss[i] != ')')
					capacity = capacity + ss[i];
				if (iscap == true && capacity != "" && ss[i] == ')')
				{
					iscap = false;
					iscon = true;
					create.push_back(capacity);
					continue;
				}

				if (iscon == true && ss[i] != ',' && ss[i] != ')')
					constraint = constraint + ss[i];
				if (iscon == true && constraint != "" && (ss[i] == ',' || ss[i] == ')'))
				{
					iscon = false;
					create.push_back(constraint);
					continue;
				}
			}
			vCreate.push_back(create);
			st = rsm[0].second;
		}
	}
	else
		return "括号中语句存在错误";

	TableManage tableManage(vCreate, dbName);
	string str;
	if (tableManage.CreatDatebase(str) == 1) {
		FieldManage fm(vCreate, dbName);
		string s = fm.manage();
		return s;
	}
	else
	{
		return str;
	}
}

string CmdParse::tableAlter()
{
	vector<vector<string>> vAlter;

	int off1;
	if ((off1 = sql.find(' ', 12)) != string::npos)
	{
		vector<string> alter;
		alter.push_back("alter");
		string s = sql.substr(12, off1 - 12);
		alter.push_back(s);
		vAlter.push_back(alter);
	}
	else
		return "alter table语句后存在错误";

	int off2;
	if ((off2 = sql.find("column")) != string::npos)
	{
		vector<string> temp;
		temp.push_back(sql.substr(off1 + 1, off2 - off1 + 5));
		vAlter.push_back(temp);

		regex rer("(.+?,)|(.+)");
		smatch rsm;
		string s = sql.substr(off2 + 7, sql.size() - off2 - 8);
		string::const_iterator st = s.begin();
		string::const_iterator en = s.end();

		while (regex_search(st, en, rsm, rer))
		{
			vector<string> alter;
			string ss = rsm.str();
			bool isname = true;
			bool isatt = false;
			bool iscap = false;
			string name = "";
			string attribute = "";
			string capacity = "";
			for (int i = 0; i < ss.size(); i++)
			{
				if (isname == true && ss[i] != ' ' && ss[i] != ',')
					name = name + ss[i];
				if (isname == true && name != "" && (ss[i] == ' ' || ss[i] == ',' || i == ss.size() - 1))
				{
					isname = false;
					isatt = true;
					alter.push_back(name);
					continue;
				}

				if (isatt == true && ss[i] != ' ' && ss[i] != '(' && ss[i] != ',')
					attribute = attribute + ss[i];
				if (isatt == true && attribute != "" && (ss[i] == '(' || ss[i] == ',' || i == ss.size() - 1))
				{
					isatt = false;
					iscap = true;
					alter.push_back(attribute);
					continue;
				}

				if (iscap == true && ss[i] != ' ' && ss[i] != '(' && ss[i] != ')')
					capacity = capacity + ss[i];
				if (iscap == true && capacity != "" && ss[i] == ')')
				{
					iscap = false;
					alter.push_back(capacity);
					continue;
				}
			}
			vAlter.push_back(alter);
			st = rsm[0].second;
		}
	}
	else if ((off2 = sql.find("constraint")) != string::npos)
	{
		vector<string> temp;
		temp.push_back(sql.substr(off1 + 1, off2 - off1 + 9));
		int off3;
		if ((off3 = sql.find(' ', off2 + 11)) != string::npos)
		{
			temp.push_back(sql.substr(off2 + 11, off3 - off2 - 11));
			vAlter.push_back(temp);

			bool isadd = false;
			while (true)
			{
				int off4;
				if ((off4 = sql.find('(', off3 + 1)) != string::npos)
				{
					vector<string> alter;
					alter.push_back(sql.substr(off3 + 1, off4 - off3 - 1));
					int off5 = sql.find(')', off4 + 1);
					alter.push_back(sql.substr(off4 + 1, off5 - off4 - 1));
					vAlter.push_back(alter);
					off3 = off5 + 1;
					isadd = true;
				}
				else if (!isadd)
				{
					vector<string> alter;
					off4 = sql.find(' ', off3 + 1);
					alter.push_back(sql.substr(off3 + 1, off4 - off3 - 1));
					alter.push_back(sql.substr(off4 + 1, sql.size() - off4 - 2));
					vAlter.push_back(alter);
					isadd = true;
				}
				else
					break;
			}
		}
		else
		{
			temp.push_back(sql.substr(off2 + 11, sql.size() - off2 - 12));
			vAlter.push_back(temp);
		}
	}
	else
		return "参数语句存在错误";

	TableManage tableManage(vAlter, dbName);
	FieldManage fm(vAlter, dbName);
	string msg = fm.manage();
	return msg;
	//return "Alter table成功";
}

string CmdParse::tableDrop()
{
	vector<vector<string>> vDrop;

	vector<string> name;
	name.push_back("drop");
	string s = sql.substr(11, sql.size() - 12);
	name.push_back(s);
	vDrop.push_back(name);

	TableManage tableManage(vDrop, dbName);
	string str;
	if (tableManage.DeleteDatebase(str) == 1) {
		return "删除表成功";
	}
	else
	{
		return str;
	};
}

string CmdParse::tableInsert()
{
	vector<vector<string>> vInsert;

	int off1;
	if ((off1 = sql.find("values")) != string::npos)
	{
		vector<string> insert;
		insert.push_back("insert");
		smatch rsm;
		string s = sql.substr(12, off1 - 12);
		if (s[s.size() - 1] == ' ')
			s.erase(s.size() - 1);

		int off2;
		if ((off2 = s.find("(")) != string::npos)
		{
			regex rr2(".+?(?=\\()");
			smatch rsm2;
			regex_search(s, rsm2, rr2);
			insert.push_back(rsm2.str());

			string ss = s.substr(off2 + 1, s.size() - off2 - 2);
			smatch rsm1;
			string stt = "";
			for (int i = 0; i < ss.size(); i++)
			{
				if (ss[i] != ' ' && ss[i] != ',' && ss[i] != '(' && ss[i] != ')')
					stt = stt + ss[i];

				if ((ss[i] == ',' || i == ss.size() - 1) && stt != "")
				{
					insert.push_back(stt);
					stt = "";
				}
			}
		}
		else
			insert.push_back(s);

		vInsert.push_back(insert);
	}
	else
		return "insert into语句后存在错误";

	int off2;
	if ((off2 = sql.rfind(")")) != string::npos)
	{
		vector<string> insert;
		string s = sql.substr(off1 + 7, off2 - off1 - 7);

		string stt = "";
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] != ' ' && s[i] != ',' && s[i] != '(' && s[i] != ')')
				stt = stt + s[i];

			if ((s[i] == ',' || i == s.size() - 1) && stt != "")
			{
				insert.push_back(stt);
				stt = "";
			}
		}
		vInsert.push_back(insert);
	}
	else
		return "values语句后存在错误";

	DataManage dataManage(vInsert, dbName);
	string msg = dataManage.manage();
	return msg;
	//return "Insert数据成功";
}


string CmdParse::tableDelete()
{
	vector<vector<string>> vDelete;

	int off1;
	if ((off1 = sql.find("where")) != string::npos)
	{
		vector<string> vdelete;
		vdelete.push_back("delete");
		string s = sql.substr(12, off1 - 13);
		vdelete.push_back(s);
		vDelete.push_back(vdelete);
	}
	else
		return "delete from与where之间存在错误";

	int off2;
	if ((off2 = sql.rfind(";")) != string::npos)
	{
		vector<string> vdelete;
		regex rr("(.+?and )|(.+?or )");
		smatch rrsm;
		string s = preWhere(sql.substr(off1 + 6, off2 - off1));
		string::const_iterator st = s.begin();
		string::const_iterator en = s.end();
		bool flag = false;

		while (regex_search(st, en, rrsm, rr))
		{
			flag = true;
			vdelete.push_back(rrsm.str());
			st = rrsm[0].second;
		}
		if (flag)
		{
			regex_search(st, en, rrsm, regex(".+"));
			vdelete.push_back(rrsm.str());
		}
		else
			vdelete.push_back(s);

		vDelete.push_back(vdelete);
	}
	else
		return "where语句后存在错误";

	DataManage dataManage(vDelete, dbName);
	string msg = dataManage.manage();
	return msg;
	//return "Delete 数据成功";
}

string CmdParse::tableUpdate()
{
	vector<vector<string>> vUpdate;

	int off1;
	if ((off1 = sql.find("set")) != string::npos)
	{
		vector<string> update;
		update.push_back("update");
		string s = sql.substr(7, off1 - 8);
		update.push_back(s);
		vUpdate.push_back(update);
	}
	else
		return "update与set语句间存在错误";

	int off2;
	if ((off2 = sql.rfind("where")) != string::npos || (off2 = sql.rfind(";")) != string::npos)
	{
		vector<string> update;
		string s = sql.substr(off1 + 4, off2 - off1 - 5);
		string stt = "";
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] != ' ' && s[i] != ',')
				stt = stt + s[i];

			if ((s[i] == ',' || i == s.size() - 1) && stt != "")
			{
				update.push_back(stt);
				stt = "";
			}
		}
		vUpdate.push_back(update);
	}
	else
		return "set字段存在错误";

	int off3;
	if (sql.rfind("where") != string::npos && (off3 = sql.rfind(";")) != string::npos)
	{
		vector<string> update;
		regex rr("(.+?and )|(.+?or )");
		smatch rrsm;
		string s = preWhere(sql.substr(off2 + 6, off3 - off2 - 6));
		string::const_iterator st = s.begin();
		string::const_iterator en = s.end();
		bool flag = false;

		while (regex_search(st, en, rrsm, rr))
		{
			flag = true;
			update.push_back(rrsm.str());
			st = rrsm[0].second;
		}
		if (flag)
		{
			regex_search(st, en, rrsm, regex(".+"));
			update.push_back(rrsm.str());
		}
		else
			update.push_back(s);

		vUpdate.push_back(update);
	}

	/*DataManage dataManage(vUpdate);
	return "Update 数据成功";*/
	DataManage dataManage(vUpdate, dbName);
	string msg = dataManage.manage();
	return msg;
}

string CmdParse::tableSelect()
{
	vector<vector<string>> vSelect;

	int off1;
	if ((off1 = sql.find("from")) != string::npos)
	{
		vector<string> attribute;
		attribute.push_back("select");

		string s = sql.substr(7, off1 - 7);
		string stt = "";
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] != ' ' && s[i] != ',')
				stt = stt + s[i];

			if ((s[i] == ',' || i == s.size() - 1) && stt != "")
			{
				attribute.push_back(stt);
				stt = "";
			}
		}
		vSelect.push_back(attribute);
	}
	else
		return "select from语句后存在错误";

	vector<vector<string>> temp;
	int off2;
	if ((off2 = sql.rfind("order by")) != string::npos)
	{
		vector<string> condition;
		condition.push_back("order by");

		string ss = "";
		int off3;
		if ((off3 = sql.rfind("asc")) != string::npos || (off3 = sql.rfind("desc")) != string::npos)
			ss = sql.substr(off3, sql.size() - off3 - 1);
		else
			off3 = sql.size() - 1;

		string s = sql.substr(off2 + 9, off3 - off2 - 9);
		string stt = "";
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] != ' ' && s[i] != ',')
				stt = stt + s[i];

			if ((s[i] == ',' || i == s.size() - 1) && stt != "")
			{
				condition.push_back(stt);
				stt = "";
			}
		}
		if (ss != "")
			condition.push_back(ss);

		temp.push_back(condition);
	}
	else
		off2 = sql.size() - 1;

	int off3;
	if ((off3 = sql.rfind("having")) != string::npos)
	{
		vector<string> condition;
		condition.push_back("having");

		regex sr("(.+?and )|(.+?or )");
		smatch srsm;
		string s = preWhere(sql.substr(off3 + 7, off2 - off3 - 7));
		string::const_iterator st = s.begin();
		string::const_iterator en = s.end();
		bool flag = false;

		while (regex_search(st, en, srsm, sr))
		{
			flag = true;
			condition.push_back(srsm.str());
			st = srsm[0].second;
		}
		if (flag)
		{
			regex_search(st, en, srsm, regex(".+"));
			condition.push_back(srsm.str());
		}
		else
			condition.push_back(s);

		temp.push_back(condition);
	}
	else
		off3 = off2;

	int off4;
	if ((off4 = sql.rfind("group by")) != string::npos)
	{
		vector<string> condition;
		condition.push_back("group by");

		string s = sql.substr(off4 + 9,off3 - off4 - 9);
		string stt = "";
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] != ' ' && s[i] != ',')
				stt = stt + s[i];

			if ((s[i] == ',' || i == s.size() - 1) && stt != "")
			{
				condition.push_back(stt);
				stt = "";
			}
		}
		temp.push_back(condition);
	}
	else
		off4 = off3;

	int off5;
	if ((off5 = sql.rfind("where")) != string::npos)
	{
		vector<string> condition;
		condition.push_back("where");

		regex sr("(.+?and )|(.+?or )");
		smatch srsm;
		string s = preWhere(sql.substr(off5 + 6, off4 - off5 - 6));
		string::const_iterator st = s.begin();
		string::const_iterator en = s.end();
		bool flag = false;

		while (regex_search(st, en, srsm, sr))
		{
			flag = true;
			condition.push_back(srsm.str());
			st = srsm[0].second;
		}
		if (flag)
		{
			regex_search(st, en, srsm, regex(".+"));
			condition.push_back(srsm.str());
		}
		else
			condition.push_back(s);

		temp.push_back(condition);
	}
	else
		off5 = off4;

	int off6;
	if ((off6 = sql.rfind(";")) != string::npos)
	{
		vector<string> tablename;
		string s = sql.substr(off1 + 5, off5 - off1 - 5);
		string stt = "";
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] != ' ' && s[i] != ',')
				stt = stt + s[i];

			if ((s[i] == ',' || i == s.size() - 1) && stt != "")
			{
				tablename.push_back(stt);
				stt = "";
			}
		}
		vSelect.push_back(tablename);
	}
	else
		return "from语句后存在错误";

	for (auto t = temp.rbegin(); t != temp.rend(); t++)
		vSelect.push_back(*t);

	DataManage dataManage(vSelect, dbName);
	string str = dataManage.manage();
	return str;
}

string CmdParse::preSql(string s) //语句预处理
{
	const int count = s.size();
	string sql = "";
	bool flag = false; //标记
	int n = 0;
	for (int i = 0; i < count; i++)
		if (s[i] != ' ')
		{
			sql += s[i];
			n++;
			flag = true;
		}
		else if (s[i] == ' ' && flag)
		{
			sql += s[i];
			n++;
			flag = false;
		}

	if (sql[n - 1] == ' ')
		sql.erase(n - 1);
	return sql;
}

string CmdParse::preWhere(string s)
{
	int off = 0, len = s.size();
	for (int i = 0;; i++)
	{
		bool ismodify = false;
		if (s[i] == ' ' || s[i] == '(')
		{
			off = i + 1;
			len--;
			ismodify = true;
		}
		if (s[s.size() - 1 - i] == ';' || s[s.size() - 1 - i] == ')' || s[s.size() - 1 - i] == ' ')
		{
			len--;
			ismodify = true;
		}
		if (!ismodify || i == s.size() - 1 - i)
			break;
	}

	return s.substr(off, len);
}
