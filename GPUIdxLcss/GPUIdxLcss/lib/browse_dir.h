#ifndef BROWSE_DIR_H
#define BROWSE_DIR_H

#include "stdlib.h" 


#pragma warning(disable:4996) //ȫ���ص� 


typedef void (*FunctionPointor)(const char* filename) ; // ����CallBack ���͵ĺ���ָ��

class CBrowseDir 
{ 
public:
	//ȱʡ������ 
	CBrowseDir(); 

	//���ó�ʼĿ¼Ϊdir���������false����ʾĿ¼������ 
	bool SetInitDir(const char* dir); 

	//������ʼĿ¼������Ŀ¼����filespecָ�����͵��ļ� 
	//filespec����ʹ��ͨ��� * ?�����ܰ���·���� 
	void Browse(const char* filespec,FunctionPointor fun); 
private: 
	//����Ŀ¼dir����filespecָ�����ļ� 
	//������Ŀ¼,���õ����ķ��� 
	void BrowseDir(const char* dir,const char* filespec); 
private:
	FunctionPointor mFun;
	//��ų�ʼĿ¼�ľ���·������'\'��β 
	char m_szInitDir[_MAX_PATH]; 
}; 

#endif