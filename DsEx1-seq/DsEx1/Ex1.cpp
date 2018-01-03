/* 1652270 计算机2班 冯舜 */
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <cstring>
#include <strstream>
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
		if(data != NULL)
			delete[] data;
	}

	virtual void clear()
	{
		this->~SqList();
		this->SqList<Elem>::SqList();
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

	virtual Elem & operator[] (int i) const
	{
		if (i <= lastPos)
			return data[i];
		else
			throw runtime_error("index greater than max Index in operator[] of SqList");
		return *(Elem *)(NULL);
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

	virtual bool delFirst(const Elem& x)
	{
		int searchNo;
		searchNo = getSearchNo(x);
		if (searchNo == -1)
			return false;
		delByNo(searchNo);
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

	//删除所有值为x的元素
	virtual bool delAll(const Elem &x, int fromIndex = 1)
	{
		if (fromIndex < 1 || fromIndex > lastPos + 1)
			return false;
		int i = fromIndex - 1;
		Elem * p = data + i;
		int deletedCount = 0;
		while (i <= lastPos)
		{
			if (compare(*p, x) == CompareEqual)
			{
				deletedCount++;
				if (p + deletedCount <= data + lastPos)
					assign(*p, *(p + deletedCount));
				p--;
			}
			else if (deletedCount != 0)
			{
				assign(*(p + 1), *(p + 1 + deletedCount));
			}

			i++;
			p++;
		}

		if (deletedCount != 0)
		{
			lastPos -= deletedCount;
			return true;
		}
		return false;
	}

	//去重
	virtual void deduplicate()
	{
		for (int i = 0; i <= lastPos - 1; i++)
		{
			delAll(data[i], i + 2);
		}
	}

	virtual void reverse()
	{
		if (lastPos == -1)
			return;
		Elem tmpData;
		for (int i = 0; i <= (lastPos) / 2; i++)
		{
			tmpData = data[i];
			data[i] = data[lastPos - i];
			data[lastPos - i] = tmpData;
		}
	}
};

template <typename Elem>
class OrderedSqList : public SqList<Elem>
{
public:
	OrderedSqList(int n) : SqList<Elem>(n)
	{}

	int insert_retInt(const Elem &x)
	{
		Elem *endPos = this->data + this->lastPos;
		Elem *p;
		for (p = endPos; p >= this->data; p--)
			if (this->compare(x, *p) == CompareGreater)
				break;

		int insertNo = p - SqList<Elem>::data + 2;
		SqList<Elem>::insert(insertNo, x);
		return insertNo;
	}

	int del_retInt(const Elem &x)
	{
		int searchNo = this->SqList<Elem>::getSearchNo(x);
		if (searchNo == -1 || searchNo == -2)
			return -1;
		else
		{
			bool delResult = SqList<Elem>::del(searchNo);
			if (!delResult)
				throw runtime_error("");
			return searchNo;
		}
	}

	void mergeTo(OrderedSqList<Elem> &l)
	{
		for (int i = 0; i <= this->lastPos; i++)
			l.insert_retInt(this->data[i]);
	}
};

void printNum(const int &i, int index)
{
	if (index != 1)
		cout << " ";
	cout << i;
}

int main()
{
	istrstream iSS("1 3 3 2 4 2 0 1 5 5 3 yes 0 0 2 9 9 yes 300 300 200 200 500 200 0 7 200 200 300 no 8 9 2 1 3 6 7 0 -1 5 4 9 10 12 0 -1 yes 0 2 199 3 3 7 199 0 199 no");
	auto backupBuf = cin.rdbuf();
	cin.set_rdbuf(iSS.rdbuf());

	//PART1 : 非有序表
	while (true)
	{
		int testInt;
		SqList<int> l(10);
		cout << "请依次输入要用来建立顺序表的整数，空格隔开，以0结束：";
		while (true)
		{
			cin >> testInt;
			if (testInt == 0)
				break;
			if (cin.fail())
			{
				cin.clear();
				break;
			}
			l.insert(testInt);
		}

		cout << endl;
		cout << "顺序表中元素：";
		l.traverse(printNum);
		cout << endl;

		int insertNo, insertNum;
		cout << "在第几个元素前插入：";
		cin >> insertNo;
		cout << endl;
		cout << "插入的数字：";
		cin >> insertNum;
		cout << endl;
		if (l.insert(insertNo, insertNum))
		{
			cout << insertNo << ","<< insertNum << " 插入成功" << endl;
			l.traverse(printNum);
			cout << endl;
		}
		else
		{
			cout << insertNo << "," << insertNum<< "插入失败" << endl;
		}

		int delNum;
		cout << "要删除的数字（删除第一个）：";
		cin >> delNum;
		cout << endl;
		if (l.delFirst(delNum))
		{
			cout << delNum << "删除成功" << endl;
			l.traverse(printNum);
			cout << endl;
		}
		else
		{
			cout << delNum << "删除失败" << endl;
		}

		cout << "要删除的数字（删除所有）：";
		cin >> delNum;
		cout << endl;
		if (l.delAll(delNum))
		{
			cout << delNum << "删除成功" << endl;
			l.traverse(printNum);
			cout << endl;
		}
		else
		{
			cout << delNum << "删除失败" << endl;
		}

		cout << "表去重" << endl;
		l.deduplicate();
		cout << "去重后元素：";
		l.traverse(printNum);
		cout << endl;

		cout << "表逆置" << endl;
		l.reverse();
		cout << "逆置后元素：";
		l.traverse(printNum);
		cout << endl;

		cout << "表清空" << endl;
		l.clear();
		cout << "清空后元素：";
		l.traverse(printNum);
		cout << endl;

		string command;
		cout << "重新开始吗？（yes 或 其他）";
		cin >> command;
		if (command != "yes")
			break;
	}

	//PART 2 : 有序表
	while (true)
	{
		int testInt;
		OrderedSqList<int> l(10);
		OrderedSqList<int> m(10);
		cout << "请依次输入要用来建立有序顺序表的整数，空格隔开，以0结束：";
		
		while (true)
		{
			cin >> testInt;
			if (testInt == 0)
				break;
			if (cin.fail())
			{
				cin.clear();
				break;
			}
			
			cout << endl << "插入到了第" << l.insert_retInt(testInt) << "位" << endl;
		}

		cout << "请输入第二个表的元素：";
		while (true)
		{
			cin >> testInt;
			if (testInt == 0)
				break;
			if (cin.fail())
			{
				cin.clear();
				break;
			}

			cout << endl << "插入到了第" << m.insert_retInt(testInt) << "位" << endl;
		}

		cout << endl;
		cout << "有序顺序表中元素：";
		l.traverse(printNum);
		cout << endl;

		m.traverse(printNum);
		cout << endl;

		int insertNum;
		cout << "插入的数字：";
		cin >> insertNum;
		cout << endl;
		cout<< "在第" << l.insert_retInt(insertNum) << "位插入了" << insertNum << "。" << endl;
		cout << "有序顺序表中元素：";
		l.traverse(printNum);
		cout << endl;

		cout << "将第二个表合并到第一个表" << endl;
		m.mergeTo(l);
		cout << "合并后第一个表：";
		l.traverse(printNum);
		cout << endl;

		string command;
		cout << "重新开始吗？（yes 或 其他）";
		cin >> command;
		if (command != "yes")
			break;
	}
	

	return 0;
}