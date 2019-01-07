/* 1652270 Computer_Class2 Feng Shun */
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <sstream>

template <typename Elem>
class LinkList;

template <typename Elem>
class OrderedLinkList;

template <typename Elem>
class DuCircularLinkList;

template <typename Elem>
class DuLinkNode;

using namespace std;
void printNum(int &n, unsigned i);

template <typename Elem>
class LinkNode
{
	friend class LinkList < Elem >;
	friend class DuLinkNode < Elem >;
	friend class OrderedLinkList< Elem>;
	friend class DuCircularLinkList< Elem>;
	template <typename Elem2>
	friend void mergeOrderedLinkListToDest(OrderedLinkList< Elem2> & dest, OrderedLinkList< Elem2> & source);
public:
	Elem dataField;
	LinkNode< Elem> * next;
public:
	//Constructor
	LinkNode()
	{
		dataField = Elem();
		next = NULL;
	}

	LinkNode(const LinkNode<Elem> &ln)
	{
		dataField = Elem(ln.dataField);
		next = NULL;
	}

	LinkNode(const Elem &x)
	{
		dataField = Elem(x);
		next = NULL;
	}

	virtual ~LinkNode()
	{
	}

	LinkNode& operator=(const LinkNode< Elem> &nl)
	{
		this->~LinkNode();
		this->LinkNode(nl);
		return *this;
	}
};

template <typename Elem>
class DuLinkNode
{
	friend class LinkList< Elem>;
	friend class OrderedLinkList< Elem>;
	friend class DuCircularLinkList< Elem>;
public:
	Elem dataField;
	DuLinkNode< Elem> * next;
	DuLinkNode< Elem> * prev;
public:
	//Constructor
	DuLinkNode()
	{
		dataField = Elem();
		next = NULL;
		prev = NULL;
	}

	DuLinkNode(const LinkNode<Elem> &ln)
	{
		dataField = Elem(ln.dataField);
		next = NULL;
		prev = NULL;
	}

	DuLinkNode< Elem>& operator=(const LinkNode< Elem> &nl)
	{
		this->~DuLinkNode();
		this->DuLinkNode(nl);
		return *this;
	}


};

template <typename Elem, template <typename Elem2> class BaseLinkNode>
class LinkNodeSearchResult
{
public:
	BaseLinkNode< Elem> *pointer, *prev;
	unsigned int number;
	LinkNodeSearchResult(decltype(pointer) _pointer, decltype(prev) _prev, decltype(number) _number) : pointer(_pointer), prev(_prev), number(_number)
	{
	}
};


template <typename Elem>
class LinkList
{
public:
	LinkNode< Elem> * sentinel;


public:
	//Constructor
	LinkList()
	{
		sentinel = new LinkNode< Elem>();
	}

	//���ƹ��캯��
	LinkList(const LinkList< Elem> &source)
	{
		(this->LinkList< Elem>::LinkList)();
		LinkNode< Elem> * currNodeP = sentinel;
		decltype(currNodeP) currNewP, sourceNextP = source.sentinel->next;
		while (sourceNextP != NULL && (this->sentinel->next == NULL || sourceNextP != source.sentinel->next))
		{
			currNewP = new LinkNode< Elem>(*(sourceNextP));
			currNodeP->next = currNewP;
			currNodeP = currNodeP->next;
			sourceNextP = sourceNextP->next;
		}
	}
	//��ձ�
	virtual void clearList()
	{
		LinkNode< Elem> * currNodeP = sentinel->next, *backupP;
		while (currNodeP != NULL)
		{
			backupP = currNodeP;
			currNodeP = currNodeP->next;
			delete backupP;
		}
		sentinel->next = NULL;
	}
	//����
	virtual ~LinkList()
	{
		this->clearList();
		delete sentinel;
	}
	//��ȡ����
	virtual unsigned int getLength() const
	{
		LinkNode< Elem> * currNodeP = sentinel->next;
		unsigned int result(0);
		while (currNodeP != NULL)
		{
			result++;
			currNodeP = currNodeP->next;
		}
		return result;
	}


