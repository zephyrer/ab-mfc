#include <vector>
#include <list>

using namespace std;

typedef list<int> ListSet;
typedef vector<int> VectorSet;

//�������㣬����ȫ����
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
	//�鲢���ϣ�mListSet��s��Ϊ����
	void MergeSet(ListSet &s)
	{
		mListSet.merge(s);
	}

	// ��Ӽ���
	void PushSet(ListSet &s)
	{
		mListSet.merge(s);
		mListSet.insert(mListSet.end(),s.begin(),s.end());
	}

	// ����h���½����
	// hΪԪ�ص����ٳ��ִ���
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

