#include "CmdParse.h"

CmdParse::CmdParse(string s)
{
	sql = preSql(s);
}

string CmdParse::sqlCheck()
{
	regex Pcreate("^create table\\s\\w+\\s?\\(.+\\);$");
	regex Pdrop("^drop table\\s\\w+;$");
	regex Pinsert("^insert into\\s\\w+\\s?(\\(.+\\))?\\svalues\\s\\(.+\\);$");
	regex Pdelete("^delete from\\s\\w+\\swhere\\s.+;$");
	regex Pupdate("^update\\s\\w+\\sset(\\s\\w+=\\w+\\s?)(where.+)?;$");
	regex Pselect("^select.+from.+(where.+)?((group by.+)?|(having.+)?|(order by.+)?);$");


	if (regex_match(sql, Pcreate))
		return ForCreate();
	else if (regex_match(sql, Pdrop))
		return ForDrop();
	else if (regex_match(sql, Pinsert))
		return ForInsert();
	else if (regex_match(sql, Pdelete))
		return  ForDelete();
	else if (regex_match(sql, Pupdate))
		return  ForUpdate();
	else if (regex_search(sql, Pselect))
		return ForSelect();

	return "Sql语句错误";
}

string CmdParse::ForCreate()
{
	vector<vector<string>> vCreate;
	/*regex re1("(?<=create table ).+?(?=\\()");
	regex re2("(?<=\\().+(?=\\))");
	smatch csm1;
	smatch csm2;
	string cstr[2];*/
	int off1;
	if ((off1 = sql.find('(', 13)) != string::npos)
	{
		vector<string> create;
		string s = sql.substr(13, off1 - 13);
		cout << "create table:" << endl;
		cout << s << endl;
		cout << endl;
		create.push_back(s);
		vCreate.push_back(create);
	}
	else return "create table语句后存在错误";

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
			for (int i = 0; i < ss.length(); i++)
			{
				if (isname == true && ss[i] != ' ')
					name = name + ss[i];

				if (isname == true && name != "" && ss[i] == ' ')
				{
					isname = false;
					isatt = true;
					cout << name << endl;
					create.push_back(name);
				}
				if (isatt == true && ss[i] != ' ' && ss[i] != '(')
					attribute = attribute + ss[i];

				if (isatt == true && attribute != "" && ss[i] == '(')
				{
					isatt = false;
					iscap = true;
					cout << attribute << endl;
					create.push_back(attribute);
				}
				if (iscap == true && ss[i] != ' ' && ss[i] != '(' && ss[i] != ')')
					capacity = capacity + ss[i];

				if (iscap == true && ss[i] == ')' && capacity != "")
				{
					iscap = false;
					cout << capacity << endl;
					create.push_back(capacity);
				}
			}
			vCreate.push_back(create);
			cout << endl;
			st = rsm[0].second;
		}
	}
	else return "括号中语句存在错误";

	result = vCreate;
	return "Create table成功";
}

string CmdParse::ForDrop()
{
	vector<vector<string>> vDrop;
	/*regex dre("(?<=drop table )\\w+(?=;)");
	smatch dsm;*/

	cout << "drop table:" << endl;
	int off;
	if ((off = sql.rfind(';') != string::npos))
	{
		vector<string> name;
		string s = sql.substr(11, sql.length() - 12);
		cout << s << endl;
		name.push_back(s);
		vDrop.push_back(name);
	}
	else return "drop语句后存在错误";

	result = vDrop;
	return "Drop table成功";
}

string CmdParse::ForInsert()
{
	vector<vector<string>> vInsert;
	/*regex re1("(?<=insert into )\.+(?=values)");
	regex re2("(?<=values\\()\.+(?=\\);)");
	string istr[2];
	smatch ism[2];*/
	int off1;
	if ((off1 = sql.find("values (", 12)) != string::npos)
	{
		vector<string> insert;
		regex rr("(?<=\\().+(?=\\))");
		smatch rsm;
		string s = sql.substr(12, off1 - 12);

		cout << "Insert into:" << endl;
		if (regex_search(s, rsm, rr))
		{
			regex rr2(".+?(?=\\()");
			smatch rsm2;
			regex_search(s, rsm2, rr2);
			cout << rsm2.str() << endl;
			cout << endl;
			insert.push_back(rsm2.str());

			string rstr = rsm.str();
			smatch rsm1;
			regex rr1("(?<=').+?(?=')");
			string::const_iterator st = rstr.begin();
			string::const_iterator en = rstr.end();

			while (regex_search(st, en, rsm1, rr1))
			{
				if (rsm1.str() != ",")
				{
					cout << rsm1.str() << endl;
					insert.push_back(rsm1.str());
				}
				st = rsm1[0].second;
			}
		}
		else
		{
			insert.push_back(s);
			cout << s << endl;
		}
		vInsert.push_back(insert);
		cout << endl;
	}
	else return "insert into语句后存在错误";

	int off2;
	if ((off2 = sql.rfind(");")) != string::npos)
	{
		vector<string> insert;
		smatch rsm3;
		regex rr3("(?<=').+?(?=')");
		string s = sql.substr(off1 + 1, off2 - off1 - 1);
		string::const_iterator st = s.begin();
		string::const_iterator en = s.end();

		cout << "Values:" << endl;
		while (regex_search(st, en, rsm3, rr3))
		{
			if (rsm3.str() != ",")
			{
				cout << rsm3.str() << endl;
				insert.push_back(rsm3.str());
			}
			st = rsm3[0].second;
		}
		vInsert.push_back(insert);
	}
	else return "values语句后存在错误";

	result = vInsert;
	return "Insert数据成功";
}


