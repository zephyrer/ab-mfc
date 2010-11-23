#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <lib/util.h>

using namespace std;
typedef list<int> IntList;

void TestDeleteNode()
{
    const int N = 3;
    const int data[N] ={1,2,3};
    IntList list1(data,data + N);
    
    for (IntList::iterator iter = list1.begin();iter != list1.end();)
    {
        if(*iter == 2) 
        {
            iter = list1.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
    for_each(list1.begin(),list1.end(),PrintInt);
    
}

void TestSplice()
{
    const int N = 3;
    const int data[N] ={1,2,3};
    IntList list1(data,data + N);
    IntList list2(list1);
    list1.splice(list1.begin(),list2,list2.begin(),list2.end());
    for_each(list1.begin(),list1.end(),PrintInt);
    printf("\n");
    for_each(list2.begin(),list2.end(),PrintInt);
    printf("\n");
}
// list中的merge只能用升序,原先的数据也必须是升序排列
void TestList()
{
	//TestDeleteNode();
    TestSplice();
}

