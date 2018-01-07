/* 1652270 Computer_Class2 Feng Shun */
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <cmath>

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
public:
	Elem dataField;
	LinkNode< Elem> * next;

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

	//Copy-Constructor
	LinkList(const LinkList< Elem> &source)
	{
		sentinel = new LinkNode< Elem>();
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
		LinkNodeSearchResult< Elem, LinkNode> nodeResult = getDataDetail(i);
		if (nodeResult.pointer == NULL)
			return false;
		nodeResult.prev->next = nodeResult.pointer->next;
		delete nodeResult.pointer;
		return true;
	}

	virtual bool deleteByVal_first(const Elem &x)
	{
		LinkNodeSearchResult< Elem, LinkNode> nodeResult = search<LinkNode>(x);
		if (nodeResult.pointer == NULL)
			return false;
		nodeResult.prev->next = nodeResult.pointer->next;
		delete nodeResult.pointer;
		return true;
	}

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

	////Copy-Constructor
	//OrderedLinkList(const OrderedLinkList< Elem> &source)
	//{
	//	this->LinkList< Elem>::LinkList(static_cast<const LinkList< Elem> &>(source));
	//}

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

class PolynomialTerm
{
public:
	double coef;
	int expn;

	bool operator==(const PolynomialTerm&b) const
	{
		return expn == b.expn;
	}

	bool operator>(const PolynomialTerm&b) const
	{
		return expn > b.expn;
	}

	bool operator<(const PolynomialTerm&b) const
	{
		return expn < b.expn;
	}

	bool operator>=(const PolynomialTerm&b) const
	{
		return expn >= b.expn;
	}

	bool operator<=(const PolynomialTerm&b) const
	{
		return expn <= b.expn;
	}
};

class Polynomial : public OrderedLinkList<PolynomialTerm>
{
	using Elem = PolynomialTerm;
public:

	LinkNode<PolynomialTerm>* insertAfterThisNode(LinkNode<PolynomialTerm> *thisNode, double coef, int expn)
	{
		static Elem example;
		LinkNode<PolynomialTerm> backup;
		example.expn = expn;
		example.coef = coef;
		LinkNode< Elem> *currNewP = new LinkNode< Elem>(example);
		currNewP->next = thisNode->next;
		thisNode->next = currNewP;
		
		return currNewP;
	}

	void combine()
	{
		auto currPrevP = this->sentinel;
		auto currPrevP2 = this->sentinel;
		decltype(currPrevP) tmpP;
		if (currPrevP->next == NULL)
			return;
		currPrevP2 = currPrevP2->next;

		while (currPrevP->next)
		{
			if (currPrevP2->next != NULL && currPrevP->next->dataField == currPrevP2->next->dataField)
			{
				currPrevP->next->dataField.coef += currPrevP2->next->dataField.coef;
				tmpP = currPrevP->next->next;
				currPrevP->next->next = currPrevP2->next->next;
				delete tmpP;
				currPrevP2 = currPrevP->next;
			}
			else
			{
				//if (currPrevP->next->dataField.coef == 0)
				if (fabs(currPrevP->next->dataField.coef) < 1e-4)
				{
					tmpP = currPrevP->next;
					currPrevP->next = currPrevP->next->next;
					delete tmpP;
					currPrevP2 = currPrevP->next;
				}
				else
				{
					currPrevP = currPrevP->next;
					currPrevP2 = currPrevP->next;
				}
			}
		}
	}

	Polynomial& operator+=(const Polynomial&p)
	{
		LinkNode< Elem> *currPrevP = p.sentinel;
		while (currPrevP->next != NULL)
		{
			insert(currPrevP->next->dataField);
			currPrevP = currPrevP->next;
		}
		combine();
		return (*this);
	}

	Polynomial operator+(const Polynomial &p)
	{
		Polynomial q(*this);
		q += p;
		return q;
	}

	Polynomial operator*(const Polynomial &q)
	{
		Polynomial result;
		auto p1 = this->sentinel;
		auto p2 = q.sentinel;
		PolynomialTerm t;
		for (; p1->next; p1 = p1->next)
		{
			for (p2 = q.sentinel; p2->next; p2 = p2->next)
			{
				t.coef = p1->next->dataField.coef * p2->next->dataField.coef;
				t.expn = p1->next->dataField.expn + p2->next->dataField.expn;
				result.insert(t);
			}
			result.combine();
		}
		return result;
	}

	friend istream & operator >> (istream& in, Polynomial &);
	friend ostream & operator << (ostream & in, const Polynomial& p);

	double operator()(double x)
	{
		double result = 0;
		auto p1 = this->sentinel;
		for (; p1->next; p1 = p1->next)
		{
			result += p1->next->dataField.coef * pow(x, p1->next->dataField.expn);
		}
		return result;
	}
};

void printNum(int &n, unsigned i)
{
	if (i != 1)
		cout << " ";
	cout << n;
}

istream & operator >> (istream & in, Polynomial& p)
{
	unsigned num;
	in >> num;
	PolynomialTerm t;
	for (size_t i = 0; i < num; i++)
	{
		in >> t.coef >> t.expn;
		p.insert(t);
	}
	p.combine();
	return in;
}


ostream & operator << (ostream & out, const Polynomial& p)
{
	auto currPrevP = p.sentinel;
	if (currPrevP->next == NULL)
	{
		//out << 0 << endl;
		return out;
	}
	while (currPrevP->next != NULL)
	{
		out << currPrevP->next->dataField.coef << " " << currPrevP->next->dataField.expn << endl;
		currPrevP = currPrevP->next;
	}
	return out;
}

int main()
{
#ifdef _FS_DEBUG
	FILE *f;
	fopen_s(&f, "poly.txt", "r");
	freopen_s(&f, "poly.txt", "r", stdin);
	//ofstream f2;
	//f2.open("Output.txt", ios::out);
	//cout.set_rdbuf(f2.rdbuf());
#endif
	
	Polynomial p;
	double x;
	cin >> p;
	cin >> x;
	cout << fixed << setprecision(1) << (p(x));

#ifdef _FS_DEBUG
	fclose(f);
#endif
	return 0;
}