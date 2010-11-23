#include <setting/config.h>
#include <setting/defines.h>
#include <iostream>
#include <indexing/lcss.h>

using namespace std;
using namespace SIE;

typedef vector<int> IntVector;

void TestLCSS()
{
	//typedef vector <int> VecInt;

	//Print("(test %d %d)",10,20);
	//char c[3]={'1','2','3'};

	//c[1] =0;
	//Print("%s\n",c);

	IntVector ivec1;
	IntVector ivec2;

	ivec1.push_back(2);
	ivec1.push_back(8);
	//ivec1.push_back(3);
	ivec1.push_back(1);
	ivec1.push_back(2);
	ivec1.push_back(1);

	ivec2.push_back(1);
	ivec2.push_back(2);
	//ivec2.push_back(3);
	ivec2.push_back(9); 
	ivec2.push_back(4);
	ivec2.push_back(1);
	ivec2.push_back(2);
	for (IntVector::iterator it1=ivec1.begin();it1!=ivec1.end();it1++)
	{cout<<*it1<<" ";
	}
	cout<<endl;
	for (IntVector::iterator it2=ivec2.begin();it2!=ivec2.end();it2++)
	{cout<<*it2<<" ";
	}
	cout<<endl;
	cout<<LCSS(ivec1,ivec2)<<endl;
	cout<<LCSSDelta(ivec1,ivec2,0);
}

