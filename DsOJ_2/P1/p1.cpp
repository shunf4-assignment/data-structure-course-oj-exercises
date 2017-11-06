/* 1652270 Computer_Class2 Feng Shun */
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cstdlib>
#include <fstream>

template <typename Elem>
class LinkList;

template <typename Elem>
class OrderedLinkList;

template <typename Elem>
class DuCircularLinkList;

using namespace std;

template <typename Elem>
class LinkNode
{
	friend class LinkList < Elem >;
	friend class OrderedLinkList< Elem>;
	friend class DuCircularLinkList< Elem>;
private:
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
class DuLinkNode : public LinkNode< Elem>
{
	friend class LinkList< Elem>;
	friend class OrderedLinkList< Elem>;
	friend class DuCircularLinkList< Elem>;
private:
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
		dataField = Elem(*ln.dataField);
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

template <typename Elem>
class LinkNodeSearchResult
{
public:
	LinkNode< Elem> *pointer, *prev;
	unsigned int number;
	LinkNodeSearchResult(decltype(pointer) _pointer, decltype(prev) _prev, decltype(number) _number) : pointer(_pointer), prev(_prev), number(_number)
	{
	}
};

template <typename Elem>
class DuLinkNodeSearchResult
{
public:
	DuLinkNode< Elem> *pointer, *prev;
	unsigned int number;
	DuLinkNodeSearchResult(decltype(pointer) _pointer, decltype(prev) _prev, decltype(number) _number) : pointer(_pointer), prev(_prev), number(_number)
	{
	}
};


template <typename Elem>
class LinkList
{
protected:
	LinkNode< Elem> * sentinel;
	

public:
	//Constructor
	LinkList()
	{
		sentinel = new LinkNode< Elem>();
	}

	//Copy-Constructor
	LinkList(const LinkList< Elem> &source)
	{
		this->LinkList();
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

	virtual ~LinkList()
	{
		this->clearList();
		delete sentinel;
	}

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
	virtual LinkNodeSearchResult< Elem> search(const Elem & value) const
	{
		LinkNode< Elem> * currPrevP = sentinel, *currP = NULL;
		unsigned int count(0);
		while ((currP = currPrevP->next) && count <= 10000)
		{
			count++;
			if (currP->dataField == value)
			{
				LinkNodeSearchResult< Elem> result(currP, currPrevP, count);
				return result;
			}
			currPrevP = currP;
			//currP=next...
		}
		return LinkNodeSearchResult< Elem>(NULL, NULL, 0);
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
	virtual LinkNodeSearchResult< Elem> getDataDetail(unsigned i) const
	{
		LinkNode< Elem> * currPrevP = sentinel;
		unsigned int count(0);
		while (currPrevP->next != NULL && count < i)
		{
			count++;
			if (count == i)
			{
				return LinkNodeSearchResult< Elem>(currPrevP->next, currPrevP, count + 1);
			}
			currPrevP = currPrevP->next;
		}

		if (currPrevP->next == NULL && count == i - 1)
			return LinkNodeSearchResult< Elem>(NULL, currPrevP, count + 1);

		return LinkNodeSearchResult< Elem>(NULL, NULL, 0);
	}

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
		LinkNodeSearchResult< Elem> thisNodeResult = getDataDetail(i);	//don't use []: may return sentinel
		
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

	void insertHead(const Elem &x)
	{
		LinkNode< Elem> *currNewP = new LinkNode< Elem>(x);
		LinkNode< Elem> *tempP;
		tempP = this->sentinel->next;
		this->sentinel->next = currNewP;
		currNewP->next = tempP;
	}

	virtual bool deleteByNo(unsigned i)
	{
		LinkNodeSearchResult< Elem> nodeResult = getDataDetail(i);
		if (nodeResult.pointer == NULL)
			return false;
		nodeResult.prev->next = nodeResult.pointer->next;
		delete nodeResult.pointer;
		return true;
	}

	virtual bool deleteByVal_first(const Elem &x)
	{
		LinkNodeSearchResult< Elem> nodeResult = search(x);
		if (nodeResult.pointer == NULL)
			return false;
		nodeResult.prev->next = nodeResult.pointer->next;
		delete nodeResult.pointer;
		return true;
	}

	virtual void deleteByVal_all(const Elem &x, LinkNode< Elem> *newSentinel = NULL)
	{
		if (newSentinel == NULL)
			newSentinel = sentinel;

		LinkNode< Elem> * loopPrevP = newSentinel, *tempP;
		while (loopPrevP->next != NULL)
		{
			if (x == loopPrevP->next->dataField)
			{
				tempP = loopPrevP->next;
				loopPrevP->next = tempP->next;
				delete tempP;
			}
			loopPrevP = loopPrevP->next;
		}

	}

	virtual void deduplicate()
	{
		LinkNode< Elem> *currPrevP = sentinel;
		while (currPrevP->next)
		{
			deleteByVal_all(currPrevP->next->dataField, currPrevP->next);
			currPrevP = currPrevP->next;
		}
	}

	virtual void reverse()
	{
		LinkNode< Elem> *currPrevP = sentinel, *reversedRearPrevP = currPrevP, *tempP;
		if (reversedRearPrevP->next == NULL)
			return;
		while (reversedRearPrevP->next->next != NULL)
		{
			tempP = reversedRearPrevP->next->next;
			reversedRearPrevP->next->next = reversedRearPrevP->next->next->next;
			tempP->next = sentinel->next;
			sentinel->next = tempP;
		}
	}

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
			if (x < *currPrevP->next->dataField)
				break;
		}
		LinkNode< Elem> * currNewP;
		currNewP = new LinkNode< Elem>(x);
		currNewP->next = currPrevP->next;
		currPrevP->next = currNewP;
	}

