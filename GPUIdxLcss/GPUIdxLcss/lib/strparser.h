#ifndef _TEXT_STRPARSER_H_
#define _TEXT_STRPARSER_H_


#include <ctype.h>  

namespace SIE
{
class StrParser
{
public:    
    //! extract the first int from the message, and advance the pointer
    static int GetInt(char** pszMsg);

	static bool GetInt(char** pszMsg, int &value);
    
	//! extract the first float from the message, and advance the pointer
    static float GetFloat(char** pszMsg);
	static bool GetFloat(char** pszMsg,float &value);

    //! extract the first string from the message (read ahead until space), and advance the pointer
    static char* GetString(char** pszMsg, char* pBuf, int nSize);
    //! extract the string (including space) before a spefied character from the message, and advance the pointer to the position of the character
    static char* GetLine(char** pszMsg, char* pBuf, int nSize, char c = '\0');

    //! move the pointer to the first occurence of the specified character
    static bool Goto(char** pszMsg ,const char c );

	//! 
	static bool Goto(char** pszMsg,const char * targetStr);

    //! move the pointer to the non-space character
    static char GotoNonSpace(char** pszMsg);
    //! fast null-terminated string compare routine  
    /*!
    \param str1 A null-terminated string.
    \param str2 A null-terminated string.
    \return Returns true if the contents of two strings are totally identical, otherwise returns false. <br/>
    */
    static bool StrCmp(const char* str1, const char* str2);
    //! fast null-terminated string case-insensitive compare routine    
    static bool StrCmpCI(const char* str1, const char* str2);
    //! fast string compare routine with fixed-length    
    /*!
    \param str1 A null-terminated string.
    \param len The length of string to compare.
    \param str2 A null-terminated string.
    \return Returns true if the first "len" characters of two strings are equal, otherwise returns false. <br/>
    */
    static bool StrCmpN(const char* str1, int len, const char* str2);
    static bool StrCmpNCI(const char* str1, int len, const char* str2);
    //! Deletes the trailing space.
    static char* TrimRight(char* str);

	// 查找对应的整数值，以空格或')'结束,
	// 如char * str= "(id 10) ",int nId=0;则 GetValue(str,"id",nId),之后nId变为10，且函数返回true
	static bool GetValue(char* str,const char *strSearch,int &value);
	static bool GetValue(char** pszMsg,const char *strSearch,int &value);
	
	static bool GetValue(char*str, const char *strSearch, char *value);
	static bool GetValue(char** pszMsg, const char *strSearch, char *value);

	static bool GetValue(char*str, const char *strSearch, float &value);
	static bool GetValue(char** pszMsg, const char *strSearch, float &value);

};

};


#endif //_TEXT_STRPARSER_H_

