#ifndef BROWSE_DIR_H
#define BROWSE_DIR_H

#include "stdlib.h" 


#pragma warning(disable:4996) //全部关掉 


typedef void (*FunctionPointor)(const char* filename) ; // 声明CallBack 类型的函数指针

class CBrowseDir 
{ 
public:
	//缺省构造器 
	CBrowseDir(); 

	//设置初始目录为dir，如果返回false，表示目录不可用 
	bool SetInitDir(const char* dir); 

	//遍历初始目录及其子目录下由filespec指定类型的文件 
	//filespec可以使用通配符 * ?，不能包含路径。 
	void Browse(const char* filespec,FunctionPointor fun); 
private: 
	//遍历目录dir下由filespec指定的文件 
	//对于子目录,采用迭代的方法 
	void BrowseDir(const char* dir,const char* filespec); 
private:
	FunctionPointor mFun;
	//存放初始目录的绝对路径，以'\'结尾 
	char m_szInitDir[_MAX_PATH]; 
}; 

#endif