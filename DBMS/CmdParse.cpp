#include "CmdParse.h"

CmdParse::CmdParse(string s)
{
	sql = preSql(s);
}

string CmdParse::sqlCheck()
{
	regex dCreate("^create database \\w+;$");
	regex dDrop("^drop database \\w+;$");
	regex tCreate("^create table \\w+\\s?\\(.+\\);$");
	regex tAlter("^alter table \\w+\\s(add|drop)\\s;$");
	regex tDrop("^drop table \\w+;$");
	regex tInsert("^insert into \\w+\\s?(\\(.+\\))?\\svalues\\s\\(.+\\);$");
	regex tDelete("^delete from \\w+\\swhere\\s.+;$");
	regex tUpdate("^update \\w+\\sset(\\s\\w+=\\w+\\s?,)*(\\s\\w+=\\w+\\s?)(where.+)?;$");
	regex tSelect("^select.+from.+(where.+)?((group by.+)?|(having.+)?|(order by.+)?);$");

	if (regex_match(sql, dCreate))
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

	return "Sql语句错误";
}

string CmdParse::dbCreate()
{
	vector<vector<string>> vCreate;

	int off;
	if ((off = sql.rfind(';') != string::npos))
	{
		vector<string> name;
		name.push_back("create");
		string s = sql.substr(16, sql.size() - 17);
		name.push_back(s);
		vCreate.push_back(name);
	}
	else
		return "create语句后存在错误";

	result = vCreate;
	return "Create database成功";
}

string CmdParse::dbDrop()
{
	vector<vector<string>> vDrop;

	int off;
	if ((off = sql.rfind(';') != string::npos))
	{
		vector<string> name;
		name.push_back("drop");
		string s = sql.substr(14, sql.size() - 15);
		name.push_back(s);
		vDrop.push_back(name);
	}
	else
		return "drop语句后存在错误";

	result = vDrop;
	return "Drop database成功";
}

string CmdParse::tableCreate()
{
	vector<vector<string>> vCreate;
	/*regex re1("(?<=create table ).+?(?=\\()");
	regex re2("(?<=\\().+(?=\\))");
	smatch csm1;
	smatch csm2;
	string cstr[2];*/
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
		string s = sql.substr(off1 + 1, off2 - off1 - 1);
		string::const_iterator st = s.begin();
		string::const_iterator en = s.end();

		while (regex_search(st, en, rsm, rer))
		{
			vector<string> create;
			string ss = rsm.str();
			bool isname = true;
			bool isatt = false;
			bool iscap = false;
			string name = "";
			string attribute = "";
			string capacity = "";
			for (int i = 0; i < ss.size(); i++)
			{
				if (isname == true && ss[i] != ' ')
					name = name + ss[i];

				if (isname == true && name != "" && ss[i] == ' ')
				{
					isname = false;
					isatt = true;
					create.push_back(name);
				}
				if (isatt == true && ss[i] != ' ' && ss[i] != '(' && ss[i] != ',')
					attribute = attribute + ss[i];

				if (isatt == true && attribute != "" && (ss[i] == '(' || ss[i] == ','))
				{
					isatt = false;
					iscap = true;
					create.push_back(attribute);
				}
				if (iscap == true && ss[i] != ' ' && ss[i] != '(' && ss[i] != ')')
					capacity = capacity + ss[i];

				if (iscap == true && ss[i] == ')' && capacity != "")
				{
					iscap = false;
					create.push_back(capacity);
				}
			}
			vCreate.push_back(create);
			st = rsm[0].second;
		}
	}
	else
		return "括号中语句存在错误";

	TableManage tableManage(vCreate);
	return "Create table成功";
}

string CmdParse::tableAlter()
{
	return string();
}

string CmdParse::tableDrop()
{
	vector<vector<string>> vDrop;
	/*regex dre("(?<=drop table )\\w+(?=;)");
	smatch dsm;*/

	int off;
	if ((off = sql.rfind(';') != string::npos))
	{
		vector<string> name;
		name.push_back("drop");
		string s = sql.substr(11, sql.size() - 12);
		name.push_back(s);
		vDrop.push_back(name);
	}
	else
		return "drop语句后存在错误";

	result = vDrop;
	return "Drop table成功";
}

