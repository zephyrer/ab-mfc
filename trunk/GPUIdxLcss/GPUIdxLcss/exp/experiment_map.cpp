#include <map>
#include <iostream>

using namespace std;

typedef map<int,int> IntMap;

void TestMap()
{
    IntMap m;
    
    m[1] = 1;
    m[2] = 2;
    m[3] = 3;
    m[4] = 4;

    for (IntMap::iterator iter = m.begin(); iter != m.end();++iter)
    {
        if( iter->first < 3 )
        {
            m.erase(iter);
            if (iter->first == 2) 
            {
                m.insert(pair<int,int>(1,9));// [8] = 9;
            }
        }
        else
        {
            break;
        }
    }

    for (IntMap::iterator iter = m.begin(); iter != m.end();++iter)
    {
        cout << iter->first << ' ';
    }
}

