#include <vector>
#include <list>

using namespace std;

typedef list<int> ListSet;
typedef vector<int> VectorSet;

//集合运算，不完全交集
class Intersection
{
public:
	Intersection();
	~Intersection();
	void Reset()
	{
		mListSet.clear();
		mResultList.clear();
	}
	//归并集合，mListSet和s均为升序
	void MergeSet(ListSet &s)
	{
		mListSet.merge(s);
	}

	// 添加集合
	void PushSet(ListSet &s)
	{
		mListSet.merge(s);
		mListSet.insert(mListSet.end(),s.begin(),s.end());
	}

	// 根据h更新结果集
	// h为元素的最少出现次数
	void UpdateResultBySort(int h);

	void UpdateResultByMerge(int h);

	void OutputResult();
private:
	const Intersection &operator = (const Intersection &);
	Intersection(const Intersection &);
public:
	ListSet mListSet;
	ListSet mResultList;
};

