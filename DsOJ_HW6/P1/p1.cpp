/*1652270 CS2 Feng Shun*/
#include <iostream>
#include <string>


typedef enum { LEFT_PREF, RIGHT_PREF } Prefer;
typedef enum { PRE_ORDER, IN_ORDER, POST_ORDER } Order;

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
		s = new Node<E>();
	}

	void appendFront(const E& e)
	{
		Node<E>* ne = new Node<E>(e);
		ne->np = s->np;
		s->np = ne;
	}

	void dropFrontTo(E& e)
	{
		if (s->np == NULL)
			throw "FAIL: STACK EMPTY";
		Node<E>* bp;
		bp = s->np;
		e = bp->d;
		s->np = bp->np;
		bp->np = NULL;
		delete bp;
	}

	E& first()
	{
		if (s->np == NULL)
			throw "FAIL: STACK EMPTY";
		return s->np->d;
	}

	~List()
	{
		delete s;
	}
};

template <typename E>
class Stack
{
public:
	List<E> l;

	void push(const E& e)
	{
		l.appendFront(e);
	}
	
	void pop(E &e)
	{
		l.dropFrontTo(e);
	}

	const E& peek()
	{
		return l.first();
	}

	bool empty()
	{
		return l.s->np == NULL;
	}
};

typedef enum {LINK, THREAD} Tag;
template <typename E>
class ThrBiTreeNode
{
public:
	E d;
	Tag lt;
	ThrBiTreeNode<E> *lp;
	Tag rt;
	ThrBiTreeNode<E> *rp;

	ThrBiTreeNode()
	{
		lt = rt = LINK;
		lp = rp = NULL;
	}

	ThrBiTreeNode(const E& e)
	{
		(this->ThrBiTreeNode<E>::ThrBiTreeNode)();
		d = e;
	}
};

template <typename E>
class ThrBiTree
{
public:
	ThrBiTreeNode<E>* root;

	ThrBiTree()
	{
		root = NULL;
	}

	ThrBiTree(const E& e)
	{
		root = new ThrBiTreeNode<E>(e);
	}

	void clear()
	{
		if (root)
		{
			if (root->lt == LINK && root->lp)
			{
				delete root->lp;
			}

			if (root->rt == LINK && root->rp)
			{
				delete root->rp;
			}

			root->lp = root->rp = NULL;
			root->rt = root->lt = LINK;

			delete root;
		}
	}

	~ThrBiTree()
	{
		clear();
	}

	void traverse(Order o, Prefer f, void(*func)(E& e, unsigned int num, unsigned int depth))
	{
		Stack<ThrBiTreeNode<E> **> s;
		s.push(&(root));
		ThrBiTreeNode<E> **currNodePP;
		unsigned int elemCount = 0;
		unsigned int elemLevel = 0;

		bool mark = false;
		while (!s.empty())
		{
			s.pop(currNodePP);
			
			if (currNodePP == NULL)
			{
				elemLevel--;
			}
			else
			if (*currNodePP == NULL || mark)
			{
				if (mark)
				{
					mark = false;
					func((*currNodePP)->d, elemCount, elemLevel - 1);
					elemCount++;
				}

				if (s.empty())
				{
					;
				}
				else
				{
					Node<ThrBiTreeNode<E> **> * tmpP0 = s.l.s->np;
					while (tmpP0 && tmpP0->d == NULL)
						tmpP0 = tmpP0->np;

					if (tmpP0 == NULL)
						continue;

					if (o == POST_ORDER && (*(tmpP0->d)) != NULL)
					{
						if (&((*(tmpP0->d))->lp) == currNodePP || &((*(tmpP0->d))->rp) == currNodePP)
						{
							mark = true;
						}
					}

					Node<ThrBiTreeNode<E> **> * tmpP1 = tmpP0->np;
					while (tmpP1 && tmpP1->d == NULL)
						tmpP1 = tmpP1->np;
					if (tmpP1 == NULL)
						continue;
					
					if (o == IN_ORDER && (*(tmpP0->d)) != NULL)
					{
						if (&((*(tmpP0->d))->lp) == (tmpP1->d) || &((*(tmpP0->d))->rp) == (tmpP1->d))
						{
							mark = true;
						}
					}
				}
			}
			else
			{
				if (f == LEFT_PREF)
				{
					s.push(NULL);
					if (o == POST_ORDER)
						s.push(currNodePP);
					s.push(&((*currNodePP)->rp));
					if (o == IN_ORDER)
						s.push(currNodePP);
					s.push(&((*currNodePP)->lp));
				}
				else
				{
					s.push(NULL);
					if (o == POST_ORDER)
						s.push(currNodePP);
					s.push(&((*currNodePP)->lp));
					if (o == IN_ORDER)
						s.push(currNodePP);
					s.push(&((*currNodePP)->rp));
				}

				if (o == PRE_ORDER)
				{
					func((*currNodePP)->d, elemCount, elemLevel);
					elemCount++;
				}

				elemLevel++;
			}

		}
	}

	
};

template <typename E>
void buildTreePreOrder(ThrBiTree<E> &bt, const std::string &str)
{
	bt.clear();
	auto p = str.cbegin();
	ThrBiTreeNode<E>** currNodePP;
	Stack<ThrBiTreeNode<E> **> s;
	s.push(&bt.root);
	while (p != str.cend())
	{
		s.pop(currNodePP);
		if (*p == '#')
			;
		else
		{
			if (*currNodePP == NULL)
				*currNodePP = new ThrBiTreeNode<E>(E(*p));
			s.push(&((*currNodePP)->rp));
			s.push(&((*currNodePP)->lp));
		}
		p++;
	}
}

void output(char &c, unsigned int num, unsigned int level)
{
	std::cout << c << ", " << num << ", " << level << std::endl;
}

int main()
{
	ThrBiTree<char> t;
	std::string treeStr = "ABDE##FGH#I##JKLN##OR###MP##Q##S#T####CU##VWXZ##a##Y#bcfghi##j##klmn######o##d##e";
	buildTreePreOrder<char>(t, treeStr);
	t.traverse(PRE_ORDER, LEFT_PREF, output);
	std::cout << std::endl;
	t.traverse(IN_ORDER, LEFT_PREF, output);
	std::cout << std::endl;
	t.traverse(POST_ORDER, LEFT_PREF, output);
	std::cout << std::endl;
	return 0;
}
