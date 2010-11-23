#include <lib/browse_dir.h>
#include <lib/util.h>
#include <algorithm>
#include <iostream>

using namespace std;

int fileCount = 0;
static void ProcessFile(const char *fileName)
{
	++fileCount; 
	printf("%s\n",fileName);
}

void TestBrowseDir()
{
	CBrowseDir ss;
	

	const char * dir = "E:/Fund/LCSSIndexingCGB/FeatureData/"; 
	fileCount = 0;
	if( ss.SetInitDir(dir) )
	{
		ss.Browse("*.log",ProcessFile);
	}
	else
	{
		printf("Error:ss.SetInitDir(%s)\n",dir);
	}

	printf("(FileCount %d)\n",fileCount);
}


