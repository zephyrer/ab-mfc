#include <setting/defines.h>
#include "strparser.h"  
#include <cstring>
#include <cmath>

using namespace SIE;





char StrParser::GotoNonSpace(char** pszMsg)
{
    while (*pszMsg && ::isspace(**pszMsg))
    {
        ++(*pszMsg);
    }
    return (*pszMsg) ? **pszMsg : '\0';
}

bool StrParser::Goto(char** pszMsg,const char c)
{    
    while (**pszMsg && (**pszMsg != c))
    {        
        ++(*pszMsg);
    }
    return (**pszMsg != '\0');
}


bool StrParser::Goto(char** pszMsg,const char * targetStr)
{
	char * pch = strstr(*pszMsg,targetStr);
	if(  pch!=NULL)
	{
		(*pszMsg) = pch;
		return true;
	}
	return false;
}


bool StrParser::GetInt(char** pszMsg, int &value)
{
	int iRes = 0;
	bool bIsMin = false;
	char *str = *pszMsg;
	bool haveInt = false;

	// walk to first non digit or minus sign
	while ((*str != '\0') && !::isdigit(*str) && (*str!='-'))
	{
		++str;                             
	}

	// if it was a minus sign, remember
	if ((*str != '\0') && (*str == '-'))       
	{
		bIsMin = true;
		++str;
	}

	// for all digits
	while ((*str != '\0') && (*str <= '9') && (*str >= '0'))   
	{
		// multiply old res with 10
		iRes = iRes*10 + (int)(*str-'0');               
		// and add new value
		++str;
		haveInt = true;
	}

	*pszMsg = str;

	if( haveInt )
	{
		value = (bIsMin) ? -iRes : iRes;
	}
	return  haveInt;
}


int StrParser::GetInt(char** pszMsg)
{
    int iRes = 0;
    bool bIsMin = false;
    char *str = *pszMsg;

    // walk to first non digit or minus sign
    while ((*str != '\0') && !::isdigit(*str) && (*str!='-'))
    {
        ++str;                             
    }

    // if it was a minus sign, remember
    if ((*str != '\0') && (*str == '-'))       
    {
        bIsMin = true;
        ++str;
    }

    // for all digits
    while ((*str != '\0') && (*str <= '9') && (*str >= '0'))   
    {
        // multiply old res with 10
        iRes = iRes*10 + (int)(*str-'0');               
        // and add new value
        ++str;                                        
    }
    *pszMsg = str;
    return (bIsMin) ? -iRes : iRes;
}

float StrParser::GetFloat(char** pszMsg)
{
    float dRes = 0.0f, dFrac = 1.0f;
    bool bIsMin = false, bInDecimal = false, bCont = true;
    char* str = *pszMsg;

    // go to first part of float(digit, minus sign or '.')
    while (*str!='\0' && !::isdigit(*str) && *str!='-' && *str!='.')
    {        
        ++str;
    }

    // if minus sign, remember that
    if (*str=='-')               
    {
        bIsMin=true;
        ++str;
    }

    // process the number bit by bit
    while (*str != '\0' && bCont)                
    {
        // in decimal part after '.'
        if (*str=='.')                            
        {
            bInDecimal = true;
        }
        else if (bInDecimal && *str<='9' && *str>='0') 
        {
            // number and in decimal
            dFrac = dFrac*10.0f;                    
            dRes += (float)(*str-'0')/dFrac;        
        }
        else if (*str<='9' && *str>='0')          
        {
            // if number and not decimal
            dRes = dRes*10.0f + (float)(*str-'0');         
        }
        else if (*str=='e')           
        {
            // if to power e, skip e			
            ++str;
            // determine sign
            if (*str == '+')                    
            {
                // skip +
                ++str;
                dRes *=  ::powf(10.0f, (float)GetInt(&str)); 
            }
            else if(*str == '-')
            {                
                dRes *=  ::powf(10.0f, (float)GetInt(&str)); 
            }
            bCont = false; 
        }
        else
        {
            // all other cases stop
            bCont = false;                           
        }

        if (bCont)      
        {
            ++str;
        }
    }

    *pszMsg = str;
    return (bIsMin && dRes != 0.0f) ? -dRes : dRes;
}

bool StrParser::GetFloat(char** pszMsg,float &value)
{
	float dRes = 0.0f, dFrac = 1.0f;
	bool bIsMin = false, bInDecimal = false, bCont = true,bHaveNum = false;
	char* str = *pszMsg;

	// go to first part of float(digit, minus sign or '.')
	while (*str!='\0' && !::isdigit(*str) && *str!='-' && *str!='.')
	{        
		++str;
	}

	// if minus sign, remember that
	if (*str=='-')               
	{
		bIsMin=true;
		++str;
	}

	// process the number bit by bit
	while (*str != '\0' && bCont)                
	{
		// in decimal part after '.'
		if (*str=='.')                            
		{
			bInDecimal = true;
		}
		else if (bInDecimal && *str<='9' && *str>='0') 
		{
			bHaveNum = true;
			// number and in decimal
			dFrac = dFrac*10.0f;                    
			dRes += (float)(*str-'0')/dFrac;        
		}
		else if (*str<='9' && *str>='0')          
		{
			bHaveNum = true;
			// if number and not decimal
			dRes = dRes*10.0f + (float)(*str-'0');         
		}
		else if (*str=='e')           
		{
			// if to power e, skip e			
			++str;
			// determine sign
			if (*str == '+')                    
			{
				// skip +
				++str;
				dRes *=  ::powf(10.0f, (float)GetInt(&str)); 
			}
			else if(*str == '-')
			{                
				dRes *=  ::powf(10.0f, (float)GetInt(&str)); 
			}
			bCont = false; 
		}
		else
		{
			// all other cases stop
			bCont = false;                           
		}

		if (bCont)      
		{
			++str;
		}
	}

	*pszMsg = str;
	if (bHaveNum)
	{
		value = (bIsMin && dRes != 0.0f) ? -dRes : dRes;
		return true;
	}
	return false;
}

