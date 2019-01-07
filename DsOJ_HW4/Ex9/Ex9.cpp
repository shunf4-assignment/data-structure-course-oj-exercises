/* 1652270 Computer_Class2 Feng Shun */
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

template <typename Elem>
class Queue
{
private:
	Elem *list;
	Elem *front;
	Elem *rear;
public:
	unsigned long length;
	unsigned long capacity;
	//构造
	Queue(unsigned long capacity_)
	{
		list = new Elem[capacity_];
		capacity = capacity_;
		length = 0;
		front = list;
		rear = list;
	}
	//析构
	~Queue()
	{
		if (list != NULL)
			delete[]list;
	}
	//入队，返回是否成功
	bool enqueue(const Elem &e)
	{
		if (length == capacity)
			return false;
		if (length == 0)
		{
			//dont ++rear
			*rear = e;
		}
		else if (rear == list + capacity - 1)
		{
			rear = list;
			*rear = e;
		}
		else
		{
			*(++rear) = e;
		}
		length++;
		return true;
	}
	//出队，返回是否成功
	bool dequeue(Elem &receiver)
	{
		if (length == 0)
			return false;
		if (front == list + capacity - 1)
		{
			receiver = *front;
			front = list;
		}
		else
		{
			receiver = *(front++);
		}
		length--;
		if (length == 0)
		{
			front = rear;
		}
		return true;
	}
	//遍历
	void traverseInQueue(void(*func)(Elem &e, unsigned int i))
	{
		if (length == 0)
			return;
		unsigned count = 0;
		Elem* front2 = front;
		func(*front2, ++count);
		while (front2 != rear)
		{
			++front2;
			if (front2 == list + capacity)
			{
				front2 = list;
			}
			func(*front2, ++count);
		}
	}
};


template <typename Elem>
class LinkList;


void printNum(int &n, unsigned i);

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
class LinkQueue
{
private:
	LinkList<Elem> list;
	LinkNode<Elem> *rearPrev;
public:
	unsigned long length;
	unsigned long capacity;
	//构造
	LinkQueue()
	{
		length = 0;
		rearPrev = list.sentinel;
	}
	//析构
	~LinkQueue()
	{
	}
	//入队
	bool enqueue(const Elem &e)
	{
		LinkNode<Elem> *nl = new LinkNode<Elem>(e);
		rearPrev->next = nl;
		rearPrev = nl;
		length++;
		return true;
	}
	//出队
	bool dequeue(Elem &receiver)
	{
		if (length == 0)
			return false;
		length--;
		if (list.sentinel->next == rearPrev)
		{
			rearPrev = list.sentinel;
		}
		list.getData(1, receiver);
		list.deleteByNo(1);
		return true;
	}
	//遍历
	void traverseInQueue(void(*func)(Elem &e, unsigned int i))
	{
		list.traverse(func);
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


int main()
{
	stringstream s;
	cin.set_rdbuf(s.rdbuf());

	s << "4 \
		dequeue \
		enqueue 10 \
		enqueue 2 \
		enqueue 3 \
		dequeue \
		dequeue \
		enqueue 1 \
		enqueue 2 \
		enqueue 3 \
		enqueue 4 \
		quit" << endl;
	s.seekg(ios::beg);

	if (true)
	{
		cout << "请输入队列容量：" << endl;
		unsigned long n;
		inputTo(n);
		Queue<int> l(n);

		string cmd;

		while (true)
		{
			cout << "请输入命令：" << endl;
			inputTo(cmd);
			if (cmd == "quit")
			{
				l.traverseInQueue(printNum);
				cout << endl;
				break;
			}
			if (cmd == "dequeue")
			{
				int outQueue;
				if (l.dequeue(outQueue))
				{
					cout << "刚出队列的是：" << endl;
					cout << outQueue;
					l.traverseInQueue(printNum);
					cout << endl;
				}
				else
				{
					cout << "Queue is Empty" << endl;
				}
			}
			if (cmd == "enqueue")
			{
				int inQueue;
				inputTo(inQueue);
				if (l.enqueue(inQueue)) {
					cout << "已经入队。" << endl;
					l.traverseInQueue(printNum);
					cout << endl;
				}
				else
					cout << "Queue is Full" << endl;
			}
		}
	}
	
	s.seekg(std::ios::beg);

	if (true)
	{
		cout << "请输入队列容量（链队列，已经弃用）：" << endl;
		unsigned long n;
		inputTo(n);
		LinkQueue<int> l;

		string cmd;

		while (true)
		{
			cout << "请输入命令：" << endl;
			inputTo(cmd);
			if (cmd == "quit")
			{
				l.traverseInQueue(printNum);
				cout << endl;
				break;
			}
			if (cmd == "dequeue")
			{
				int outQueue;
				if (l.dequeue(outQueue))
				{
					cout << "刚出队列的是：" << endl;
					cout << outQueue;
					l.traverseInQueue(printNum);
					cout << endl;
				}
				else
				{
					cout << "Queue is Empty" << endl;
				}
			}
			if (cmd == "enqueue")
			{
				int inQueue;
				inputTo(inQueue);
				if (l.enqueue(inQueue)) {
					cout << "已经入队。" << endl;
					l.traverseInQueue(printNum);
					cout << endl;
				}
				else
					cout << "Queue is Full" << endl;
			}
		}
	}
	
	return 0;
}