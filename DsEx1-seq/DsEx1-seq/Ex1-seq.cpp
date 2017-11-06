/* 1652270 计算机2班 冯舜 */
#include <iostream>
#include <iomanip>
#include <stdexcept>
using namespace std;

#define ERROR_LIST_INVALID -2

const int defInitSize = 100;
const int increSize = 2;
enum CompareResult {CompareLess, CompareEqual, CompareGreater};

template <typename Elem>
class SqList
{
protected:
	int maxSize;
	int lastPos;
	Elem * data;
	void(*assign)(Elem &a, const Elem &b);
	CompareResult(*compare)(const Elem &a, const Elem &b);

	static void defaultAssign(Elem &a, const Elem &b)
	{
		a = b;
	}

	static CompareResult defaultCompare(const Elem &a, const Elem &b)
	{
		if (a < b)
			return CompareLess;
		if (a == b)
			return CompareEqual;

		return CompareGreater;
	}

public:
	SqList(int initSize = defInitSize)
	{
		//构造函数
		if (initSize < 0)
		{
			throw runtime_error("initSize should not be less than 0 in SqList initializing");
		}
		maxSize = initSize + increSize;

		data = new Elem[maxSize];
		lastPos = -1;
		assign = &defaultAssign;
		compare = &defaultCompare;
	}

	SqList(SqList<Elem>& L)
	{
		//复制构造
		Elem gottenVal;
		maxSize = L.maxSize;
		lastPos = L.lastPos;
		data = new Elem[maxSize];
		for (int i = 1; i <= lastPos + 1; i++)
		{
			L.getData(i, gottenVal);
			assign(data[i - 1], gottenVal);
		}
	}

	virtual ~SqList()
	{
		if (data == NULL)
		{
			//Very Strange
			return;
		}
		delete[] data;
	}

	virtual void setAssign(void(*newAssign)(Elem &a, const Elem &b))
	{
		this->assign = newAssign;
	}

	virtual void setCompare(CompareResult(*newCompare)(const Elem &a, const Elem &b))
	{
		this->compare = newCompare;
	}

	virtual int getMaxSize() const
	{
		return maxSize;
	}

	virtual int getLength() const
	{
		return lastPos + 1;
	}

	virtual int getSearchNo(const Elem & valueToBeSearched) const
	{
		//搜索 -2 for invalid seqlist, -1 for not found
		if (data == NULL)
			return ERROR_LIST_INVALID;
		for (int i = 0; i <= lastPos; i++)
		{
			if (compare(data[i], valueToBeSearched) == CompareEqual)
			{
				return i + 1;
			}
		}
		return -1;
	}

	virtual int locate(int i) const
	{
		//Locate i
		if (i >= 1 && i <= lastPos + 1)
			return i;
		return 0;
	}

	virtual bool getData(int i, Elem &x)
	{
		if (i >= 1 && i <= lastPos + 1)
		{
			assign(x, data[i - 1]);
			return true;
		}
		return false;
	}

	virtual bool insert(int i, const Elem &x)
	{
		if (i < 1 || i > lastPos + 2)
			return false;
		if (lastPos + 2 > maxSize)
		{
			//need to reallocate
			/*auto blocksNeeded = (lastPos + 2 - maxSize) / increSize + 1;
			auto newData = new Elem[maxSize + blocksNeeded * increSize];*/
			Elem * newData = new Elem[maxSize + increSize];
			maxSize = maxSize + increSize;
			Elem * p = data;
			Elem * pd = newData;
			Elem * placeToInsert = data + i - 1;

			for (; p < placeToInsert;)
				assign(*pd++, *p++);
			assign(*pd++, x);

			Elem * end = data + lastPos + 1;
			for (; p < end;)
				assign(*pd++, *p++);

			delete[]data;
			data = newData;
		}
		else
		{
			Elem * afterInsert = data + i - 1;
			Elem *afterP, *p;
			for (p = data + lastPos + 1; p > afterInsert; )
			{
				afterP = p--;
				assign(*afterP, *p);
			}
			assign(*p, x);
		}
		lastPos++;
		return true;
	}

	virtual bool insert(const Elem &x)
	{
		return insert(this->lastPos + 2, x);
	}

	virtual bool del(int i)
	{
		if (i < 1 || i > lastPos + 1)
			return false;
		Elem * endPos = data + lastPos;
		Elem * deletePlace = data + i - 1;
		for (Elem * p = deletePlace; p < endPos; p++)
		{
			assign(*p, *(p + 1));
		}

		lastPos--;
		return true;
	}

	virtual void traverse(void(*myVisit)(const Elem &, int index))
	{
		for (int i = 0; i <= lastPos; i++)
			myVisit(*(data + i), i + 1);
	}

	virtual void traverse(void(*myVisit)(Elem, int index))
	{
		for (int i = 0; i <= lastPos; i++)
			myVisit(*(data + i), i + 1);
	}
};

void printNumBeautiful(int i, int index)
{
	cout << setw(5) << i;
	if (index % 8 == 0)
		cout << endl;
	else
		cout << "  ";
}

void printNum(int i, int index)
{
	if (index != 1)
		cout << " ";
	cout << i;
}

int main()
{
	int initLength;
	cin >> initLength;
	SqList<int> l(initLength);

	int inputVal;
	for (int i = 0; i < initLength; i++)
	{
		cin >> inputVal;
		l.insert(inputVal);
	}
	
	l.traverse(printNum);
	
	cout << endl;

	int insertPos, insertVal;
	cin >> insertPos >> insertVal;
	bool insertResult;
	insertResult = l.insert(insertPos, insertVal);
	if (!insertResult)
		cout << -1;
	else
		l.traverse(printNum);

	cout << endl;

	int deletePos;
	cin >> deletePos;
	bool deleteResult;
	deleteResult = l.del(deletePos);
	if (!deleteResult)
		cout << -1;
	else
		l.traverse(printNum);

	cout << endl;

	int searchVal;
	cin >> searchVal;
	cout << l.getSearchNo(searchVal);

	cout << endl;
	return 0;
}