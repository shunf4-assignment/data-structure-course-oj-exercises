/*1652270 CS2 Feng Shun*/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>


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
class ThrBiTreeNode;

template <typename X>
struct ChildThread
{
	Tag t;
	ThrBiTreeNode<X> *c;
};

template <typename E>
class ThrBiTreeNode
{
	
public:
	E d;
	ChildThread <E> l;
	ChildThread <E> r;

	ThrBiTreeNode()
	{
		l.t = r.t = LINK;
		l.c = r.c = NULL;
	}

	ThrBiTreeNode(const E& e)
	{
		l.t = r.t = LINK;
		l.c = r.c = NULL;
		d = e;
	}

	~ThrBiTreeNode()
	{
		if (l.t == LINK && l.c)
		{
			delete l.c;
		}

		if (r.t == LINK && r.c)
		{
			delete r.c;
		}

		l.c = r.c = NULL;
		l.t = r.t = LINK;
	}
};


template <typename E>
class ThrBiTree
{
public:
	ThrBiTreeNode<E>* root;
	static bool startTraverse;

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
			delete root;
		}
	}

	~ThrBiTree()
	{
		clear();
	}

	void traverse_nonRecursive(Order o, Prefer f, void(*func)(ThrBiTreeNode<E> &n, unsigned int num, unsigned int depth))
	{
		Stack<ChildThread<E> *> s;
		ChildThread<E> rootChildThreadP;
		rootChildThreadP.c = root;
		rootChildThreadP.t = LINK;
		s.push(&rootChildThreadP);
		ChildThread<E> *currP;
		unsigned int elemCount = 0;
		unsigned int elemLevel = 0;

		bool mark = false;
		
		ThrBiTreeNode<E> *nullPointer = NULL;

		while (!s.empty())
		{
			s.pop(currP);
			
			if (currP == NULL)
			{
				elemLevel--;
			}
			else
			if ((currP->t == THREAD || currP->c == NULL) || mark)
			{
				if (mark)
				{
					mark = false;
					func(*(currP->c), elemCount, elemLevel - 1);
					elemCount++;
				}

				if (s.empty())
				{
					;
				}
				else
				{
					Node<ChildThread<E> *> * tmpP0 = s.l.s->np;	//栈顶的第一个非NULL元素
					while (tmpP0 && tmpP0->d == NULL)
						tmpP0 = tmpP0->np;

					if (tmpP0 == NULL)
						continue;

					if (o == POST_ORDER && (tmpP0->d->c) != NULL)
					{
						if (&(tmpP0->d->c->l) == currP || &(tmpP0->d->c->r) == currP)
						{
							mark = true;
						}
					}

					Node<ChildThread<E> *> * tmpP1 = tmpP0->np;
					while (tmpP1 && tmpP1->d == NULL)
						tmpP1 = tmpP1->np;
					if (tmpP1 == NULL)
						continue;
					
					if (o == IN_ORDER && (tmpP0->d->c) != NULL)
					{
						if (&(tmpP0->d->c->l) == (tmpP1->d) || &(tmpP0->d->c->r) == (tmpP1->d))
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
						s.push(currP);
					s.push(&(currP->c->r));
					if (o == IN_ORDER)
						s.push(currP);
					s.push(&(currP->c->l));
				}
				else
				{
					s.push(NULL);
					if (o == POST_ORDER)
						s.push(currP);
					s.push(&(currP->c->l));
					if (o == IN_ORDER)
						s.push(currP);
					s.push(&(currP->c->r));
				}

				if (o == PRE_ORDER)
				{
					func(*(currP->c), elemCount, elemLevel);
					elemCount++;
				}

				elemLevel++;
			}

		}
	}

	

	void traverse(Order o, Prefer f, void(*func)(ThrBiTreeNode<E> &n, unsigned int num, unsigned int depth))
	{
		startTraverse = true;
		if(this->root)
			traverse_do(this->root, o, f, 0, func);
	}


	static void traverse_do(ThrBiTreeNode<E> *n, Order o, Prefer f, unsigned int depth, void(*func)(ThrBiTreeNode<E> &n, unsigned int num, unsigned int depth))
	{
		unsigned static int elemCount;
		if (startTraverse)
		{
			startTraverse = false;
			elemCount = 0;
		}
		if (o == PRE_ORDER)
		{
			func(*n, elemCount++, depth);
		}

		if (f == LEFT_PREF)
		{
			if(n->l.t == LINK && n->l.c)
				traverse_do(n->l.c, o, f, depth + 1, func);

			if (o == IN_ORDER)
			{
				func(*n, elemCount++, depth);
			}
		}

		if (n->r.t == LINK && n->r.c)
			traverse_do(n->r.c, o, f, depth + 1, func);

		if (f == RIGHT_PREF)
		{
			if (o == IN_ORDER)
			{
				func(*n, elemCount++, depth);
			}

			if (n->l.t == LINK && n->l.c)
				traverse_do(n->l.c, o, f, depth + 1, func);
		}

		if (o == POST_ORDER)
		{
			func(*n, elemCount++, depth);
		}
	}

	void threading_nonRecursive(Order o)
	{
		Stack<ChildThread<E> *> s;
		ChildThread<E> rootChildThreadP;
		rootChildThreadP.c = root;
		rootChildThreadP.t = LINK;
		s.push(&rootChildThreadP);
		ChildThread<E> *currP, *prevP;
		unsigned int elemCount = 0;
		unsigned int elemLevel = 0;

		bool mark = false;

		prevP = NULL;
		ThrBiTreeNode<E> *nullPointer = NULL;

		while (!s.empty())
		{
			s.pop(currP);

			if (currP == NULL)
			{
				elemLevel--;
			}
			else
				if ((currP->t == THREAD || currP->c == NULL) || mark)
				{
					if (mark)
					{
						mark = false;
						//func(currP->c->d, elemCount, elemLevel - 1);
						if (currP->c->l.c == NULL)
						{
							if(prevP != NULL)
								currP->c->l.c = prevP->c;
							else
								currP->c->l.c = NULL;
							currP->c->l.t = THREAD;
						}
						
						if (prevP && prevP->c->r.c == NULL)
						{
							prevP->c->r.c = currP->c;
							prevP->c->r.t = THREAD;
						}

						prevP = currP;

						elemCount++;
					}

					if (s.empty())
					{
						;
					}
					else
					{
						Node<ChildThread<E> *> * tmpP0 = s.l.s->np;	//栈顶的第一个非NULL元素
						while (tmpP0 && tmpP0->d == NULL)
							tmpP0 = tmpP0->np;

						if (tmpP0 == NULL)
							continue;

						if (o == POST_ORDER && (tmpP0->d->c) != NULL)
						{
							if (&(tmpP0->d->c->l) == currP || &(tmpP0->d->c->r) == currP)
							{
								mark = true;
							}
						}

						Node<ChildThread<E> *> * tmpP1 = tmpP0->np;
						while (tmpP1 && tmpP1->d == NULL)
							tmpP1 = tmpP1->np;
						if (tmpP1 == NULL)
							continue;

						if (o == IN_ORDER && (tmpP0->d->c) != NULL)
						{
							if (&(tmpP0->d->c->l) == (tmpP1->d) || &(tmpP0->d->c->r) == (tmpP1->d))
							{
								mark = true;
							}
						}
					}
				}
				else
				{
					//if (f == LEFT_PREF)
					if (true)
					{
						s.push(NULL);
						if (o == POST_ORDER)
							s.push(currP);
						s.push(&(currP->c->r));
						if (o == IN_ORDER)
							s.push(currP);
						s.push(&(currP->c->l));
					}
					else
					{
						s.push(NULL);
						if (o == POST_ORDER)
							s.push(currP);
						s.push(&(currP->c->l));
						if (o == IN_ORDER)
							s.push(currP);
						s.push(&(currP->c->r));
					}

					if (o == PRE_ORDER)
					{
						if (currP->c->l.c == NULL)
						{
							if (prevP != NULL)
								currP->c->l.c = prevP->c;
							else
								currP->c->l.c = NULL;
							currP->c->l.t = THREAD;
						}

						if (prevP && prevP->c->r.c == NULL)
						{
							prevP->c->r.c = currP->c;
							prevP->c->r.t = THREAD;
						}

						prevP = currP;
						elemCount++;
					}

					elemLevel++;
				}

		}
		if (prevP && prevP->c->r.c == NULL)
			prevP->c->r.t = THREAD;
			
	}

	void threading(Order o)
	{
		if (this->root == NULL)
			return;
		ThrBiTreeNode<E> *lastP = threading_do(o, NULL, this->root);
		if (lastP && lastP->r.c == NULL)
			lastP->r.t = THREAD;
	}

	static ThrBiTreeNode<E> * threading_do(Order o, ThrBiTreeNode<E> *pre, ThrBiTreeNode<E> *cur)
	{
		ThrBiTreeNode<E> *newPre;
		if (cur == NULL)
			return pre;

		newPre = pre;

		if (o == PRE_ORDER)
		{
			if (cur->l.c == NULL)
			{
				cur->l.c = pre;
				cur->l.t = THREAD;
			}

			if (pre && pre->r.c == NULL)
			{
				pre->r.c = cur;
				pre->r.t = THREAD;
			}

			newPre = cur;
		}

		if(cur->l.t != THREAD)
			newPre = threading_do(o, newPre, cur->l.c);

		if (o == IN_ORDER)
		{
			if (cur->l.c == NULL)
			{
				cur->l.c = newPre;
				cur->l.t = THREAD;
			}

			if (newPre && newPre->r.c == NULL)
			{
				newPre->r.c = cur;
				newPre->r.t = THREAD;
			}

			newPre = cur;
		}

		if(cur->r.t != THREAD)
			newPre = threading_do(o, newPre, cur->r.c);

		if (o == POST_ORDER)
		{
			if (cur->l.c == NULL)
			{
				cur->l.c = newPre;
				cur->l.t = THREAD;
			}

			if (newPre && newPre->r.c == NULL)
			{
				newPre->r.c = cur;
				newPre->r.t = THREAD;
			}

			newPre = cur;
		}

		return newPre;
	}
	
};