	//search [value] in this LinkList. return the number of the object
	template<template <typename Elem2> class Node>
	LinkNodeSearchResult< Elem, Node> search(const Elem & value) const
	{
		Node< Elem> * currPrevP = sentinel, *currP = NULL;
		unsigned int count(0);
		while ((currP = currPrevP->next) && count <= 10000)
		{
			count++;
			if (currP->dataField == value)
			{
				LinkNodeSearchResult< Elem, Node> result(currP, currPrevP, count);
				return result;
			}
			currPrevP = currP;
			//currP=next...
		}
		return LinkNodeSearchResult< Elem, Node>(NULL, NULL, 0);
	}
	//����i��Ԫ�ظ��Ƶ�x
	virtual bool getData(unsigned i, Elem &x) const
	{
		if (i == 0)
			return false;
		try
		{
			x = this->operator[](i - 1);
		}
		catch (runtime_error e)
		{
			return false;
		}
		return true;
	}

	//��������
	virtual LinkNodeSearchResult< Elem, LinkNode> getDataDetail(unsigned i) const
	{
		LinkNode< Elem> * currPrevP = sentinel;
		unsigned int count(0);
		while (currPrevP->next != NULL && count < i)
		{
			count++;
			if (count == i)
			{
				return LinkNodeSearchResult< Elem, LinkNode>(currPrevP->next, currPrevP, count + 1);
			}
			currPrevP = currPrevP->next;
		}

		if (currPrevP->next == NULL && count == i - 1)
			return LinkNodeSearchResult< Elem, LinkNode>(NULL, currPrevP, count + 1);

		return LinkNodeSearchResult< Elem, LinkNode>(NULL, NULL, 0);
	}
	//�±����������
	virtual Elem& operator[] (unsigned int i) const
	{
		LinkNode< Elem> * currNodeP = sentinel->next;
		unsigned int count(0);
		while (currNodeP != NULL && count <= i)
		{
			if (count == i)
			{
				return currNodeP->dataField;
			}
			currNodeP = currNodeP->next;
			count++;
		}
		throw runtime_error("index out of range");
	}

	//insert x before i-th node.
	virtual bool insert(int i, const Elem &x)
	{
		if (i == 0)
			return false;
		LinkNodeSearchResult< Elem, LinkNode> thisNodeResult = getDataDetail(i);	//don't use []: may return sentinel

		if (thisNodeResult.number == 0)
			return false;

		LinkNode< Elem> *currNewP = new LinkNode< Elem>(x);
		thisNodeResult.prev->next = currNewP;
		currNewP->next = thisNodeResult.pointer;
		return true;
	}

