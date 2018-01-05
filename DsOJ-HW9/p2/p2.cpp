/*1652270 CS2 fengshun*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>


typedef enum { MINHEAP, MAXHEAP } HeapType;
typedef enum{LESS, EQUAL, GREATER} Compare;

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
				if (maxIndex == n || compare(arr[maxIndex] , arr[i]) == LESS)
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

	void cut_top()
	{
		if (n == 0)
			return;
		if (n == 1)
		{
			n = 0;
			return;
		}
		arr[0] = arr[--n];
		shiftDown(0);
	}

	~Heap()
	{
		if (arr)
			delete [] arr;
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

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))

	std::ofstream f1;
	f1.open("out.txt", std::ios::out);
	auto coutbackup = std::cout.rdbuf();
	std::cout.set_rdbuf(f1.rdbuf());
#endif

	using std::cin;
	using std::cout;

	size_t totalNum, needMax;
	cin >> totalNum >> needMax;
	Heap<int> h(MAXHEAP, needMax);

	int curNum;
	for (size_t i = 0; i < totalNum; i++)
	{
		cin >> curNum;
		h.insert_competition(curNum);
	}

	for (size_t i = 0; i < needMax; i++)
	{
		if (i) cout << ' ';
		cout << h.arr[0];
		h.cut_top();
	}

#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	f0.close();
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))
	f1.close();
#endif
	return 0;
}

