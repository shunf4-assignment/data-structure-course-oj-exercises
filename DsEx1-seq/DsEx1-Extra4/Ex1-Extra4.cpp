/* 1652270 计算机2班 冯舜 */
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <cstring>
using namespace std;

#define ERROR_LIST_INVALID -2

const int defInitSize = 100;
const int increSize = 2;
enum CompareResult { CompareLess, CompareEqual, CompareGreater };

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

	SqList(const SqList<Elem>& L, void (*assignFunc)(Elem &a, const Elem &b) = &defaultAssign)
	{
		//复制构造
		Elem gottenVal;
		maxSize = L.maxSize;
		lastPos = L.lastPos;
		data = new Elem[maxSize];
		assign = assignFunc;
		compare = &defaultCompare;

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

	virtual unsigned getLength() const
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

	virtual bool getData(int i, Elem &x) const
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

	virtual void insert(const Elem &x)
	{
		insert(this->lastPos + 2, x);
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

template <typename Elem>
class OrderedDeDupSqList : public SqList<Elem>
{
public:
	OrderedDeDupSqList(int n = defInitSize) : SqList<Elem>(n)
	{}

	OrderedDeDupSqList(const OrderedDeDupSqList<Elem> &x, void (*assignFunc)(Elem &, const Elem &) = &(SqList<Elem>::defaultAssign)) : SqList<Elem>(x, assignFunc)
	{}

	void insert(const Elem &x)
	{
		Elem *endPos = this->data + this->lastPos;
		Elem *p;
		for (p = endPos; p >= this->data; p--)
		{
			if (this->compare(x, *p) == CompareEqual)
				return;
			if (this->compare(x, *p) == CompareGreater)
				break;
		}
			

		int insertNo = p - SqList<Elem>::data + 2;
		SqList<Elem>::insert(insertNo, x);
		return;
	}
};

template <typename T>
void print(T i, int index)
{
	if (index != 1)
		cout << " ";
	cout << i;
}

SqList<string> * in2out_recursive(const string &inStr)
{
	// remember to delete it!
	if (inStr.length() < 0)
		return NULL;

	SqList<string> *outList = new SqList<string>();
	if (inStr.length() == 0)
	{
		outList->insert(string(""));
		return outList;
	}

	if (inStr.length() == 1)
	{
		outList->insert(inStr);
		return outList;
	}

	if (inStr.length() == 2)
	{
		outList->insert(inStr);
		string reversedStr = string() + inStr[1] + inStr[0];
		outList->insert(reversedStr);
		return outList;
	}

	string formerStr, latterStr;
	string iStr, jStr;
	SqList<string> *formerSeqP = NULL, *latterSeqP = NULL;

	for (unsigned formerLen = 0; formerLen <= inStr.length() - 1; formerLen++)
	{
		formerStr = inStr.substr(1, formerLen);
		latterStr = inStr.substr(formerLen + 1);
		formerSeqP = in2out_recursive(formerStr);
		latterSeqP = in2out_recursive(latterStr);
		
		for (unsigned i = 0; i < formerSeqP->getLength(); i++)
		{
			for (unsigned j = 0; j < latterSeqP->getLength(); j++)
			{
				formerSeqP->getData(i + 1, iStr);
				latterSeqP->getData(j + 1, jStr);
				
				outList->insert(iStr + inStr[0] + jStr);
			}
		}

		delete formerSeqP;
		delete latterSeqP;
	}

	return outList;
}


CompareResult myCompare(const string &a, const string &b)
{
	string a_ = a, b_ = b;
	for (unsigned i = 0; i < a.length(); i++)
	{
		a_[i] = tolower(a_[i]);
	}
	for (unsigned i = 0; i < b.length(); i++)
	{
		b_[i] = tolower(b_[i]);
	}
	if (a_ > b_)
		return CompareGreater;
	if (a_ == b_)
		return CompareEqual;
	return CompareLess;
}

OrderedDeDupSqList<string> * in2out_old(string inStr)
{
	SqList<string> *outListP = in2out_recursive(inStr);
	string tmpStr;
	OrderedDeDupSqList<string> &finalList = *(new OrderedDeDupSqList<string>);
	//finalList.setCompare(&myCompare);
	for (unsigned i = 0; i < outListP->getLength(); i++)
	{
		outListP->getData(i + 1, tmpStr);
		finalList.insert(tmpStr);
	}
	delete outListP;
	return &finalList;
}

SqList<string> * in2out(string inStr)
{
	//此题要求按输入的顺序排，汗……
	//先把输入转换为等长的顺序字符，从A开始
	string mapStr(inStr.length(), '0');
	for (unsigned i = 0; i < inStr.length(); i++)
	{
		mapStr[i] = 'A' + i;
	}
	SqList<string> *outListP = in2out_recursive(mapStr);
	string tmpStr;
	OrderedDeDupSqList<string> sortedList;
	for (unsigned i = 0; i < outListP->getLength(); i++)
	{
		outListP->getData(i + 1, tmpStr);
		sortedList.insert(tmpStr);
	}
	delete outListP;

	SqList<string> &finalList = *(new SqList<string>);

	for (unsigned i = 0; i < sortedList.getLength(); i++)
	{
		sortedList.getData(i + 1, tmpStr);
		for (unsigned j = 0; j < tmpStr.length(); j++)
		{
			tmpStr[j] = inStr[tmpStr[j] - 'A'];
		}
		finalList.insert(tmpStr);
	}

	return &finalList;
}


int main()
{
	string trainOrder;
	cin >> trainOrder;
	SqList<string> &outList = *in2out(trainOrder);

	//SqList<string> &outList = *in2out_recursive(trainOrder);
	string tmpStr;
	for (unsigned i = 0; i < outList.getLength(); i++)
	{
		if (i != 0)
			cout << endl;
		outList.getData(i + 1, tmpStr);
		cout << tmpStr;
	}

	delete &outList;
	return 0;
}