char* StrParser::GetLine(char** pszMsg, char* pBuf, int nSize, char c)
{	
    char* str = *pszMsg;
    while (*str && (*str != c))
    {   		
        ++str;
    }

    int count = (int)(str-(*pszMsg));
    // avoid buffer overflow
    if (count > nSize-1) count = nSize-1;
    // leave a space to pad a null character
    ::strncpy(pBuf, *pszMsg, count);
    pBuf[count] = '\0';

    *pszMsg = str;	
    return pBuf;
}

char* StrParser::GetString(char** pszMsg, char* pBuf, int nSize)
{	
    char* str = *pszMsg;
    while (*str && !::isspace(*str))
    {   		
        ++str;
    }

    int count = (int)(str-(*pszMsg));
    // avoid buffer overflow
    if (count > nSize-1) count = nSize-1;
    // leave a space to pad a null character
    ::strncpy(pBuf, *pszMsg, count);
    pBuf[count] = '\0';

    *pszMsg = str;	
    return pBuf;
}

bool StrParser::StrCmp(const char* str1, const char* str2)
{
    while (*str1 == *str2)
    {
        if (*str1 == '\0') return true;
        ++str1;
        ++str2;
    }
    return false;
}

bool StrParser::StrCmpCI(const char* str1, const char* str2)
{
    while (::toupper(*str1) == ::toupper(*str2))
    {
        if (*str1 == '\0') return true;
        ++str1;
        ++str2;
    }
    return false;    
}

bool StrParser::StrCmpN(const char* str1, int len, const char* str2)
{
    if (len == 0) return true;
    if ((*str1 == '\0')||(*str2 == '\0')) return false;

    while (*str1 == *str2)
    {			
        if (--len == 0) return true;
        ++str1;
        ++str2;		
        if ((*str1 == '\0')||(*str2 == '\0')) return false;
    }

    return false;
}

bool StrParser::StrCmpNCI(const char* str1, int len, const char* str2)
{
    if (len == 0) return true;
    if ((*str1 == '\0')||(*str2 == '\0')) return false;

    while (::toupper(*str1) == ::toupper(*str2))
    {			
        if (--len == 0) return true;
        ++str1;
        ++str2;		
        if ((*str1 == '\0')||(*str2 == '\0')) return false;
    }

    return false;
}

char* StrParser::TrimRight(char* str)
{    
    for (int l = static_cast<int>(::strlen(str)-1); l >= 0; --l)
    {
        if (::isspace(str[l])) str[l] = '\0';
        else break;
    }

    return str;
}

bool StrParser::GetValue(char* str,const char *strSearch,int &value)
{

	char * stra = (char *)strstr(str,strSearch);
	
	if (stra != NULL)
	{
		stra += strlen(strSearch);
		value = GetInt(&stra);
		return true;
	}
	return false;

}

bool StrParser::GetValue(char** pszMsg,const char *strSearch,int &value)
{

	char *str = *pszMsg;
	char * stra = (char *)strstr(str,strSearch);

	if (stra != NULL)
	{
		stra += strlen(strSearch);
		value = GetInt(&stra);
		*pszMsg = stra;
		return true;
	}
	return false;

}

bool StrParser::GetValue(char* str,const char *strSearch,float &value)
{

	char * stra = (char *)strstr(str,strSearch);
	
	if (stra != NULL)
	{
		stra += strlen(strSearch);
		value = GetFloat(&stra);
		return true;
	}
	return false;

}

bool StrParser::GetValue(char**pszMsg,const char *strSearch,float &value)
{

	char *str = *pszMsg;
	char * stra = (char *)strstr(str,strSearch);

	if (stra != NULL)
	{
		stra += strlen(strSearch);
		value = GetFloat(&stra);
		*pszMsg = stra;
		return true;
	}
	return false;

}


bool StrParser::GetValue(char*str, const char *strSearch, char *value)
{

	char * stra = (char *)strstr(str,strSearch);
	char * strv;
	int count = 0;

	// find strSearch
	if (stra != NULL)
	{
		stra +=strlen(strSearch);
		while (*stra && isspace(*stra) ) 
		{   		
			++stra;
		}

		// find value
		strv = stra;

		while (*stra && !isspace(*stra) && *stra != ')') 
		{   		
			++stra;
			++count;
		}

		if (count >0)
		{
			strncpy(value, strv, count);
			value[count] = '\0';
		}
		return true;
	}
	return false;

}

bool StrParser::GetValue(char**pszMsg, const char *strSearch, char *value)
{

	char *str = *pszMsg;
	char * stra = (char *)strstr(str,strSearch);
	char * strv;
	int count = 0;

	// find strSearch
	if (stra != NULL)
	{
		stra +=strlen(strSearch);
		while (*stra && isspace(*stra) ) 
		{   		
			++stra;
		}

		// find value
		strv = stra;

		while (*stra && !isspace(*stra) && *stra != ')') 
		{   		
			++stra;
			++count;
		}

		if (count >0)
		{
			strncpy(value, strv, count);
			value[count] = '\0';
		}
		*pszMsg = stra;
		return true;
	}
	return false;

}

