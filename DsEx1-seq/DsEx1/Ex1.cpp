/* 1652270 计算机2班 冯舜 */
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <cstring>
using namespace std;

#define ERROR_LIST_INVALID -2

template <typename Elem>
class SqList
{
protected:
	static const int defInitSize = 100;
	static const int increSize = 10;
	enum CompareResult { CompareLess, CompareEqual, CompareGreater };

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

	SqList(const SqList<Elem>& L, void(*assignFunc)(Elem &a, const Elem &b) = &defaultAssign)
	{
		//复制构造
		Elem gottenVal;
		maxSize = L.maxSize + increSize;
		lastPos = L.lastPos;
		data = new Elem[maxSize];
		assign = assignFunc;
		compare = &defaultCompare;

		for (int i = 0; i <= lastPos; i++)
		{
			assign(gottenVal, L.data[i])
			assign(data[i], gottenVal);
		}

	}

	virtual ~SqList()
	{
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

	virtual unsigned getLength() const
	{
		return lastPos + 1;
	}

	virtual int getSearchNo(const Elem & valueToBeSearched) const
	{

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

	virtual bool getData(int i, Elem &x) const
	{
		if (i >= 1 && i <= lastPos + 1)
		{
			assign(x, data[i - 1]);
			return true;
		}
		return false;
	}

	virtual Elem & operator[] (unsigned int i) const
	{
		if (i <= lastPos)
			return data[i];
		else
			throw runtime_error("index greater than max Index in operator[] of SqList");
		return (Elem &)(NULL);
	}

	//在第i个元素前插入元素x
	virtual bool insert(int i, const Elem &x)
	{
		
		if (i < 1 || i > lastPos + 2)
			return false;
		if (lastPos + 2 > maxSize)
		{
			//need to reallocate
			maxSize = maxSize + increSize;
			Elem * newData = new Elem[maxSize];
			Elem * p = data;
			Elem * pd = newData;
			Elem * placeToInsert = data + i - 1;

			while (p < placeToInsert)
				assign(*pd++, *p++);
			assign(*pd++, x);

			Elem * end = data + lastPos + 1;
			while (p < end)
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

	//在末插入x
	virtual void insert(const Elem &x)
	{
		insert(this->lastPos + 2, x);
	}

	//删除指定位置的值
	virtual bool delByNo(int i)
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

	//遍历
	virtual void traverse(void(*myVisit)(const Elem &, int index))
	{
		for (int i = 0; i <= lastPos; i++)
			myVisit(*(data + i), i + 1);
	}

	//遍历
	virtual void traverse(void(*myVisit)(Elem, int index))
	{
		for (int i = 0; i <= lastPos; i++)
			myVisit(*(data + i), i + 1);
	}
};