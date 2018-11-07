#include "CmdParse.h"



CmdParse::CmdParse()
{
}


CmdParse::~CmdParse()
{
}

string CmdParse::sqlCheck(string sql)
{
	regex Pupdate("update\.+set\.+(where\.+)?END");
	regex Pdelete("delete\.+from\.+where\.+END");
	regex Pinsert("insert\.+into\.+(\(\.+/)\.*)?values\(.+\)END");
	regex Pcreate("create table\.+\\(\.+\\)END");
	regex Pselect("select\.+from\.+(where\.+)?((group by)?|(order by)?|(having)?)END");
	regex Pdrop("drop table\.+END");

	if (regex_match(sql, Pupdate))
		return  ForUpdate();
	else if (regex_match(sql, Pdelete))
		return  ForDelete();
	else if (regex_match(sql, Pinsert))
		return ForInsert();
	else if (regex_match(sql, Pcreate))
		return ForCreate();
	else if (regex_search(sql, Pselect))
		return ForSelect();
	else if (regex_match(sql, Pdrop))
		return ForDrop();

	return "Sql”Ôæ‰¥ÌŒÛ";
}
