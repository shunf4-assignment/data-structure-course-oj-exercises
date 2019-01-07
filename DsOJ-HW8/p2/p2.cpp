/*1652270 CS2 fengshun*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <climits>
#include <assert.h>

using std::cin;
using std::cout;
using std::endl;
using std::string;

typedef enum { STATOK, STATFULL, STATMISPLACE, STATEMPTY } status;

template <typename E>
class SLL
{
public:
	struct SLLNode
	{
		E e;
		int c;
	};

	SLLNode *s;
	int size;
	int tail;

	SLL() = default;
	SLL(int size_) { init(size_); }

	void init(int size_)
	{
		size = size_;
		s = new SLLNode[size];
		for (int i = 0; i < size; i++)
		{
			if (i == 0 || i == size - 1)
			{
				s[i].c = -1;
			}
			else
			{
				s[i].c = i + 1;
			}
			s[i].e = E();
		}
		tail = 0;
	}

	status append(const E &e, int &iR)
	{
		if (s[1].c == -1)
		{
			return STATFULL;
		}
		int allocMem = s[1].c;
		s[1].c = s[allocMem].c;
		s[allocMem].e = e;
		s[allocMem].c = -1;

		s[tail].c = allocMem;
		tail = allocMem;
		iR = allocMem;
		return STATOK;
	}

	status insert(int no, const E &e, int &iR)
	{
		if (no <= 0)
			return STATMISPLACE;
		if (s[1].c == -1)
		{
			return STATFULL;
		}

		int insertIndex = 0;
		int insertPrev = -1;
		for (int i = 0; i < no; i++)
		{
			insertPrev = insertIndex;
			if (insertPrev == -1)
			{
				return STATMISPLACE;
			}
			insertIndex = s[insertIndex].c;

		}
		int allocMem = s[1].c;
		s[1].c = s[allocMem].c;
		s[allocMem].e = e;
		s[allocMem].c = insertIndex;

		s[insertPrev].c = allocMem;

		if (insertIndex == -1)
		{
			tail = allocMem;
		}
		iR = allocMem;
		return STATOK;
	}

	status del(int no, E& e)
	{
		if (no <= 0)
			return STATMISPLACE;
		if (s[0].c == -1)
			return STATEMPTY;
		int delPrev = -1;
		int delIndex = 0;
		for (int i = 0; i < no; i++)
		{
			delPrev = delIndex;
			delIndex = s[delIndex].c;
			if (delIndex == -1)
			{
				return STATMISPLACE;
			}
		}

		if (tail == delIndex)
		{
			tail = delPrev;
		}

		s[delPrev].c = s[delIndex].c;
		s[delIndex].c = s[1].c;
		s[1].c = delIndex;
		e = s[delIndex].e;

		return STATOK;
	}

	status delByIndex(int index)
	{
	
		if (s[0].c == -1)
			return STATEMPTY;
		int delPrev = -1;
		int delIndex = 0;
		while(true)
		{
			delPrev = delIndex;
			delIndex = s[delIndex].c;
			if (delIndex == -1)
			{
				return STATMISPLACE;
			}
			if (delIndex == index)
				break;
		}

		if (tail == delIndex)
		{
			tail = delPrev;
		}

		s[delPrev].c = s[delIndex].c;
		s[delIndex].c = s[1].c;
		s[1].c = delIndex;

		return STATOK;
	}

	int search(const E &e)
	{
		if (s[0].c == -1)
			return -1;

		int index = 0;
		for (int i = 1; ; i++)
		{
			index = s[index].c;
			if (index == -1)
			{
				return -1;
			}
			if (s[index].e == e)
			{
				return i;
			}
		}
		return -1;
	}

	template <typename E_>
	friend std::ostream & operator <<(std::ostream& out, const SLL<E_>& sll);

	void print(std::ostream& out) const
	{
		int count = 0;
		for (int i = 0; i < size; i++)
		{
			count++;
			if (count == 3)
				count = 0;
			if (count == 2 || count == 0)
			{
				out << "\t\t";
			}
			out << i << " : " << s[i].e << " : " << s[i].c;
			if (count == 0)
				out << endl;
		}
		if (count)
			cout << endl;
	}

	~SLL()
	{
		delete[]s;
	}
};

template <typename E>
std::ostream & operator <<(std::ostream& out, const SLL<E>& sll)
{
	for (auto i = sll.s[0].c; i != -1; i = sll.s[i].c)
	{
		if (i != sll.s[0].c)
			out << " ";
		out << sll.s[i].e;
	}
	return out;
}

template <typename E>
struct Node
{
	E e;
	int l;
	int r;
};

template <typename E>
struct BiSearchTree
{
	SLL<Node<E>> sll;
	int root;

	BiSearchTree(int size)
	{
		sll.init(size);
		root = -1;
	}

	bool insert(E inNum)
	{
		int index;
		int lastIndex;
		int toCompare;
		BiSearchTree<E> &t = *this;
		if (t.root == -1)
		{
			t.sll.append(Node<int>{inNum, -1, -1}, index);
			t.root = index;
		}
		else
		{
			lastIndex = -1;
			index = t.root;
			while (true) {
				toCompare = t.sll.s[index].e.e;
				if (inNum == toCompare)
				{
					return true;
				}
				lastIndex = index;
				if (inNum < toCompare)
				{
					index = t.sll.s[index].e.l;
				}
				else
				{
					index = t.sll.s[index].e.r;
				}
				if (index == -1)
					break;
			};
			t.sll.append(Node<int>{inNum, -1, -1}, index);
			if (inNum < toCompare)
			{
				t.sll.s[lastIndex].e.l = index;
			}
			else
			{
				t.sll.s[lastIndex].e.r = index;
			}
		}
		return false;
	}

	bool del(E inNum)
	{
		int index;
		int lastIndex;
		int toCompare;
		bool left;
		BiSearchTree<E> &t = *this;
		if (t.root == -1)
		{
			return false;
		}
		else
		{
			lastIndex = -1;
			index = t.root;
			while (true) {
				toCompare = t.sll.s[index].e.e;
				if (inNum == toCompare)
				{
					if (t.sll.s[index].e.l == -1 && t.sll.s[index].e.r == -1)
					{
						if (lastIndex != -1)
							if (left)
							{
								t.sll.s[lastIndex].e.l = -1;
							}
							else
							{
								t.sll.s[lastIndex].e.r = -1;
							}
						else
							this->root = -1;
					}
					else if (t.sll.s[index].e.l == -1)
					{
						if (lastIndex != -1)
							if (left)
							{
								t.sll.s[lastIndex].e.l = t.sll.s[index].e.r;
							}
							else
							{
								t.sll.s[lastIndex].e.r = t.sll.s[index].e.r;
							}
						else
							this->root = t.sll.s[index].e.r;
					}
					else if (t.sll.s[index].e.r == -1)
					{
						if (lastIndex != -1)
							if (left)
							{
								t.sll.s[lastIndex].e.l = t.sll.s[index].e.l;
							}
							else
							{
								t.sll.s[lastIndex].e.r = t.sll.s[index].e.l;
							}
						else
							this->root = t.sll.s[index].e.l;
					}
					else
					{
						int inOrderPrev = t.sll.s[index].e.l;
						int inOrderPrevPrev = -1;
						while (t.sll.s[inOrderPrev].e.r != -1)
						{
							inOrderPrevPrev = inOrderPrev;
							inOrderPrev = t.sll.s[inOrderPrev].e.r;
						}
						if (inOrderPrevPrev != -1)
						{
							t.sll.s[inOrderPrevPrev].e.r = t.sll.s[inOrderPrev].e.l;
							t.sll.s[inOrderPrev].e.l = t.sll.s[index].e.l;
							t.sll.s[inOrderPrev].e.r = t.sll.s[index].e.r;
						}
						else
						{
							t.sll.s[inOrderPrev].e.r = t.sll.s[index].e.r;
						}
						
						if (lastIndex != -1)
							if (left)
							{
								t.sll.s[lastIndex].e.l = inOrderPrev;
							}
							else
							{
								t.sll.s[lastIndex].e.r = inOrderPrev;
							}
						else
							this->root = inOrderPrev;
					}

					sll.delByIndex(index);
					return true;
				}
				lastIndex = index;
				if (inNum < toCompare)
				{
					index = t.sll.s[index].e.l;
					left = true;
				}
				else
				{
					index = t.sll.s[index].e.r;
					left = false;

				}
				if (index == -1)
					break;
			};
			return false;
		}
	}

	double printPreOrderTra()
	{
		return printPreOrderTra_do(this->root, 1, true);
	}

	double printPreOrderTra_do(int i, int count, bool first)
	{
		static int allSearchNum = 0;
		static int nodeNum = 0;
		if (first)
		{
			allSearchNum = 0;
			nodeNum = 0;
		}
		cout << sll.s[i].e.e << " ";
		allSearchNum += count;
		nodeNum++;
		if (sll.s[i].e.l != -1)
		{
			printPreOrderTra_do(sll.s[i].e.l, count+1, false);
		}
		if (sll.s[i].e.r != -1)
		{
			printPreOrderTra_do(sll.s[i].e.r, count+1, false);
		}
		return double(allSearchNum) / nodeNum;
	}

};

#define FS_INFILE
//#define FS_OUTFILE
int main()
{
#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	std::ifstream f0;
	f0.open("p2.txt", std::ios::in);
	auto cinbackup = std::cin.rdbuf();
	std::cin.set_rdbuf(f0.rdbuf());
#endif
	const int maxSize = 100000;
	size_t n;
	cin >> n;
	BiSearchTree<int> t(maxSize);
	int inNum;
	
	for (size_t i = 0; i < n; i++)
	{
		cin >> inNum;
		t.insert(inNum);
	}

	int delSearchNum;
	cin >> delSearchNum;
	if (t.del(delSearchNum))
	{
		cout << 1 << endl;
	}
	else
	{
		cout << 0 << endl;
	}

	if (t.insert(delSearchNum))
	{
		cout << 1 << endl;
	}
	else
	{
		cout << 0 << endl;
	}

	double avg = t.printPreOrderTra();
	cout << endl;
	cout << std::fixed << std::setprecision(2) << avg << endl;

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))

		std::ofstream f1;
		f1.open("out.txt", std::ios::out);
		auto coutbackup = std::cout.rdbuf();
		std::cout.set_rdbuf(f1.rdbuf());
#endif





#if(defined(_FS_DEBUG) && defined(FS_INFILE))
		f0.close();
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))
		f1.close();
#endif
		return 0;
	}