	//insert x after last.
	virtual void insertTail(const Elem &x)
	{
		LinkNode< Elem> * currPrevP = sentinel;
		while (currPrevP->next != NULL)
		{
			currPrevP = currPrevP->next;
		}

		LinkNode< Elem> *currNewP = new LinkNode< Elem>(x);
		currPrevP->next = currNewP;
		return;
	}
	//ͷ��x
	void insertHead(const Elem &x)
	{
		LinkNode< Elem> *currNewP = new LinkNode< Elem>(x);
		LinkNode< Elem> *tempP;
		tempP = this->sentinel->next;
		this->sentinel->next = currNewP;
		currNewP->next = tempP;
	}
	//ɾ����i��Ԫ��
	virtual bool deleteByNo(unsigned i)
	{
		LinkNodeSearchResult< Elem, LinkNode> nodeResult = getDataDetail(i);
		if (nodeResult.pointer == NULL)
			return false;
		nodeResult.prev->next = nodeResult.pointer->next;
		delete nodeResult.pointer;
		return true;
	}
	//ɾ������������Ԫ��x
	virtual bool deleteByVal_first(const Elem &x)
	{
		LinkNodeSearchResult< Elem, LinkNode> nodeResult = search<LinkNode>(x);
		if (nodeResult.pointer == NULL)
			return false;
		nodeResult.prev->next = nodeResult.pointer->next;
		delete nodeResult.pointer;
		return true;
	}
	//ɾ������Ԫ��x
	virtual bool deleteByVal_all(const Elem &x, LinkNode< Elem> *newSentinel = NULL)
	{
		if (newSentinel == NULL)
			newSentinel = sentinel;

		bool everDelete = false;

		LinkNode< Elem> * loopPrevP = newSentinel, *tempP;
		while (loopPrevP->next != NULL)
		{
			if (x == loopPrevP->next->dataField)
			{
				tempP = loopPrevP->next;
				loopPrevP->next = tempP->next;
				delete tempP;
				everDelete = true;
			}
			else
				loopPrevP = loopPrevP->next;
		}
		return everDelete;
	}
	//ȥ��
	virtual void deduplicate()
	{
		LinkNode< Elem> *currPrevP = sentinel;
		while (currPrevP->next)
		{
			deleteByVal_all(currPrevP->next->dataField, currPrevP->next);
			currPrevP = currPrevP->next;
		}
	}
	//����
	virtual void reverse()
	{
		LinkNode< Elem> *currPrevP = sentinel, *reversedRearP = currPrevP->next, *tempP;
		if (reversedRearP == NULL)
			return;
		while (reversedRearP->next != NULL)
		{
			tempP = reversedRearP->next;
			reversedRearP->next = reversedRearP->next->next;
			tempP->next = sentinel->next;
			sentinel->next = tempP;
		}
	}
	//��func����������Ԫ�ء�
	virtual void traverse(void(*func)(Elem &e, unsigned i))
	{
		LinkNode< Elem> *currPrevP = sentinel;
		unsigned count = 0;
		while (currPrevP->next != NULL)
		{
			count++;
			func((currPrevP->next->dataField), count);
			currPrevP = currPrevP->next;
		}
	}
};

template <typename Elem>
class OrderedLinkList : public LinkList< Elem>
{
public:
	OrderedLinkList()
	{
		LinkList<Elem>();
	}

	//Convertion-Constructor
	OrderedLinkList(const LinkList< Elem> &source)
	{
		this->OrderedLinkList();
		LinkNode< Elem> * currNodeP = this->sentinel;
		decltype(currNodeP) sourceNextP = source.sentinel->next;
		while (sourceNextP != NULL)
		{
			insert(*sourceNextP);
			sourceNextP = sourceNextP->next;
		}
	}

	//Copy-Constructor
	OrderedLinkList(const OrderedLinkList< Elem> &source)
	{
		this->LinkList(static_cast<const LinkList< Elem> &>(source));
	}

	void insert(const Elem &x)
	{
		LinkNode< Elem> *currPrevP = this->sentinel;
		while (currPrevP->next)
		{
			if (x < currPrevP->next->dataField)
				break;
			currPrevP = currPrevP->next;
		}
		LinkNode< Elem> * currNewP;
		currNewP = new LinkNode< Elem>(x);
		currNewP->next = currPrevP->next;
		currPrevP->next = currNewP;
	}

	template <typename Elem2>
	friend void mergeOrderedLinkListToDest(OrderedLinkList< Elem2> &dest, OrderedLinkList< Elem2> &source);
};

template <typename Elem>
void mergeOrderedLinkListToDest(OrderedLinkList< Elem> & dest, OrderedLinkList< Elem> & source)
{
	LinkNode< Elem> * destPrevP = dest.sentinel, *sourcePrevP = source.sentinel, *tmpP;
	while (sourcePrevP->next != NULL && destPrevP->next != NULL)
	{
		if (destPrevP->next->dataField >= sourcePrevP->next->dataField)
		{
			tmpP = destPrevP->next;
			destPrevP->next = sourcePrevP->next;
			sourcePrevP->next = sourcePrevP->next->next;
			destPrevP->next->next = tmpP;

		}
		destPrevP = destPrevP->next;
	}

	if (sourcePrevP->next != NULL)
	{
		destPrevP->next = sourcePrevP->next;
		sourcePrevP->next = NULL;
	}
}

