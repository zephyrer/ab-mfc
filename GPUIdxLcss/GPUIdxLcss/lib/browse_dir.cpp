#include "browse_dir.h" 
#include "stdlib.h" 
#include "direct.h" 
#include "string.h" 
#include "io.h" 



CBrowseDir::CBrowseDir() 
{ 
	//�õ�ǰĿ¼��ʼ��m_szInitDir 
	getcwd(m_szInitDir,_MAX_PATH); 

	//���Ŀ¼�����һ����ĸ����'\',����������һ��'\' 
	size_t  len=strlen(m_szInitDir); 
	if (m_szInitDir[len-1] != '\\') 
		strcat(m_szInitDir,"\\"); 
} 

bool CBrowseDir::SetInitDir(const char *dir) 
{ 
	//�Ȱ�dirת��Ϊ����·�� 
	if (_fullpath(m_szInitDir,dir,_MAX_PATH) == NULL) 
		return false; 

	//�ж�Ŀ¼�Ƿ���� 
	if (_chdir(m_szInitDir) != 0) 
		return false; 

	//���Ŀ¼�����һ����ĸ����'\',����������һ��'\' 
	size_t len=strlen(m_szInitDir); 
	if (m_szInitDir[len-1] != '\\') 
		strcat(m_szInitDir,"\\"); 

	return true; 
} 

void CBrowseDir::Browse(const char *filespec,FunctionPointor fun) 
{ 
	mFun = fun;
	BrowseDir(m_szInitDir,filespec); 
} 

void CBrowseDir::BrowseDir(const char *dir,const char *filespec) 
{ 
	_chdir(dir); 

	//���Ȳ���dir�з���Ҫ����ļ� 
	//long hFile; 
    intptr_t hFile;
	_finddata_t fileinfo; 
	if ((hFile=_findfirst(filespec,&fileinfo)) != -1) 
	{ 
		do 
		{ 
			//����ǲ���Ŀ¼ 
			//�������,����д��� 
			if (!(fileinfo.attrib & _A_SUBDIR)) 
			{ 
				char filename[_MAX_PATH]; 
				strcpy(filename,dir); 
				strcat(filename,fileinfo.name); 
				mFun(filename);
			} 
		} while (_findnext(hFile,&fileinfo) == 0); 
		_findclose(hFile); 
	} 

	//����dir�е���Ŀ¼ 
	//��Ϊ�ڴ���dir�е��ļ�ʱ���������ProcessFile�п��ܸı��� 
	//��ǰĿ¼����˻�Ҫ�������õ�ǰĿ¼Ϊdir�� 
	//ִ�й�_findfirst�󣬿���ϵͳ��¼���������Ϣ����˸ı�Ŀ¼ 
	//��_findnextû��Ӱ�졣 
	_chdir(dir); 
	if ((hFile=_findfirst("*.*",&fileinfo)) != -1) 
	{ 
		do 
		{ 
			//����ǲ���Ŀ¼ 
			//�����,�ټ���ǲ��� . �� .. 
			//�������,���е��� 
			if ((fileinfo.attrib & _A_SUBDIR)) 
			{ 
				if (strcmp(fileinfo.name,".") != 0 && strcmp 
					(fileinfo.name,"..") != 0) 
				{ 
					char subdir[_MAX_PATH]; 
					strcpy(subdir,dir); 
					strcat(subdir,fileinfo.name); 
					strcat(subdir,"\\"); 
					 
				} 
			} 
		} while (_findnext(hFile,&fileinfo) == 0);
		_findclose(hFile); 
	} 
} 
