/*1652270 CS2 Feng Shun*/
#include <iostream>
#include <string>

template <typename E>
class Node
{
public:
	E d;
	Node<E> *np;

	Node()
	{
		d = E();
		np = NULL;
	}

	Node(const E& e)
	{
		d = e;
		np = NULL;
	}

	~Node()
	{
		if (np != NULL)
		{
			delete np;
		}
	}
};

template <typename E>
class List
{
public:
	Node<E> *s;

	List()
	{
		s = new Node();
	}

	void appendFront(const E& e)
	{
		Node<E>* ne = new Node(e);
		ne->np = s->np;
		s->np = ne;
	}

	void dropFrontTo(E& e)
	{
		if (s->np == NULL)
			throw "FAIL: STACK EMPTY";
		Node<e>* bp;
		bp = s->np;
		e = bp->d;
		s->np = bp->np;
		delete bp;
	}

	~List()
	{
		delete s;
	}
};

template <typename E>
class Stack
{
private:
	List<E> l;

public:
	void push(const E& e)
	{
		l.appendFront(e);
	}
	
	void pop(E &e)
	{
		l.dropFrontTo(e);
	}
};

typedef enum {LINK, THREAD} Tag;
template <typename E>
class BiTreeNode
{
public:
	E d;
	Tag lt;
	BiTreeNode<E> *lp;
	Tag rt;
	BiTreeNode<E> *rp;

	BiTreeNode()
	{
		lt = rt = LINK;
		lp = rp = NULL;
	}

	BiTreeNode(const E& e)
	{
		this->BiTreeNode<E>();
		d = e;
	}
};

template <typename E>
class BiTree
{
public:
	BiTreeNode<E>* root;

	BiTree()
	{
		root = NULL;
	}

	BiTree(const E& e)
	{
		root = new BiTreeNode<E>(e);
	}

	void clear()
	{
		if (root->lt == LINK && root->lp)
		{
			delete root->lp;
		}

		if (root->rt == LINK && root->rp)
		{
			delete root->rp;
		}

		root->lp = root_rp = NULL;
		root->rt = root->lt = LINK;
	}

	~BiTree()
	{
		clear();
		delete root;
	}

	
};

template <typename E>
void buildTreePreOrder(BiTree<E> bt, const std::string &str)
{
	bt.clear();
	auto p = str.cbegin();
	BiTreeNode<E>** currNodePP;
	Stack<BiTreeNode<E> **> s;
	s.push(&bt.root);
	while (p != str.cend())
	{
		s.pop(currNodePP);
		if (*p == '#')
		{
			
		}
		else
		{
			if (*currNodePP == NULL)
				*currNodePP = new BiTreeNode<E>(E(*p));
			s.push(&((*q)->rp));
			s.push(&((*q)->lp));
		}
		p++;
	}
}

template <typename E>
void printTree(BiTree<E> bt,)