template <typename Elem>
class DuCircularLinkList
{
public:
	DuLinkNode< Elem> * sentinel;


public:
	//Constructor
	DuCircularLinkList()
	{
		sentinel = new DuLinkNode< Elem>();
	}

	//Copy-Constructor
	DuCircularLinkList(const LinkList< Elem> &source)
	{
		this->DuCircularLinkList();
		DuLinkNode< Elem> * currNodeP = this->sentinel;
		decltype(currNodeP) currNewP;
		const LinkNode< Elem> * sourceNextP = source.sentinel->next;
		if (sourceNextP == NULL)
		{
			return;
		}

		while (sourceNextP != NULL && (this->sentinel->next == NULL || sourceNextP != source.sentinel->next))
		{
			currNewP = new DuLinkNode< Elem>(*(sourceNextP));
			currNodeP->next = currNewP;
			this->sentinel->prev = this->sentinel->next->prev = currNewP;
			currNewP->prev = currNodeP;
			currNewP->next = this->sentinel->next;

			currNodeP = currNodeP->next;
			sourceNextP = sourceNextP->next;
		}
	}

	virtual void clearList()
	{
		DuLinkNode< Elem> * currNodeP = sentinel->next, *backupP = NULL;
		while (currNodeP != NULL && (backupP == NULL || currNodeP != sentinel->next))
		{
			backupP = currNodeP;
			currNodeP = currNodeP->next;
			delete backupP;
		}
		sentinel->next = NULL;
		sentinel->prev = NULL;
	}

	virtual ~DuCircularLinkList()
	{
		this->clearList();
		delete sentinel;
	}

	virtual unsigned int getLength() const
	{
		DuLinkNode< Elem> * currNodeP = sentinel->next;
		unsigned int result(0);
		while (currNodeP != NULL && (result == 0 || currNodeP != sentinel->next))
		{
			result++;
			currNodeP = currNodeP->next;
		}
		return result;
	}

	//search [value] in this LinkList. return the number of the object
	//template should only choose DuLinkNode
	template <template <typename Elem2> class Node>
	LinkNodeSearchResult< Elem, Node> search(const Elem & value) const
	{
		Node< Elem> * currPrevP = sentinel, *currP = NULL;
		unsigned int count(0);
		while ((currP = currPrevP->next) && (count == 0 || currP != sentinel->next))
		{
			count++;
			if (currP->dataField == value)
			{
				LinkNodeSearchResult< Elem, Node> result(currP, currPrevP, count);
				return result;
			}
			currPrevP = currP;
			//currP=next...
		}
		return LinkNodeSearchResult< Elem, Node>(NULL, NULL, 0);
	}

	virtual bool getData(unsigned i, Elem &x) const
	{
		if (i == 0)
			return false;
		try
		{
			x = this->operator[](i - 1);
		}
		catch (runtime_error e)
		{
			return false;
		}
		return true;
	}

	//allow i = length + 1
	template <template <typename Elem2> class Node>
	LinkNodeSearchResult< Elem, Node> getDataDetail(unsigned i) const
	{
		Node< Elem> * currPrevP = sentinel;
		unsigned int count(0);
		while (currPrevP->next != NULL && (currPrevP->next != sentinel->next || count == 0) && count < i)
		{
			count++;
			if (count == i)
			{
				return LinkNodeSearchResult< Elem, Node>(currPrevP->next, currPrevP, count + 1);
			}
			currPrevP = currPrevP->next;
		}

		if (currPrevP->next == sentinel->next && count == i - 1)
			return LinkNodeSearchResult< Elem, Node>(NULL, currPrevP, count + 1);

		return LinkNodeSearchResult< Elem, Node>(NULL, NULL, 0);
	}

