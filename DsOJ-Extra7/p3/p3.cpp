/*1652270 CS2 fengshun*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <climits>
#include <cassert>
using std::cin;
using std::cout;
using std::endl;
using std::string;


typedef enum { MINHEAP, MAXHEAP } HeapType;
typedef enum { LESS, EQUAL, GREATER } Compare;

const size_t defaultHeapCapacity = 10;


template <typename E>
class Heap
{
public:
	E* arr;
	size_t capacity;
	size_t n;
	HeapType type;

	Compare compare(const E& e1, const E& e2)
	{
		if (type == MINHEAP)
		{
			return e1 > e2 ? GREATER : e1 == e2 ? EQUAL : LESS;
		}
		else
		{
			return e1 < e2 ? GREATER : e1 == e2 ? EQUAL : LESS;
		}
	}

	Heap(HeapType t = MINHEAP, size_t hc = defaultHeapCapacity)
	{
		type = t;
		capacity = hc;
		arr = new E[hc];
		n = 0;
	}

	void swap(size_t i1, size_t i2)
	{
		E tmp;
		tmp = arr[i1];
		arr[i1] = arr[i2];
		arr[i2] = tmp;
	}

	void insert(const E& e)
	{
		if (n == capacity)
		{
			throw "CAPACITY NOT ENOUGH";
		}
		arr[n++] = e;
		shiftUp();
	}

	void insert_competition(const E& e)
	{
		size_t maxIndex = n;
		if (n == capacity)
		{
			for (size_t i = n - 1; i > n / 2 - 1; i--)
			{
				if (maxIndex == n || compare(arr[maxIndex], arr[i]) == LESS)
					maxIndex = i;
			}

			if (compare(arr[maxIndex], e) == GREATER)
			{
				arr[maxIndex] = e;
				shiftUp(maxIndex);
			}

		}
		else
		{
			arr[n++] = e;
			shiftUp();
		}


	}

	void shiftDown(size_t from)
	{
		if (n == 0)
			return;
		size_t cur = from;
		size_t minNode;
		E curE = arr[cur];
		while (cur * 2 + 1 < n)
		{
			minNode = cur * 2 + 1;
			if (cur * 2 + 2 < n && compare(arr[cur * 2 + 2], arr[cur * 2 + 1]) == LESS)
				minNode++;

			if (compare(arr[minNode], curE) == LESS)
			{
				arr[cur] = arr[minNode];
				cur = minNode;
			}
			else break;
		}
		arr[cur] = curE;
	}

	void shiftUp(size_t i = SIZE_MAX)
	{
		if (n == 0)
			return;
		size_t curparent, cur;
		if (i == SIZE_MAX)
		{
			curparent = n / 2 - 1;
			cur = n - 1;
		}
		else
		{
			curparent = (i - 1) / 2;
			cur = i;
		}

		E curE = arr[cur];
		while (cur > 0 && curparent >= 0)
		{
			if (compare(arr[curparent], curE) == GREATER)
			{
				arr[cur] = arr[curparent];
				cur = curparent;
				curparent = (cur - 1) / 2;
			}
			else break;
		}
		arr[cur] = curE;
	}

	void print(std::ostream &out)
	{
		for (size_t i = 0; i < n; i++)
		{
			if (i != 0)
				out << " ";

			out << arr[i];
		}
		out << std::endl;
	}

	E cut_top()
	{
		if (n == 0)
			throw "HEAP EMPTY";
		if (n == 1)
		{
			n = 0;
			return arr[0];
		}
		swap(0, --n);
		shiftDown(0);
		return arr[n];
	}

	~Heap()
	{
		if (arr)
			delete[] arr;
	}
};

typedef enum { ISLEFTCHILD, ISRIGHTCHILD } childType;

template <typename E>
struct HuffNode
{
	childType ct;
	size_t parent;
	size_t lchild;
	size_t rchild;
	size_t index;
	E e;

	HuffNode() = default;

	bool operator==(const HuffNode<E> &h2)const { return (e == h2.e); }
	bool operator>(const HuffNode<E> &h2)const { return (e > h2.e); }
	bool operator<(const HuffNode<E> &h2)const { return (e < h2.e); }
	bool operator>=(const HuffNode<E> &h2)const { return (e >= h2.e); }
	bool operator<=(const HuffNode<E> &h2)const { return (e <= h2.e); }

	HuffNode(HuffNode<E> &h1, HuffNode<E> &h2, size_t myIndex)
	{
		beParent(&h1, &h2, myIndex);
		e = h1.e + h2.e;
	}

	void beParent(HuffNode<E> *h1, HuffNode<E> *h2, size_t myIndex)
	{
		if (h1)
		{
			lchild = h1->index;
			h1->parent = myIndex;
			h1->ct = ISLEFTCHILD;
		}
		if (h2)
		{
			rchild = h2->index;
			h2->parent = myIndex;
			h2->ct = ISRIGHTCHILD;
		}
		index = myIndex;
	}
};

template <typename E>
struct HuffTree
{
	HuffNode<E> *body;
	size_t size;
	size_t n;
	size_t rootI;
	string *codes;

	HuffTree()
	{
		body = NULL;
		codes = NULL;
	}

	void init(size_t n_, size_t size_)
	{
		size = size_;
		n = n_;
		if (body)
			delete[]body;
		if (codes)
			delete[]codes;
		body = new HuffNode<E>[size];
		codes = new string[n_]{};
	}

	~HuffTree()
	{
		if (body)
			delete[]body;
		if (codes)
			delete[]codes;
	}

	E build(E *seq, size_t n)
	{
		init(n, 2 * n + 1 + 1);
		Heap<HuffNode<E>> h(MINHEAP, size);
		E weightedSum = 0;
		for (size_t i = 0; i < size; i++)
		{
			body[i].ct = ISLEFTCHILD;
			body[i].parent = SIZE_MAX;
			body[i].rchild = SIZE_MAX;
			body[i].lchild = SIZE_MAX;
			body[i].index = i;

			if (i < n)
			{
				body[i].e = seq[i];
				weightedSum += seq[i];
				h.insert(body[i]);
			}
		}
		HuffNode<E> s1;
		HuffNode<E> s2;
		size_t newHNI = n;
		while (h.n > 1)
		{
			s1 = h.cut_top();
			s2 = h.cut_top();
			assert(newHNI < 2 * n - 1);
			body[newHNI] = HuffNode<E>(s1, s2, newHNI);
			if (newHNI != 2 * n - 2)
				weightedSum += body[newHNI].e;
			h.insert(body[newHNI]);
			newHNI++;
		}
		assert(newHNI == 2 * n - 1);

		return weightedSum;
	}

	void genCodes()
	{
		for (size_t i = 0; i < n; i++)
		{
			size_t curr = i;
			while (curr != SIZE_MAX)
			{
				if (body[curr].parent != SIZE_MAX)
					codes[i] = static_cast<char>(body[curr].ct + '0') + codes[i];
				else
					rootI = curr;
				curr = body[curr].parent;
			}
		}
	}

	template <typename E_>
	friend std::istream & operator >> (std::istream &in, HuffTree<E_>& h);

	void printAllCodes(std::ostream& out) const
	{
		for (size_t i = 0; i < n; i++)
		{
			out << body[i].e << " " << codes[i] << endl;
		}
	}

	string decode(const string &enStr) const
	{
		size_t currI = rootI;
		string result = "";
		for (auto p = enStr.cbegin(); p != enStr.cend(); p++)
		{
			if (*p == '0')
			{
				currI = body[currI].lchild;
			}
			else if (*p == '1')
			{
				currI = body[currI].rchild;
			}
			if (currI < n)
			{
				result += string(body[currI].e);
				currI = rootI;
			}
		}
		return result;
	}
};


template <typename E>
std::istream & operator >> (std::istream &in, HuffTree<E>& h)
{
	size_t n;
	in >> n;

	h.init(n, 2 * n + 1 + 1);
	for (size_t i = 0; i < h.size; i++)
	{
		h.body[i].ct = ISLEFTCHILD;
		h.body[i].parent = SIZE_MAX;
		h.body[i].lchild = SIZE_MAX;
		h.body[i].rchild = SIZE_MAX;
		h.body[i].index = SIZE_MAX;
	}
	E e;
	string code;
	size_t rootI = n;
	size_t currI = n;
	size_t freeI = n + 1;
	for (size_t i = 0; i < n; i++)
	{

		in >> e;
		in >> code;
		currI = rootI;
		for (auto p = code.cbegin(); p != code.cend(); p++)
		{
			if (*p == '0')
			{
				if (h.body[currI].lchild == SIZE_MAX)
				{
					if (p + 1 == code.cend())
					{
						h.body[i].index = i;
						h.body[currI].beParent(&h.body[i], NULL, currI);
					}
					else
					{
						h.body[freeI].index = freeI;
						h.body[currI].beParent(&h.body[freeI++], NULL, currI);
					}
				}
				currI = h.body[currI].lchild;
			}
			else if (*p == '1')
			{
				if (h.body[currI].rchild == SIZE_MAX)
				{
					h.body[freeI].index = freeI;
					if (p + 1 == code.cend())
					{
						h.body[i].index = i;
						h.body[currI].beParent(NULL, &h.body[i], currI);
					}
					else
					{
						h.body[i].index = freeI;
						h.body[currI].beParent(NULL, &h.body[freeI++], currI);
					}
				}
				currI = h.body[currI].rchild;
			}
			h.body[currI].e = e;
		}
	}
	return in;
}

struct char_
{
	char c;
	char_()
	{
		c = '\0';
	}

	operator char&()
	{
		return c;
	}

	char_(const char& d)
	{
		c = d;
	}

	operator string()
	{
		return string("") + c;
	}
};

std::istream& operator >> (std::istream& in, char_& c)
{
	while (in.peek() == '\n' || in.peek() == '\r')
		in.get();
	c = in.get();
	return in;
}

#define FS_INFILE
//#define FS_OUTFILE
int main()
{
#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	std::ifstream f0;
	f0.open("p3.txt", std::ios::in);
	auto cinbackup = std::cin.rdbuf();
	std::cin.set_rdbuf(f0.rdbuf());
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))

	std::ofstream f1;
	f1.open("out.txt", std::ios::out);
	auto coutbackup = std::cout.rdbuf();
	std::cout.set_rdbuf(f1.rdbuf());
#endif
	HuffTree<char_> huff;
	cin >> huff;
	huff.genCodes();
	//huff.printAllCodes(cout);
	string encodedStr;
	cin >> encodedStr;
	string decodedStr = huff.decode(encodedStr);
	cout << decodedStr << endl;

#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	f0.close();
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))
	f1.close();
#endif
	return 0;
}

