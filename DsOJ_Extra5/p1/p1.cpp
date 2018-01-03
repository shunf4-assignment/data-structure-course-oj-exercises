/* 1652270 Computer_Class2 Feng Shun */
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <fstream>

#ifndef ULONG_MAX
#define ULONG_MAX static_cast<unsigned long>(-1)
#endif

using namespace std;
template <typename Elem>
class LinkList;

template <typename Elem>
class DuLinkNode;


template <typename Elem>
class LinkNode
{
	friend class LinkList < Elem >;
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
class LinkStack
{
private:
	LinkList<Elem> list;
public:
	unsigned long length;
	unsigned long capacity;

public:
	LinkStack()
	{
		list.clearList();
		capacity = ULONG_MAX;
		length = 0;
	}

	LinkStack(unsigned long capacity_)
	{
		list.clearList();
		capacity = capacity_;
		length = 0;
	}

	~LinkStack()
	{

	}

	bool push(const Elem &x)
	{
		if (length + 1 > capacity)
			return false;
		list.insertHead(x);
		length++;
		return true;
	}

	bool pop(Elem &receiver)
	{
		if (isEmpty())
			return false;
		list.getData(1, receiver);
		list.deleteByNo(1);
		length--;
		return true;
	}

	bool pop()
	{
		if (isEmpty())
			return false;
		list.deleteByNo(1);
		length--;
		return true;
	}

	bool isEmpty()
	{
		return length == 0;
		//	return list.sentinel->next == NULL;
	}

	void traverseInStack(void(*func)(Elem &e, unsigned int i))
	{
		list.traverse(func);
	}
};

int main()
{
#ifdef _FS_DEBUG
	FILE *f;
	fopen_s(&f, "P1.txt", "r");
	freopen_s(&f, "P1.txt", "r", stdin);
	//ofstream f2;
	//f2.open("Output.txt", ios::out);
	//cout.set_rdbuf(f2.rdbuf());
#endif
	char testChar;
	int currChar;
	bool good = true;
	LinkStack<char> l;
	while (!cin.eof())
	{
		currChar = cin.get();
		if (currChar == -1)
		{
			break;
		}
		if (currChar == '{')
			l.push('{');
		else if (currChar == '[')
			l.push('[');
		else if (currChar == '(')
			l.push('(');
		else if (currChar == '}')
		{
			if (!l.pop(testChar))
			{
				cout << "no" << endl;
				cout << "}ÆÚ´ý×óÀ¨ºÅ" << endl;
				good = false;
				break;
			}
			else if (testChar != '{')
			{
				cout << "no" << endl;
				cout << testChar << "ÆÚ´ýÓÒÀ¨ºÅ" << endl;
				//cout << "}ÆÚ´ý×óÀ¨ºÅ" << endl;
				good = false;
				break;
			}
		}
		else if (currChar == ']')
		{
			if (!l.pop(testChar))
			{
				cout << "no" << endl;
				cout << "]ÆÚ´ý×óÀ¨ºÅ" << endl;
				good = false;
				break;
			}
			else if (testChar != '[')
			{
				cout << "no" << endl;
				cout << testChar << "ÆÚ´ýÓÒÀ¨ºÅ" << endl;
				//cout << "]ÆÚ´ý×óÀ¨ºÅ" << endl;
				good = false;
				break;
			}
		}
		else if (currChar == ')')
		{
			if (!l.pop(testChar))
			{
				cout << "no" << endl;
				cout << ")ÆÚ´ý×óÀ¨ºÅ" << endl;
				good = false;
				break;
			}
			else if (testChar != '(')
			{
				cout << "no" << endl;
				cout << testChar << "ÆÚ´ýÓÒÀ¨ºÅ" << endl;
				//cout << ")ÆÚ´ý×óÀ¨ºÅ" << endl;
				good = false;
				break;
			}
		}
	}
	if (good && l.length != 0)
	{
		/*while (l.length != 1)
		{
			l.pop();
		}*/
		l.pop(testChar);
		cout << "no" << endl;
		cout << testChar << "ÆÚ´ýÓÒÀ¨ºÅ" << endl;
		good = false;
	}
	if (good)
	{
		cout << "yes" << endl;
	}
#ifdef _FS_DEBUG
	fclose(f);
#endif
	return 0;
}