	virtual Elem& operator[] (unsigned int i) const
	{
		DuLinkNode< Elem> * currNodeP = sentinel->next;
		unsigned int count(0);
		while (currNodeP != NULL && ((currNodeP != sentinel->next) || count == 0) && count <= i)
		{
			if (count == i)
			{
				return currNodeP->dataField;
			}
			currNodeP = currNodeP->next;
			count++;
		}
		throw runtime_error("index out of range");
	}

	//insert x before i-th node.
	virtual bool insert(int i, const Elem &x)
	{
		if (i == 0)
			return false;
		LinkNodeSearchResult< Elem, DuLinkNode> thisNodeResult = getDataDetail<DuLinkNode>(i);	//don't use []: may return sentinel

		if (thisNodeResult.number == 0)
			return false;

		if (this->sentinel->next == NULL)
		{
			DuLinkNode< Elem> *currNewP = new DuLinkNode< Elem>(x);
			this->sentinel->next = this->sentinel->prev = currNewP->next = currNewP->prev = currNewP;
			return true;
		}

		DuLinkNode< Elem> *currNewP = new DuLinkNode< Elem>(x);
		thisNodeResult.prev->next = currNewP;
		currNewP->next = thisNodeResult.pointer;
		currNewP->prev = thisNodeResult.prev;
		if (thisNodeResult.pointer)
			thisNodeResult.pointer->prev = currNewP;

		if (thisNodeResult.prev == this->sentinel)
		{
			//ͷ��
			currNewP->prev = this->sentinel->prev;
			this->sentinel->prev->next = currNewP;
		}

		if (currNewP->next == NULL)
		{
			//β��
			currNewP->next = this->sentinel->next;
			this->sentinel->prev = this->sentinel->next->prev = currNewP;
		}
		return true;
	}

	//insert x after last.
	virtual void insertTail(const Elem &x)
	{
		DuLinkNode< Elem> * currPrevP = sentinel;

		DuLinkNode< Elem> *currNewP = new DuLinkNode< Elem>(x);

		if (this->sentinel->next == NULL)
		{
			this->sentinel->next = this->sentinel->prev = currNewP->next = currNewP->prev = currNewP;
			return;
		}

		this->sentinel->prev->next = currNewP;
		currNewP->prev = this->sentinel->prev;
		this->sentinel->prev = this->sentinel->next->prev = currNewP;
		currNewP->next = this->sentinel->next;
		return;
	}

	virtual bool insertHead(const Elem &x)
	{
		return insert(1, x);
	}

	virtual bool deleteByNo(unsigned i)
	{
		LinkNodeSearchResult< Elem, DuLinkNode> nodeResult = getDataDetail<DuLinkNode>(i);
		if (nodeResult.pointer == NULL)
			return false;

		if (i == 1)
		{
			if (nodeResult.pointer->next == nodeResult.pointer)
			{
				this->sentinel->next = this->sentinel->prev = NULL;
				delete nodeResult.pointer;
				return true;
			}

			this->sentinel->next = nodeResult.pointer->next;
			nodeResult.pointer->next->prev = this->sentinel->prev;
			this->sentinel->prev->next = nodeResult.pointer->next;
			delete nodeResult.pointer;
			return true;
		}

		nodeResult.pointer->prev->next = nodeResult.pointer->next;
		nodeResult.pointer->next->prev = nodeResult.pointer->prev;

		if (nodeResult.pointer->next == this->sentinel->next)
		{
			this->sentinel->prev = nodeResult.pointer->prev;
		}

		delete nodeResult.pointer;
		return true;
	}

