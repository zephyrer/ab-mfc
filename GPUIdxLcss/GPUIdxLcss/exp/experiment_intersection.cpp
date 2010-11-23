#include "experiment.h"
#include <indexing/intersection.h>
#include <iostream>
#include <algorithm>
#include <lib/util.h>

void TestIntersection()
{
	Intersection intersec;
	ListSet set1,set2,set3;
	set1.push_back(1);
	set1.push_back(2);
	set1.push_back(3);
	
	set2.push_back(1);
	set2.push_back(2);
	set2.push_back(4);

	set3.push_back(-1);
	set3.push_back(2);
	set3.push_back(6);

	for_each(set1.begin(),set1.end(),PrintInt);

	intersec.MergeSet(set1);
	intersec.MergeSet(set2);
	intersec.MergeSet(set3);

	intersec.UpdateResultByMerge(3);
	intersec.OutputResult();

	for_each(set1.begin(),set1.end(),PrintInt);


// 	intersec.Reset();
// 
// 	intersec.MergeSet(set1);
// 	intersec.MergeSet(set2);
// 	intersec.MergeSet(set3);
// 
// 	intersec.UpdateResultByMerge(3);
// 	intersec.OutputResult();
}