string CmdParse::tableInsert()
{
	vector<vector<string>> vInsert;
	/*regex re1("(?<=insert into )\.+(?=values)");
	regex re2("(?<=values\\()\.+(?=\\);)");
	string istr[2];
	smatch ism[2];*/
	int off1;
	if ((off1 = sql.find(" values (")) != string::npos)
	{
		vector<string> insert;
		insert.push_back("insert");
		//regex rr("(?<=\\().+(?=\\))");
		smatch rsm;
		string s = sql.substr(12, off1 - 12);

		int off2;
		if ((off2 = s.find("(")) != string::npos)
		{
			regex rr2(".+?(?=\\()");
			smatch rsm2;
			regex_search(s, rsm2, rr2);
			insert.push_back(rsm2.str());

			string ss = s.substr(off2 + 1, s.size() - off2 - 2);
			smatch rsm1;
			/*regex rr1("(?<=').+?(?=')");
			string::const_iterator st = s.begin();
			string::const_iterator en = s.end();

			while (regex_search(st, en, rsm1, rr1))
			{
				if (rsm1.str() != ",")
					insert.push_back(rsm1.str());
				st = rsm1[0].second;
			}*/
			string stt = "";
			for (int i = 0; i < ss.size(); i++)
			{
				if (ss[i] != ' ' && ss[i] != ',')
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
	if ((off2 = sql.rfind(");")) != string::npos)
	{
		vector<string> insert;
		string s = sql.substr(off1 + 9, off2 - off1 - 9);
		/*smatch rsm3;
		regex rr3("(?<=').+?(?=')");
		string::const_iterator st = s.begin();
		string::const_iterator en = s.end();

		while (regex_search(st, en, rsm3, rr3))
		{
			if (rsm3.str() != ",")
				insert.push_back(rsm3.str());
			st = rsm3[0].second;
		}*/
		string stt = "";
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] != ' ' && s[i] != ',')
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

	result = vInsert;
	return "Insert数据成功";
}


string CmdParse::tableDelete()
{
	vector<vector<string>> vDelete;
	/*regex re1("(?<=delete from )\.+(?=where )");
	regex re2("(?<=where )\.+(?=;)");
	string dstr[2];
	smatch dsm[2];
	string::const_iterator st = sql.begin();
	string::const_iterator en = sql.end();*/
	int off1;
	if ((off1 = sql.find("where ")) != string::npos)
	{
		vector<string> vdelete;
		vdelete.push_back("delete");
		string s = sql.substr(12, off1 - 12);
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
		string s = sql.substr(off1 + 6, off2 - off1);
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

	result = vDelete;
	return "Delete 数据成功";
}

string CmdParse::tableUpdate()
{
	vector<vector<string>> vUpdate;
	/*string str[3];
	smatch sm[3];
	regex re1("(?<=update )\.+(?= set)");
	regex re2("((?<=set )\.+(?= where))|((?<=set )\.+(?=;))");
	regex re3("(?<=where )\.+(?=;)");*/
	int off1;
	if ((off1 = sql.find(" set")) != string::npos)
	{
		vector<string> update;
		update.push_back("update");
		string s = sql.substr(7, off1 - 7);
		update.push_back(s);
		vUpdate.push_back(update);
	}
	else
		return "update与set语句间存在错误";

	int off2;
	if ((off2 = sql.rfind(" where")) != string::npos || (off2 = sql.rfind(";")) != string::npos)
	{
		vector<string> update;
		string s = sql.substr(off1 + 5, off2 - off1 - 5);
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
	if (sql.rfind(" where") != string::npos && (off3 = sql.rfind(";")) != string::npos)
	{
		vector<string> update;
		regex rr("(.+?and )|(.+?or )");
		smatch rrsm;
		string s = sql.substr(off2 + 7, off3 - off2 - 7);
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

	result = vUpdate;
	return "Update 数据成功";
}

string CmdParse::tableSelect()
{
	vector<vector<string>> vSelect;
	/*regex reg1("(?<=select )\.+?(?= from)");
	regex reg2("((?<=from )\.+?(?= where))|((?<=from )\.+?(?=;))");
	regex reg3("(?<=where )\.+?(?=;)");
	smatch ssm[3];*/
	int off1;
	if ((off1 = sql.find(" from")) != string::npos)
	{
		vector<string> attribute;
		attribute.push_back("select");
		string stt = "";
		string s = sql.substr(7, off1 - 7);
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

	int off2;
	if ((off2 = sql.rfind(" where")) != string::npos || (off2 = sql.rfind(";")) != string::npos)
	{
		vector<string> tablename;
		string stt = "";
		string s = sql.substr(off1 + 11, off2 - off1 - 11);
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

	int off3;
	if (sql.rfind(" where") != string::npos && (off3 = sql.rfind(";")) != string::npos)
	{
		vector<string> condition;
		regex sr("(.+?and )|(.+?or )");
		smatch srsm;
		string s = sql.substr(off2 + 7, off3 - off2 - 7);
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

		vSelect.push_back(condition);
	}
	result = vSelect;
	return "Select数据成功";
}

string CmdParse::preSql(string s) //语句预处理
{
	const int count = s.size();
	string sql = "";
	int flag = 0; //标记
	int n = 0;
	for (int i = 0; i < count; i++)
		if (s[i] != ' ')
		{
			sql += s[i];
			n++;
			flag = 1;
		}
		else if (s[i] == ' ' && flag == 1)
		{
			sql += s[i];
			n++;
			flag = 0;
		}

	if (sql[n - 1] == ' ')
		sql.erase(n - 1, 1);
	return sql;
}