	virtual bool deleteByVal_first(const Elem &x)
	{
		LinkNodeSearchResult< Elem, DuLinkNode> nodeResult = search<DuLinkNode>(x);
		if (nodeResult.pointer == NULL)
			return false;

		if (nodeResult.number == 1)
		{
			if (nodeResult.pointer->next == nodeResult.pointer)
			{
				this->sentinel->next = this->sentinel->prev = NULL;
				delete nodeResult.pointer;
				return true;
			}

			this->sentinel->next = nodeResult.pointer->next;
			nodeResult.pointer->next->prev = this->sentinel->prev;
			this->sentinel->prev->next = nodeResult.pointer->next;
			delete nodeResult.pointer;
			return true;
		}

		nodeResult.pointer->prev->next = nodeResult.pointer->next;
		nodeResult.pointer->next->prev = nodeResult.pointer->prev;

		if (nodeResult.pointer->next == this->sentinel->next)
		{
			this->sentinel->prev = nodeResult.pointer->prev;
		}

		delete nodeResult.pointer;
		return true;
	}

	virtual void traverse(void(*func)(Elem &e, unsigned i))
	{
		DuLinkNode< Elem> *currPrevP = this->sentinel;
		unsigned count = 0;
		while (currPrevP->next != this->sentinel->next || (count == 0 && this->sentinel->next != NULL))
		{
			count++;
			func((currPrevP->next->dataField), count);
			currPrevP = currPrevP->next;
		}
	}
};

void printNum(int &n, unsigned i)
{
	if (i != 1)
		cout << " ";
	cout << n;
}

template <typename E>
void inputTo(E &e)
{
	cin >> e;
	cout << e << endl;
}

#define inputTo(e) inputTo<decltype(e)>(e)

template <typename E0, template <typename E> class T>
bool isCircular(const T< E0> &t)
{
	auto sen = t.sentinel;
	auto senBack = t.sentinel;
	sen = sen->next;
	if (sen == NULL)
		return false;
	else
		sen = sen->next;
	while (sen != NULL && sen != senBack->next)
	{
		sen = sen->next;
	}
	if (sen == senBack->next)
		return true;
	return false;
}