string CmdParse::ForDelete()
{
	vector<vector<string>> vDelete;
	/*regex re1("(?<=delete from )\.+(?=where )");
	regex re2("(?<=where )\.+(?=;)");
	string dstr[2];
	smatch dsm[2];
	string::const_iterator st = sql.begin();
	string::const_iterator en = sql.end();*/
	int off1;
	if ((off1 = sql.find("where ", 12)) != string::npos)
	{
		vector<string> vdelete;
		string s = sql.substr(12, off1 - 12);
		vdelete.push_back(s);
		cout << "delete from:" << endl;
		cout << s << endl;
		cout << endl;
		vDelete.push_back(vdelete);
	}
	else return "delete from与where之间存在错误";

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

		cout << "where:" << endl;
		while (regex_search(st, en, rrsm, rr))
		{
			flag = true;
			cout << rrsm.str() << endl;
			vdelete.push_back(rrsm.str());
			st = rrsm[0].second;
		}
		if (flag)
		{
			regex_search(st, en, rrsm, regex("\.+"));
			cout << rrsm.str() << endl;
			vdelete.push_back(rrsm.str());
		}
		else
		{
			cout << s << endl;
			vdelete.push_back(s);
		}
		vDelete.push_back(vdelete);
	}
	else return "where语句后存在错误";

	result = vDelete;
	return "Delete 数据成功";
}

string CmdParse::ForUpdate() {
	vector<vector<string>> vUpdate;
	string str[3];
	smatch sm[3];
	regex re1("(?<=update )\.+(?= set)");
	regex re2("((?<=set )\.+(?= where))|((?<=set )\.+(?=END))");
	regex re3("(?<=where )\.+(?=END)");

	if (regex_search(sql, sm[0], re1)) {
		vector<string> update;
		str[0] = sm[0].str();
		update.push_back(str[0]);
		cout << "update:" << endl;
		cout << str[0] << endl;
		cout << endl;
		vUpdate.push_back(update);
	}
	else return "update与set语句间存在错误";

	if (regex_search(sql, sm[1], re2)) {
		vector<string> update;
		str[1] = sm[1].str();
		update.push_back(str[1]);
		cout << "set:" << endl;
		cout << str[1] << endl;
		cout << endl;
		vUpdate.push_back(update);
	}
	else return "set字段存在错误";

	if (regex_search(sql, sm[2], re3)) {
		vector<string> update;
		regex rr("(\.+?and )|(\.+?or )");
		smatch rrsm;
		str[2] = sm[2].str();
		string::const_iterator st = str[2].begin();
		string::const_iterator en = str[2].end();
		bool flag = false;

		cout << "where:" << endl;
		while (regex_search(st, en, rrsm, rr)) {
			flag = true;
			cout << rrsm.str() << endl;
			update.push_back(rrsm.str());
			st = rrsm[0].second;
		}
		if (flag) {
			regex_search(st, en, rrsm, regex("\.+"));
			cout << rrsm.str() << endl;
			update.push_back(rrsm.str());
		}
		else {
			cout << str[2] << endl;
			update.push_back(str[2]);
		}
		vUpdate.push_back(update);
	}

	result = vUpdate;
	return "Update 数据成功";
}

string CmdParse::ForSelect() {
	vector<vector<string>> vSelect;
	regex reg1("(?<=select )\.+?(?= from)");
	regex reg2("((?<=from )\.+?(?= where))|((?<=from )\.+?(?=END))");
	regex reg3("(?<=where )\.+?(?=END)");
	smatch ssm[3];

	if (regex_search(sql, ssm[0], reg1)) {
		cout << "select:" << endl;
		vector<string> attribute;
		string stt = "";
		string ss = ssm[0].str();
		for (int i = 0; i < ss.size(); i++) {
			if (ss[i] != ' ' && ss[i] != ',') {
				stt = stt + ss[i];
			}
			if ((ss[i] == ',' || i == ss.size() - 1) && stt != "") {
				cout << stt << endl;
				attribute.push_back(stt);
				stt = "";
			}
		}
		vSelect.push_back(attribute);
	}
	else return "select from语句后存在错误";

	if (regex_search(sql, ssm[1], reg2)) {
		cout << "from:" << endl;
		vector<string> tablename;
		string stt = "";
		string ss = ssm[1].str();
		for (int i = 0; i < ss.size(); i++) {
			if (ss[i] != ' ' && ss[i] != ',') {
				stt = stt + ss[i];
			}
			if ((ss[i] == ',' || i == ss.size() - 1) && stt != "") {
				cout << stt << endl;
				tablename.push_back(stt);
				stt = "";
			}
		}
		vSelect.push_back(tablename);
	}
	else return "from语句后存在错误";

	if (regex_search(sql, ssm[2], reg3)) {
		vector<string> condition;
		regex sr("(\.+?and )|(\.+?or )");
		smatch srsm;
		string sst = ssm[2].str();
		string::const_iterator st = sst.begin();
		string::const_iterator en = sst.end();
		bool flag = false;

		cout << "where:" << endl;
		while (regex_search(st, en, srsm, sr)) {
			flag = true;
			cout << srsm.str() << endl;
			condition.push_back(srsm.str());
			st = srsm[0].second;
		}
		if (flag) {
			regex_search(st, en, srsm, regex("\.+"));
			cout << srsm.str() << endl;
			condition.push_back(srsm.str());
		}
		else {
			cout << sst << endl;
			condition.push_back(sst);
		}
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
