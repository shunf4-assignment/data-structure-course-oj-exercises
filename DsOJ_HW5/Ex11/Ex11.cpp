/*1652270 ComputerClass2 Fengshun*/
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <string>

using namespace std;

#ifndef ULONG_MAX
#define ULONG_MAX static_cast<unsigned long>(-1)
#endif
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

template <typename Elem>
class LinkQueue
{
private:
	LinkList<Elem> list;
	LinkNode<Elem> *rearPrev;
public:
	unsigned long length;
	unsigned long capacity;

	LinkQueue()
	{
		length = 0;
		rearPrev = list.sentinel;
	}

	~LinkQueue()
	{
	}

	bool enqueue(const Elem &e)
	{
		LinkNode<Elem> *nl = new LinkNode<Elem>(e);
		rearPrev->next = nl;
		rearPrev = nl;
		length++;
		return true;
	}

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

	void traverseInQueue(void(*func)(Elem &e, unsigned int i))
	{
		list.traverse(func);
	}
};


template <typename Elem>
class TreeNode
{
public:
	Elem data;
	TreeNode<Elem> *l;
	TreeNode<Elem> *r;
	//���캯��
	TreeNode() : data()
	{
		l = NULL;
		r = NULL;
	}
	//���ٸý��
	void destroy()
	{
		if (l != NULL)
		{
			l->destroy();
			delete l;
			l = NULL;
		}

		if (r != NULL)
		{
			r->destroy();
			delete r;
			r = NULL;
		}
	}


};

template <typename Elem>
class BinaryTree
{
public:
	TreeNode<Elem> *root;
	//���캯��
	BinaryTree()
	{
		root = NULL;
	}
	//��������
	~BinaryTree()
	{
		if (root != NULL)
		{
			root->destroy();
			delete root;
		}
		root = NULL;
	}
	//�������ַ���������
	void createTree(const string &treeStr)
	{
		decltype(treeStr.cbegin()) it = treeStr.cbegin();
		TreeNode<Elem> **currNode = &root;
		LinkStack<TreeNode<Elem> **> s;

		while (it != treeStr.cend())
		{
			if (*it == '#')
			{
				if (s.length == 0)
					break;
				s.pop(currNode);
				it++;
				continue;
			}
			*currNode = new TreeNode<Elem>;
			(*currNode)->data = *it;
			s.push(&((*currNode)->r));
			s.push(&((*currNode)->l));
			s.pop(currNode);
			it++;
		}
	}

	//�ǵݹ��������
	void preTraverse(void(*func)(Elem &n, unsigned i))
	{
		if (root == NULL)
			return;
		LinkStack<TreeNode<Elem> *> s;
		s.push(root);
		TreeNode<Elem> *currNode;
		unsigned count = 0;
		while (true)
		{
			if (s.length == 0)
				break;
			s.pop(currNode);
			func(currNode->data, ++count);
			if (currNode->r)
				s.push(currNode->r);
			if (currNode->l)
				s.push(currNode->l);
		}
	}
	//�ǵݹ��������
	void inTraverse(void(*func)(Elem &n, unsigned i))
	{
		if (root == NULL)
			return;
		LinkStack<TreeNode<Elem> *> s;
		TreeNode<Elem> *currNode = root;
		unsigned count = 0;
		while (true)
		{
			if (currNode == NULL)
			{
				if (s.length == 0)
					break;
				s.pop(currNode);
				func(currNode->data, ++count);
				currNode = currNode->r;
				continue;
			}
			s.push(currNode);
			currNode = currNode->l;
		}
	}
	//���������ӡ��
	void inTraverse_printTree(void(*func)(const Elem &n, unsigned level), unsigned level = 0, TreeNode<Elem> *r = NULL)
	{
		if (r == NULL)
			r = root;
		if (r == NULL)
			return;
		TreeNode<Elem> *currNode = r;
		if (r->r == NULL && r->l == NULL)
		{
			func(r->data, level);
			return;
		}
		if (r->r != NULL)
		{
			inTraverse_printTree(func, level + 1, r->r);
		}
		else
		{
			func(' ', 0);
		}
		func(r->data, level);
		if (r->l != NULL)
		{
			inTraverse_printTree(func, level + 1, r->l);
		}
		else
		{
			func(' ', 0);
		}
	}
	//�ǵݹ����������ӡ��
	void inTraverse_printTree_nonRecursive(void(*func)(const Elem &n, unsigned level))
	{
		if (root == NULL)
			return;
		LinkStack<TreeNode<Elem> *> s;
		TreeNode<Elem> *currNode = root;
		unsigned level = 0;
		while (true)
		{
			if (currNode == NULL)
			{
				if (s.length == 0)
					break;
				s.pop(currNode);
				level--;

				if (currNode == NULL)
				{
					while (currNode == NULL)
					{
						if (!s.pop(currNode))
							break;
						level--;
					}
					if (s.length == 0)
						break;
					func(currNode->data, level);
					currNode = currNode->l;
					if (currNode == NULL)
					{
						func(' ', 0);
					}
				}
				else
				{
					func(currNode->data, level);
					currNode = currNode->l;
				}

				level++;
				continue;
			}
			s.push(NULL);
			s.push(currNode);
			level++;
			if (currNode->r == NULL && currNode->l != NULL)
			{
				func(' ', 0);
				currNode = NULL;
				continue;
			}
			currNode = currNode->r;
		}
	}
	//�������
	void postTraverse(void(*func)(Elem &n, unsigned i))
	{
		if (root == NULL)
			return;
		LinkStack<TreeNode<Elem> *> s1;
		LinkStack<TreeNode<Elem> *> s2;
		s1.push(root);
		TreeNode<Elem> *currNode;
		unsigned count = 0;
		while (!s1.isEmpty())
		{
			s1.pop(currNode);
			s2.push(currNode);
			if (currNode->l)
				s1.push(currNode->l);
			if (currNode->r)
				s1.push(currNode->r);
		}
		while (!s2.isEmpty())
		{
			s2.pop(currNode);
			func(currNode->data, ++count);
		}
	}
	//��α���
	void levelTraverse(void(*func)(Elem &n, unsigned i))
	{
		if (root == NULL)
			return;
		LinkQueue<TreeNode<Elem> *> q;
		TreeNode<Elem> *currNode;
		unsigned count = 0;

		q.enqueue(root);
		while (q.length)
		{
			q.dequeue(currNode);
			func(currNode->data, ++count);
			if (currNode->l)
				q.enqueue(currNode->l);
			if (currNode->r)
				q.enqueue(currNode->r);
		}
	}
	//������������Ϣ��ֵ�����������
	friend void treeTraverseForAttrs(BinaryTree<Elem>& tree, unsigned &leafNum, unsigned &nodeNum, unsigned &height);
};