template <typename E>
bool ThrBiTree<E>::startTraverse = false;

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
			s.push(&((*currNodePP)->r.c));
			s.push(&((*currNodePP)->l.c));
		}
		p++;
	}
}

void output(ThrBiTreeNode<char> &n, unsigned int num, unsigned int level)
{
	std::cout << n.d << ", " << num << ", " << level << std::endl;
}

void printNode(ThrBiTreeNode<char> &n, unsigned int num, unsigned int depth)
{
	std::cout << std::setw(depth * 5) << "" << n.d << n.l.t << n.r.t;
	std::cout << std::endl;
}

int main()
{
#ifdef _FS_DEBUG
	FILE *f;
	fopen_s(&f, "p1.txt", "r");
	freopen_s(&f, "p1.txt", "r", stdin);
	std::ofstream f2;
	f2.open("Output.txt", std::ios::out);
	//std::cout.set_rdbuf(f2.rdbuf());
#endif

	std::string treeStr;
	std::cin >> treeStr;

	if (0)
	{
		ThrBiTree<char> t;
		buildTreePreOrder<char>(t, treeStr);
		t.traverse(IN_ORDER, RIGHT_PREF, printNode);
		t.threading(IN_ORDER);
		t.traverse(PRE_ORDER, LEFT_PREF, output);
		std::cout << std::endl;
		t.traverse(IN_ORDER, LEFT_PREF, output);
		std::cout << std::endl;
		t.traverse(POST_ORDER, LEFT_PREF, output);
		std::cout << std::endl;
	}
	
	if (0)
	{
		ThrBiTree<char> t;
		buildTreePreOrder<char>(t, treeStr);
		t.threading_nonRecursive(IN_ORDER);
		t.traverse_nonRecursive(PRE_ORDER, LEFT_PREF, output);
		std::cout << std::endl;
		t.traverse_nonRecursive(IN_ORDER, LEFT_PREF, output);
		std::cout << std::endl;
		t.traverse_nonRecursive(POST_ORDER, LEFT_PREF, output);
		std::cout << std::endl;
	}

	if (1)
	{
		ThrBiTree<char> t;
		buildTreePreOrder<char>(t, treeStr);
		t.threading(PRE_ORDER);
		t.traverse(IN_ORDER, RIGHT_PREF, printNode);

		ThrBiTreeNode<char> *tP = t.root;
		while (true)
		{
			std::cout << tP->d;
			if (tP->l.t != THREAD)
			{
				tP = tP->l.c;
			}
			else
			{
				if (tP->r.c == NULL)
					break;
				else
					tP = tP->r.c;
			}
		}

		std::cout << std::endl;
	}

	if (0)
	{
		ThrBiTree<char> t;
		buildTreePreOrder<char>(t, treeStr);
		t.threading_nonRecursive(IN_ORDER);

		ThrBiTreeNode<char> *tP = t.root;
		std::string seq = "";
		std::string cap = "";

		char c;
		std::cin >> c;
		bool found = false;

		tP = t.root;

		ThrBiTreeNode<char> *sucP = NULL, *preP = NULL;

		while (tP)
		{
			while (true)
			{
				if (tP->l.t == THREAD)
					break;
				tP = tP->l.c;
			}
			seq += tP->d;
			if (tP->d == c)
			{
				found = true;
				if (tP->r.c) {
					sucP = tP->r.c;
					if (tP->r.t == LINK)
					{
						while (sucP->l.t == LINK)
							sucP = sucP->l.c;
					}
				}
				else sucP = NULL;
				if (tP->l.c)
				{
					preP = tP->l.c;
					if (tP->l.t == LINK)
					{
						while (preP->r.t == LINK)
							preP = preP->r.c;
					}
				}
				else preP = NULL;
			}
			while (tP->r.c && tP->r.t == THREAD)
			{
				tP = tP->r.c;
				seq += tP->d;
				if (tP->d == c)
				{
					found = true;
					if (tP->r.c) {
						sucP = tP->r.c;
						if (tP->r.t == LINK)
						{
							while (sucP->l.t == LINK)
								sucP = sucP->l.c;
						}
					}
					else sucP = NULL;
					if (tP->l.c)
					{
						preP = tP->l.c;
						if (tP->l.t == LINK)
						{
							while (preP->r.t == LINK)
								preP = preP->r.c;
						}
					}
					else preP = NULL;
				}
			}

			tP = tP->r.c;
		}
		std::cout << seq << std::endl;

		if (!found)
		{
			std::cout << "Not found" << std::endl;
		}
		else
		{
			std::cout << "succ is ";
			if (sucP)
				std::cout << sucP->d << sucP->r.t << std::endl;
			else
				std::cout << "NULL" << std::endl;
			std::cout << "prev is ";
			if (preP)
				std::cout << preP->d << preP->l.t << std::endl;
			else
				std::cout << "NULL" << std::endl;
		}
	}

#ifdef _FS_DEBUG
	fclose(f);
	f2.close();
#endif
	return 0;
}
