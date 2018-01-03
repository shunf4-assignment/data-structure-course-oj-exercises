/* 1652270 �����2�� ��˴ */
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
		//���캯��
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
		//���ƹ���
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

	//�ڵ�i��Ԫ��ǰ����Ԫ��x
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

	//��ĩ����x
	virtual void insert(const Elem &x)
	{
		insert(this->lastPos + 2, x);
	}

	//ɾ��ָ��λ�õ�ֵ
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

	//����
	virtual void traverse(void(*myVisit)(const Elem &, int index))
	{
		for (int i = 0; i <= lastPos; i++)
			myVisit(*(data + i), i + 1);
	}

	//����
	virtual void traverse(void(*myVisit)(Elem, int index))
	{
		for (int i = 0; i <= lastPos; i++)
			myVisit(*(data + i), i + 1);
	}

	//ɾ������ֵΪx��Ԫ��
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

	//ȥ��
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

	//PART1 : �������
	while (true)
	{
		int testInt;
		SqList<int> l(10);
		cout << "����������Ҫ��������˳�����������ո��������0������";
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
		cout << "˳�����Ԫ�أ�";
		l.traverse(printNum);
		cout << endl;

		int insertNo, insertNum;
		cout << "�ڵڼ���Ԫ��ǰ���룺";
		cin >> insertNo;
		cout << endl;
		cout << "��������֣�";
		cin >> insertNum;
		cout << endl;
		if (l.insert(insertNo, insertNum))
		{
			cout << insertNo << ","<< insertNum << " ����ɹ�" << endl;
			l.traverse(printNum);
			cout << endl;
		}
		else
		{
			cout << insertNo << "," << insertNum<< "����ʧ��" << endl;
		}

		int delNum;
		cout << "Ҫɾ�������֣�ɾ����һ������";
		cin >> delNum;
		cout << endl;
		if (l.delFirst(delNum))
		{
			cout << delNum << "ɾ���ɹ�" << endl;
			l.traverse(printNum);
			cout << endl;
		}
		else
		{
			cout << delNum << "ɾ��ʧ��" << endl;
		}

		cout << "Ҫɾ�������֣�ɾ�����У���";
		cin >> delNum;
		cout << endl;
		if (l.delAll(delNum))
		{
			cout << delNum << "ɾ���ɹ�" << endl;
			l.traverse(printNum);
			cout << endl;
		}
		else
		{
			cout << delNum << "ɾ��ʧ��" << endl;
		}

		cout << "��ȥ��" << endl;
		l.deduplicate();
		cout << "ȥ�غ�Ԫ�أ�";
		l.traverse(printNum);
		cout << endl;

		cout << "������" << endl;
		l.reverse();
		cout << "���ú�Ԫ�أ�";
		l.traverse(printNum);
		cout << endl;

		cout << "�����" << endl;
		l.clear();
		cout << "��պ�Ԫ�أ�";
		l.traverse(printNum);
		cout << endl;

		string command;
		cout << "���¿�ʼ�𣿣�yes �� ������";
		cin >> command;
		if (command != "yes")
			break;
	}

	//PART 2 : �����
	while (true)
	{
		int testInt;
		OrderedSqList<int> l(10);
		OrderedSqList<int> m(10);
		cout << "����������Ҫ������������˳�����������ո��������0������";
		
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
			
			cout << endl << "���뵽�˵�" << l.insert_retInt(testInt) << "λ" << endl;
		}

		cout << "������ڶ������Ԫ�أ�";
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

			cout << endl << "���뵽�˵�" << m.insert_retInt(testInt) << "λ" << endl;
		}

		cout << endl;
		cout << "����˳�����Ԫ�أ�";
		l.traverse(printNum);
		cout << endl;

		m.traverse(printNum);
		cout << endl;

		int insertNum;
		cout << "��������֣�";
		cin >> insertNum;
		cout << endl;
		cout<< "�ڵ�" << l.insert_retInt(insertNum) << "λ������" << insertNum << "��" << endl;
		cout << "����˳�����Ԫ�أ�";
		l.traverse(printNum);
		cout << endl;

		cout << "���ڶ�����ϲ�����һ����" << endl;
		m.mergeTo(l);
		cout << "�ϲ����һ����";
		l.traverse(printNum);
		cout << endl;

		string command;
		cout << "���¿�ʼ�𣿣�yes �� ������";
		cin >> command;
		if (command != "yes")
			break;
	}
	

	return 0;
}