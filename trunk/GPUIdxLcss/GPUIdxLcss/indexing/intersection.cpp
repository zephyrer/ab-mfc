#include "intersection.h"
#include <iostream>

Intersection::Intersection()
{

}

Intersection::~Intersection()
{

}


void Intersection::UpdateResultBySort(int h)
{
	mListSet.sort();
	mResultList.clear();
	int last = mListSet.front();
	int count = 0;
 	for (ListSet::const_iterator it = mListSet.begin(); it != mListSet.end(); ++it)
 	{
		if(*it == last)
		{
			++count;			
		}
		else
		{
			if(count >=h )	mResultList.push_back(last);
			count = 1;
			last = *it;
		}
 	}
	if(count >=h )	mResultList.push_back(last);
}


void Intersection::UpdateResultByMerge(int h)
{
	mResultList.clear();
	int last = mListSet.front();
	int count = 0;
	for (ListSet::const_iterator it = mListSet.begin(); it != mListSet.end(); ++it)
	{
		if(*it == last)
		{
			++count;			
		}
		else
		{
			if(count >=h )	mResultList.push_back(last);
			count = 1;
			last = *it;
		}
	}
	if(count >=h )	mResultList.push_back(last);

}


void Intersection::OutputResult()
{
	for (ListSet::const_iterator it = mResultList.begin(); it != mResultList.end(); ++it)
	{
		cout << *it<<" ";
	}
}

