#include "config.h"
#include <fstream>
#include <string>
#include <lib/strparser.h>
#include <setting/defines.h>

using namespace SIE;

const char * Config::strUserFileName = "user.cfg";
int Config::iUserID;
const char * Config::strUserName[UI_MAX] =
{
	"Anonymous",
	"CuiGuangbin",
	"GaoChengkai",
	"ZhengHu"
};


void Config::Init()
{
	std::ifstream fin( strUserFileName );
	std::string strTmp;
	const int MAX_BUF = 1024;
	char buf[MAX_BUF];
	char * pch;
	AssertLog(fin,"Error: Could'nt open file(%s)!",strUserFileName);
	while ( fin.getline(buf,MAX_BUF,'\n'))
	{
		pch = buf;
		if( *pch == '#') continue;
		if( *pch == '(')
		{
			bool ok = StrParser::GetValue(&pch,"USER_ID",iUserID);
			AssertLog( 0<= iUserID && iUserID < UI_MAX ,"Error:(UserID %d)",iUserID);
			Print("(UserName %s)",strUserName[iUserID]);
			AssertLog(ok,"Error: ");
			break;
		}
	}
}