int main()
{
	
	stringstream s;
	s << "4 10 30 40 30 1 25 1 40 30 y";
	s << "6 150 150 -2 3 1 4 0 25 7 40 -5 y";
	s << "0 1 25 1 200 10 n";
	s.seekg(ios::beg);
	cin.set_rdbuf(s.rdbuf());
	while (true) {
		
		cout << "=== ��һ�½ڣ���������� ===" << endl;
		unsigned elemNum;
		cout << "�����봴���ĵ������Ԫ�ظ�����" << endl;
		inputTo(elemNum);


		int currNum;
		LinkList< int> l;

		for (; elemNum > 0; elemNum--)
		{
			cout << "�������" << elemNum << "������" << endl;
			inputTo(currNum);
			l.insertHead(currNum);
		}

		cout << "ͷ����ϡ������ǵ�ǰԪ�أ�" << endl;

		l.traverse(printNum);
		cout << endl;

		unsigned insertI;
		int insertX;
		cout << "�������ڵ� i ��Ԫ��ǰ���� x ��i��xֵ��" << endl;
		inputTo(insertI);
		inputTo(insertX);
		cout << "����ɹ��������ӡ�������ݣ�������� -1��" << endl;

		if (l.insert(insertI, insertX))
		{
			l.traverse(printNum);

		}
		else
		{
			cout << -1;
		}

		cout << endl;

		unsigned deleteJ;
		cout << "������ɾ���� j ��Ԫ�ص�jֵ��" << endl;
		inputTo(deleteJ);
		cout << "ɾ���ɹ��������ӡ�������ݣ�������� -1��" << endl;
		if (l.deleteByNo(deleteJ))
		{
			l.traverse(printNum);
		}
		else
		{
			cout << -1;
		}

		cout << endl;

		int searchY;
		cout << "���������Ԫ�ص�ֵ��" << endl;
		inputTo(searchY);
		auto sR = l.search<LinkNode>(searchY);
		cout << "�����ҵ�����ʾ���ǵڼ���Ԫ�أ���û���ҵ�����ʾ-1��" << endl;
		if (sR.number == 0)
			cout << -1;
		else
			cout << sR.number;
		cout << endl;

		cout << "���ڽ���ʾ��ǰԪ�ظ�����" << endl;
		cout << l.getLength();
		cout << endl;

		cout << "���ڽ����øõ�����" << endl;
		l.reverse();
		l.traverse(printNum);
		cout << endl;
		
		int K;
		cout << "������Ҫɾ�������У���Ԫ�ص�ֵ��" << endl;
		inputTo(K);
		cout << "ɾ���ɹ��������ӡ�������ݣ�������� -1��" << endl;
		if (l.deleteByVal_all(K))
		{
			l.traverse(printNum);
		}
		else
		{
			cout << -1;
		}

		cout << endl;

		cout << "����һ��ѭ�����������һ������ֵ��0��1" << endl;
		bool isCir = isCircular<int, LinkList>(l);
		cout << isCir << endl;

		cout << "Ҫ����һ����" << endl;
		char Z;
		inputTo(Z);
		if (Z == 'y')
			continue;
		else
			break;
	}

	s.str("");
	s << "4 10 20 40 30 1 25 1 40 y";
	s << "6 100 200 -2 3 1 4 7 25 7 200 y";
	s << "0 1 25 1 200 n";
	s.seekg(ios::beg);
	while (true) {

		cout << "=== �ڶ��½ڣ�ѭ��˫��������� ===" << endl;
		unsigned elemNum;
		cout << "�����봴����ѭ��˫�������Ԫ�ظ�����" << endl;
		inputTo(elemNum);


		int currNum;
		DuCircularLinkList< int> l;

		for (size_t i = 0; i < elemNum; i++)
		{
			cout << "�������" << i+1 << "������" << endl;
			inputTo(currNum);
			l.insertTail(currNum);
		}

		cout << "β����ϡ������ǵ�ǰԪ�أ�" << endl;

		l.traverse(printNum);
		cout << endl;

		unsigned insertI;
		int insertX;
		cout << "�������ڵ� i ��Ԫ��ǰ���� x ��i��xֵ��" << endl;
		inputTo(insertI);
		inputTo(insertX);
		cout << "����ɹ��������ӡ�������ݣ�������� -1��" << endl;

		if (l.insert(insertI, insertX))
		{
			l.traverse(printNum);

		}
		else
		{
			cout << -1;
		}

		cout << endl;

		unsigned deleteJ;
		cout << "������ɾ���� j ��Ԫ�ص�jֵ��" << endl;
		inputTo(deleteJ);
		cout << "ɾ���ɹ��������ӡ�������ݣ�������� -1��" << endl;
		if (l.deleteByNo(deleteJ))
		{
			l.traverse(printNum);
		}
		else
		{
			cout << -1;
		}

		cout << endl;

		int searchY;
		cout << "���������Ԫ�ص�ֵ��" << endl;
		inputTo(searchY);
		auto sR = l.search<DuLinkNode>(searchY);
		cout << "�����ҵ�����ʾ���ǵڼ���Ԫ�أ���û���ҵ�����ʾ-1��" << endl;
		if (sR.number == 0)
			cout << -1;
		else
			cout << sR.number;
		cout << endl;

		cout << "���ڽ���ʾ��ǰԪ�ظ�����" << endl;
		cout << l.getLength();
		cout << endl;

		cout << "����һ��ѭ�����������һ������ֵ��0��1���޷��ж�һ���ձ��Ƿ�ѭ������" << endl;
		bool isCir = isCircular<int, DuCircularLinkList>(l);
		cout << isCir << endl;

		cout << "Ҫ����һ����" << endl;
		char Z;
		inputTo(Z);
		if (Z == 'y')
			continue;
		else
			break;
	}

	
	return 0;
}