	template <typename Elem2>
	friend void mergeOrderedSqListToDest(OrderedLinkList< Elem2> &dest, OrderedLinkList< Elem2> &source);
};

template <typename Elem>
void mergeOrderedSqListToDest(OrderedLinkList< Elem> & dest, OrderedLinkList< Elem> & source)
{
	LinkNode< Elem> * destPrevP = dest.sentinel, *sourcePrevP = source.sentinel, *tmpP;
	while (sourcePrevP->next != NULL && destPrevP->next != NULL)
	{
		if (*destPrevP->next->dataField <= *sourcePrevP->next->dataField)
		{
			tmpP = destPrevP->next;
			destPrevP->next = sourcePrevP->next;
			sourcePrevP->next = sourcePrevP->next->next;
			destPrevP->next->next = tmpP;
			
		}
		sourcePrevP = sourcePrevP->next;
	}

	if (destPrevP->next != NULL)
	{
		sourcePrevP->next = destPrevP->next;
		destPrevP->next = NULL;
	}
}

template <typename Elem>
class DuCircularLinkList : public LinkList< Elem>
{
protected:
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
		LinkNode< Elem> * currNodeP = sentinel->next, *backupP = NULL;
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
		LinkNode< Elem> * currNodeP = sentinel->next;
		unsigned int result();
		while (currNodeP != NULL && (result == 0 || currNodeP != sentinel->next))
		{
			result++;
			currNodeP = currNodeP->next;
		}
		return result;
	}

	//search [value] in this LinkList. return the number of the object
	virtual DuLinkNodeSearchResult< Elem> search(const Elem & value) const
	{
		DuLinkNode< Elem> * currPrevP = sentinel, *currP = NULL;
		unsigned int count();
		while ((currP = currPrevP->next) && (count == 0 || currP != sentinel->next))
		{
			count++;
			if (*currP->dataField == value)
			{
				DuLinkNodeSearchResult< Elem> result(currP, currPrevP, count);
				return result;
			}
			currPrevP = currP;
			//currP=next...
		}
		return DuLinkNodeSearchResult< Elem>(NULL, NULL, 0);
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
	virtual DuLinkNodeSearchResult< Elem> getDataDetail(unsigned i) const
	{
		LinkNode< Elem> * currPrevP = sentinel;
		unsigned int count();
		while (currPrevP->next != NULL && (currPrevP->next != sentinel->next || count == 0) && count < i)
		{
			count++;
			if (count == i)
			{
				return DuLinkNodeSearchResult< Elem>(currPrevP->next, currPrevP, count + 1);
			}
			currPrevP = currPrevP->next;
		}

		if (currPrevP->next == sentinel->next && count == i - 1)
			return DuLinkNodeSearchResult< Elem>(NULL, currPrevP, count + 1);

		return DuLinkNodeSearchResult< Elem>(NULL, NULL, 0);
	}

	virtual Elem& operator[] (unsigned int i) const
	{
		LinkNode< Elem> * currNodeP = sentinel->next;
		unsigned int count();
		while (currNodeP != NULL && (currNodeP != sentinel->next || count == 0) && count <= i)
		{
			if (count == i)
			{
				return *currNodeP->dataField;
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
		DuLinkNodeSearchResult< Elem> thisNodeResult = getDataDetail(i);	//don't use []: may return sentinel

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
			//Í·²å
			currNewP->prev = this->sentinel->prev;
			this->sentinel->prev->next = currNewP;
		}

		if (currNewP->next == NULL)
		{
			//Î²²å
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
		DuLinkNodeSearchResult< Elem> nodeResult = getDataDetail(i);
		if (nodeResult.pointer == NULL)
			return false;

		if (i == 1)
		{
			if (nodeResult.pointer->next == nodeResult.pointer)
			{
				this->sentinel->next = this->sentinel->prev = NULL;
				delete nodeResult;
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
		DuLinkNodeSearchResult< Elem> nodeResult = search(x);
		if (nodeResult.pointer == NULL)
			return false;

		if (nodeResult.number == 1)
		{
			if (nodeResult.pointer->next == nodeResult.pointer)
			{
				this->sentinel->next = this->sentinel->prev = NULL;
				delete nodeResult;
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
		while (currPrevP->next != this->sentinel->next || count == 0)
		{
			count++;
			func(*(currPrevP->next->dataField), count);
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

int main()
{
#ifdef _FS_DEBUG
	FILE *f;
	fopen_s(&f, "p1.txt", "r");
	freopen_s(&f, "p1.txt", "r", stdin);
	//ofstream f2;
	//f2.open("Output.txt", ios::out);
	//cout.set_rdbuf(f2.rdbuf());
#endif
	unsigned elemNum;
	cin >> elemNum;
	int currNum;
	LinkList< int> l;

	for (; elemNum > 0; elemNum--)
	{
		cin >> currNum;
		l.insertHead(currNum);
	}
	

	l.traverse(printNum);
	cout << endl;

	unsigned insertI;
	int insertX;
	cin >> insertI >> insertX;
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
	cin >> deleteJ;
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
	cin >> searchY;
	LinkNodeSearchResult< int> sR = l.search(searchY);
	if (sR.number == 0)
		cout << -1;
	else
		cout << sR.number;
	cout << endl;

	cout << l.getLength();
#ifdef _FS_DEBUG
	fclose(f);
#endif
	return 0;
}