void print_treenode(const char &n, unsigned count)
{
	cout << n << endl;
}
//�����������Ĺ��ܺ���
template <typename Elem>
TreeNode<Elem> mirrorTree_func(const TreeNode<Elem>& node)
{
	TreeNode<Elem> mNode;
	mNode.data = node.data;

	if (node.r != NULL)
	{
		mNode.l = new TreeNode<Elem>;
		*(mNode.l) = mirrorTree_func<Elem>(*(node.r));
	}


	if (node.l != NULL)
	{
		mNode.r = new TreeNode<Elem>;
		*(mNode.r) = mirrorTree_func<Elem>(*(node.l));

	}
	return mNode;
}

template <typename Elem>
void treeTraverseForAttrs(BinaryTree<Elem> & tree, unsigned &leafNum, unsigned &nodeNum, unsigned &height)
{
	if (tree.root == NULL)
	{
		leafNum = 0;
		nodeNum = 0;
		height = 0;
		return;
	}
	LinkStack<TreeNode<Elem> *> s;
	s.push(tree.root);
	TreeNode<Elem> *currNode;
	unsigned leafNum_ = 0;
	unsigned level_ = 0;
	unsigned nodeNum_ = 0;
	unsigned maxLevel = 0;
	currNode = tree.root;
	while (true)
	{
		if (currNode == NULL)
		{
			s.pop(currNode);

			if (currNode == NULL)
			{
				while (s.length != 0 && currNode == NULL)
				{
					if (!s.pop(currNode))
						break;
					level_--;
				}
				if (s.length == 0)
					break;
			}
			else
			{
				if (currNode->r == NULL)
					leafNum_++;
			}

			currNode = currNode->r;
			continue;
		}
		s.push(NULL);
		s.push(currNode);
		nodeNum_++;
		level_++;
		if (level_ > maxLevel)
			maxLevel = level_;
		currNode = currNode->l;
	}
	leafNum = leafNum_;
	height = maxLevel;
	nodeNum = nodeNum_;
	return;
}
//����������
template <typename Elem>
void mirrorTree(BinaryTree<Elem>& targetTree, const BinaryTree<Elem>& sourceTree)
{
	targetTree.~BinaryTree();
	if (sourceTree.root == NULL) return;
	targetTree.root = new TreeNode<Elem>;
	*(targetTree.root) = mirrorTree_func<Elem>(*(sourceTree.root));
}

void printNum(int &n, unsigned i)
{
	if (i != 1)
		cout << " ";
	cout << n;
}
void print_nospace(char &n, unsigned i)
{
	cout << n;
}

void print_tree(const char &n, unsigned level)
{
	cout << setw(level * 5) << "" << n << endl;
}

template <typename E>
void inputTo(E &e)
{
	cin >> e;
	cout << e << endl;
}

#define inputTo(e) inputTo<decltype(e)>(e)

#include <sstream>
int main()
{
	stringstream s;
	cin.set_rdbuf(s.rdbuf());

	s << "abc##d##ef### y" << endl;
	s << "a## y" << endl;
	s << "123#456##7#8##9##ab##c##de##fg### y" << endl;
	s << "# n" << endl;
	s.seekg(ios::beg);

	using elem = char;

	while (true)
	{
		BinaryTree<elem> t;
		cout << " === ���Ļ������� ===" << endl;

		cout << "�����������������У�" << endl;
		string treeStr;
		inputTo(treeStr);

		t.createTree(treeStr);
		cout << "��������������" << endl;
		t.preTraverse(print_nospace);
		cout << endl;
		cout << "��������������" << endl;
		t.inTraverse(print_nospace);
		cout << endl;
		cout << "��������������" << endl;
		t.postTraverse(print_nospace);
		cout << endl;
		cout << "��α����������" << endl;
		t.levelTraverse(print_nospace);
		cout << endl;
		//t.inTraverse_printTree(print_tree);
		cout << "��ӡ�������" << endl;
		t.inTraverse_printTree_nonRecursive(print_tree);

		unsigned nodeNum = 0, height = 0, leafNum = 0;
		treeTraverseForAttrs<elem>(t, leafNum, nodeNum, height);
		cout << "����Ҷ������ " << leafNum << endl;
		cout << "���Ľڵ����� " << nodeNum << endl;
		cout << "���ĸ߶ȣ�   " << height << endl;

		BinaryTree<elem> tMirror;
		mirrorTree<elem>(tMirror, t);
		cout << "���������������" << endl;
		tMirror.inTraverse_printTree_nonRecursive(print_tree);

		char c;
		cout << "Ҫ����������" << endl;
		inputTo(c);
		if (c != 'y')
			break;
	}


	return 0;
}