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
class PointerHeap
{
public:
	E** arr;
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

	PointerHeap(HeapType t = MINHEAP, size_t hc = defaultHeapCapacity)
	{
		type = t;
		capacity = hc;
		arr = new E*[hc];
		n = 0;
	}

	void swap(size_t i1, size_t i2)
	{
		E *tmp;
		tmp = arr[i1];
		arr[i1] = arr[i2];
		arr[i2] = tmp;
	}

	void insert(E& e)
	{
		if (n == capacity)
		{
			throw "CAPACITY NOT ENOUGH";
		}
		arr[n++] = &e;
		shiftUp();
	}

	void insert_competition(const E& e)
	{
		size_t maxIndex = n;
		if (n == capacity)
		{
			for (size_t i = n - 1; i > n / 2 - 1; i--)
			{
				if (maxIndex == n || compare(*arr[maxIndex], *arr[i]) == LESS)
					maxIndex = i;
			}

			if (compare(*arr[maxIndex], e) == GREATER)
			{
				arr[maxIndex] = &e;
				shiftUp(maxIndex);
			}

		}
		else
		{
			arr[n++] = &e;
			shiftUp();
		}


	}

	void shiftDown(size_t from)
	{
		if (n == 0)
			return;
		size_t cur = from;
		size_t minNode;
		E* curE = arr[cur];
		while (cur * 2 + 1 < n)
		{
			minNode = cur * 2 + 1;
			if (cur * 2 + 2 < n && compare(*arr[cur * 2 + 2], *arr[cur * 2 + 1]) == LESS)
				minNode++;

			if (compare(*arr[minNode], *curE) == LESS)
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

		E *curE = arr[cur];
		while (cur > 0 && curparent >= 0)
		{
			if (compare(*arr[curparent], *curE) == GREATER)
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

	E* cut_top()
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

	~PointerHeap()
	{
		if (arr)
			delete[] arr;
	}
};

typedef enum { ISLEFTCHILD, ISRIGHTCHILD } childType;

//需要将所有的E都封装成一个具有成员w表示权值的类
template <typename E>
struct HuffNode
{
	childType ct;
	size_t parent;
	size_t lchild;
	size_t rchild;
	size_t index;
	E e;

	//无参数构造
	HuffNode() { init(); }
	//初始化，给成员赋初值SIZE_MAX
	void init()
	{
		ct = ISLEFTCHILD;
		parent = SIZE_MAX;
		lchild = SIZE_MAX;
		rchild = SIZE_MAX;
		index = SIZE_MAX;
	}
	//判断大于、小于、相等，实际就是比较E的权值成员e.w
	bool operator==(const HuffNode<E> &h2)const { return (e.w == h2.e.w); }
	bool operator>(const HuffNode<E> &h2)const { return (e.w > h2.e.w); }
	bool operator<(const HuffNode<E> &h2)const { return (e.w < h2.e.w); }
	bool operator>=(const HuffNode<E> &h2)const { return (e.w >= h2.e.w); }
	bool operator<=(const HuffNode<E> &h2)const { return (e.w <= h2.e.w); }
	//构造，同时让自己成为h1和h2的父亲，权值相加 赋值给自己
	HuffNode(HuffNode<E> &h1, HuffNode<E> &h2, size_t myIndex)
	{
		init();
		beParent(&h1, &h2, myIndex);
		e.w = h1.e.w + h2.e.w;
	}
	//让自己成为h1和h2的父亲
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
	//树的构造
	HuffTree()
	{
		body = NULL;
		codes = NULL;
	}
	//初始化空间
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
	//析构
	~HuffTree()
	{
		if (body)
			delete[]body;
		if (codes)
			delete[]codes;
	}

	//从一个元素E序列构造出哈夫曼树，并返回最短加权路径值
	using W = decltype(E::w);
	W build(E *seq, size_t n)
	{
		init(n, 2 * n + 1 + 1);
		PointerHeap<HuffNode<E>> h(MINHEAP, size);
		W weightedSum = 0;
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
				weightedSum += seq[i].w;
				h.insert(body[i]);
			}
		}
		HuffNode<E> *s1;
		HuffNode<E> *s2;
		size_t newHNI = n;
		while (h.n > 1)
		{
			s1 = h.cut_top();
			s2 = h.cut_top();
			assert(newHNI < 2 * n - 1);
			body[newHNI] = HuffNode<E>(*s1, *s2, newHNI);
			if (newHNI != 2 * n - 2)
				weightedSum += body[newHNI].e.w;
			h.insert(body[newHNI]);
			newHNI++;
		}
		assert(newHNI == 2 * n - 1);

		return weightedSum;
	}
	//从已经构建的二叉树生成01前缀码，存在自身code成员
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
	//直接输入各元素和前缀码生成哈夫曼树
	template <typename E_>
	friend std::istream & operator >> (std::istream &in, HuffTree<E_>& h);
	//打印码表。注意，只能在genCodes之后调用
	void printAllCodes(std::ostream& out) const
	{
		for (size_t i = 0; i < n; i++)
		{
			out << body[i].e << " " << codes[i] << endl;
		}
	}
	//编码。注意，只能在genCodes之后调用
	string encode(const decltype(E::e) *str, size_t len) const
	{
		string encodedStr = "";
		for (size_t i = 0; i < len; i++)
		{
			size_t found = SIZE_MAX;
			for (size_t j = 0; j < n; j++)
			{
				if (body[j].e.e == str[i]){
					found = j;
					break;
				}
			}
			if (found == SIZE_MAX)
				return encodedStr;
			encodedStr += codes[found];
		}
		return encodedStr;
	}
	//解码。注意，只能在genCodes之后调用
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
	cout << n << endl;

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
		cout << e << " ";
		in >> code;
		cout << code << endl;
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

struct char_	//带权、可转换为string的char，封装被huffTree使用
{
	char e;
	double w;		//weight
	char_()
	{
		e = '\0';
	}

	operator char&()
	{
		return e;
	}

	char_(const char& d)
	{
		e = d;
	}

	operator string()
	{
		return string("") + e;
	}

	bool operator==(const char_& x) const { return e == x.e; }
};

std::istream& operator >> (std::istream& in, char_& c)
{
	while (in.peek() == '\n' || in.peek() == '\r')
		in.get();
	c.e = in.get();
	return in;
}

std::ostream& operator << (std::ostream& out, char_& c)
{
	out << c.e << "," << c.w;
	return out;
}

template <typename E>
void inputTo(E &e)
{
	cin >> e;
	cout << e << endl;
}

#define inputTo(e) inputTo<decltype(e)>(e)

#include<sstream>
using std::stringstream;
int main()
{
	stringstream s;
	cin.set_rdbuf(s.rdbuf());

	s << "10 \
		A 23.5 \
		B 11.3 \
		C 45.4 \
		D 33.3 \
		E 89.7 \
		F 23.4 \
		G 8.7 \
		H 17.0 \
		I 22.0 \
		J 3 \
		AIJCGCABBAGE \
		" << endl;

	s << "23\n  111\n. 010011\
\nI 0111110\
\nS 011110\
\na 1000\
\nb 11010\
\nc 10011\
\nd 10110\
\ne 0010\
\nf 01000\
\nh 01110\
\ni 11011\
\nl 1010\
\nm 0111111\
\nn 10010\
\no 000\
\nr 0110\
\ns 0101\
\nt 11001\
\nu 11000\
\nv 010010\
\nw 0011\
\ny 10111\
	01111001110000001111110111000110000110111010001010000001110011011101000011011001010111110001001010110111100110001001010011001010001010111101110001100001101111100110001101010100010010111110001001010110111011111011100111101110101010111110100010111011111110111010111001110110100011011001010110010011111011110011100000011111101110001100001101111100110001101010100010010111110001001010110111101110001100001101110100010100000011100110111010000110110010101111001111011101010101111101000101110001101001001011011000110000101010011" << endl;
	s.seekg(std::ios::beg);

	if (1)
	{
		cout << " === 第一部分 哈夫曼树的建立、编码、解码 ===" << endl;
		cout << "请输入哈夫曼树的叶子个数：" << endl;
		size_t n;
		inputTo(n);
		char_ *chars = new char_[n];
		for (size_t i = 0; i < n; i++)
		{
			cout << "请输入第 " << i + 1 << " 个叶子节点的值（char）和权（double）：" << endl;
			inputTo(chars[i].e);
			inputTo(chars[i].w);
		}

		HuffTree<char_> huff;
		cout << "该哈夫曼树的最短带权路径长度：" << endl;
		cout << huff.build(chars, n) << endl;
		cout << "该哈夫曼树码表：" << endl;
		huff.genCodes();
		huff.printAllCodes(cout);
		cout << endl;

		cout << "请输入要用哈夫曼编码的字符串：" << endl;
		string toEn;
		inputTo(toEn);
		string encodedStr = huff.encode(toEn.c_str(), toEn.length());
		cout << "编码后的字符串：" << endl;
		cout << encodedStr << endl;
		cout << "再对该字符串进行解码：" << endl;
		string decodedStr = huff.decode(encodedStr);
		cout << decodedStr << endl;

		delete[]chars;
	}

	if (1)
	{
		cout << " === 第二部分 哈夫曼码表的读入，和译码 ===" << endl;
		cout << "请先输入哈夫曼码表的长度，再输入码表：" << endl;
		HuffTree<char_> huff;
		cin >> huff;
		huff.genCodes();
		//huff.printAllCodes(cout);
		cout << "请输入待解码字符串：" << endl;
		string encodedStr;
		inputTo(encodedStr);
		string decodedStr = huff.decode(encodedStr);
		cout << "解码后字符串：" << endl;
		cout << decodedStr << endl;
	}
	return 0;
}

