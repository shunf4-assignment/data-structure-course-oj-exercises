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
using std::stringstream;

#ifndef _FS_DEBUG
#define _FS_DEBUG
#endif

int searchIn(int num, int *arr, int start, int end)
{
	if (start == end)
	{
		if (num == arr[start])
			return start;
		else
			return -1;
	}

	int mid;
	mid = (start + end) / 2;
	//if (num == arr[mid])
	//	return mid;
	//else
	if (num <= arr[mid])
		return searchIn(num, arr, start, mid);
	else
		return searchIn(num, arr, mid + 1, end);
}

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
		while (true)
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
	//构造
	BiSearchTree(int size)
	{
		sll.init(size);
		root = -1;
	}
	//查找插入某元素
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
	//删除某元素
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
	//先序遍历输出
	double printPreOrderTra()
	{
		return printPreOrderTra_do(this->root, 1, true);
	}
	//先序遍历辅助函数
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
			printPreOrderTra_do(sll.s[i].e.l, count + 1, false);
		}
		else
		{
			cout << "#" << " ";
		}
		if (sll.s[i].e.r != -1)
		{
			printPreOrderTra_do(sll.s[i].e.r, count + 1, false);
		}
		else
		{
			cout << "#" << " ";
		}
		return double(allSearchNum) / nodeNum;
	}

};

template <typename E>
class Hash
{
public:
	struct HashItem {
		E data;
		size_t cnt;
		HashItem() {
			data = E();
			cnt = 0;
		}
	};

	HashItem *arr;
	size_t size;
	size_t(*hashFunc)(const E&e);
	//构造
	Hash(size_t size_, size_t(*hashFunc_)(const E&e))
	{
		arr = new HashItem[size_];
		size = size_;
		hashFunc = hashFunc_;
	}
	//插入值，返回迭代查找空位次数
	size_t insert_rLUCnt(const E&e, size_t &addrRecv)
	{
		size_t h, hBackup;
		h = hashFunc(e);
		h %= size;
		hBackup = h;
		size_t lookupCount = 1;

		if (arr[h].cnt == 0)
		{
			arr[h].cnt++;
			arr[h].data = e;
		}
		else
		{
			//Collision
#ifdef _FS_DEBUG
			std::cout << "发生冲突：" << endl;
			std::cout << h;
#endif
			while ((arr[h].data != e && arr[h].cnt != 0))
			{
				h += 1;
				h %= size;
				lookupCount++;
#ifdef _FS_DEBUG
				std::cout << " == （" << lookupCount << "） ==> " << h;
#endif
				if (hBackup == h)
					break;
			}
#ifdef _FS_DEBUG
			std::cout << std::endl;
#endif
			if (lookupCount != 1 && hBackup == h)
			{
				throw "CANNOT FIND A PROPER PLACE";
			}
			else
			{
				if (arr[h].cnt == 0)
				{
					arr[h].cnt++;
					arr[h].data = e;
				}
				else
				{
					arr[h].cnt++;
				}
			}
		}


		addrRecv = h;
		return lookupCount;
	}
	//搜索值，返回迭代查找空位次数
	size_t search_rLUCnt(const E&e, size_t &addrRecv) //wrong
	{
		size_t h, hBackup;
		h = hashFunc(e);
		h %= size;
		hBackup = h;
		size_t lookupCount = 1;
		if (arr[h].data != e)
		{
			//Collision
			while ((arr[h].data != e))
			{
				h += 1;
				h %= size;
				lookupCount++;
				if (hBackup == h && lookupCount != 1)
					break;
			}
			if (hBackup == h)
			{
				//throw "CANNOT FIND";
				return SIZE_MAX;
			}
			else
			{
				addrRecv = h;
			}
		}
		else
		{
			addrRecv = h;
		}
		return lookupCount;
	}

	~Hash()
	{
		delete[]arr;
	}
};

//对string的hash函数
size_t hashString(const std::string &s)
{
	size_t base = 37;
	size_t currBase = 1;
	size_t result = 0;
	for (auto p = s.cend(); p != s.cbegin(); )
	{
		p--;
		result += static_cast<size_t>(*p) * currBase;
		currBase *= base;
	}
	return result;
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

	s << "10 \
		1 2 2 3 3 4 5 5 5 5 \
		5" << endl;
	s << "10 \
		3263 12082 8535 26651 32548 28478 22980 6755 1502 29078 \
		12082" << endl;
	s << "4 11 \
		A B C L L A" << endl;
	s.seekg(std::ios::beg);

	if (true)
	{
		cout << " == 第一部分 折半查找 ==" << endl;
		int n;
		cout << ("请输入要输入的元素个数") << endl;
		inputTo(n);
		int *arr;
		arr = (int *)malloc(n * sizeof(int));
		if (arr == NULL)
			return 0;

		for (int i = 0; i < n; i++)
		{
			cout << "请输入第 " << i+1 <<" 个元素：" << endl;
			inputTo(arr[i]);
		}

		int num;
		cout << "请输入要查找的元素" << endl;
		inputTo(num);

		cout << "元素在第 " << searchIn(num, arr, 0, n - 1) << " 位置" << endl;
	}

	if (true)
	{
		cout << " == 第二部分 二叉查找树 ==" << endl;
		const int maxSize = 100000;
		size_t n;
		cout << "请输入树中待排序元素个数：" << endl;
		inputTo(n);
		BiSearchTree<int> t(maxSize);
		int inNum;

		for (size_t i = 0; i < n; i++)
		{
			cout << "请输入第 " << i + 1 << " 个元素：" << endl;
			inputTo(inNum);
			t.insert(inNum);
		}

		cout << "当前先序遍历的序列：" << endl;
		t.printPreOrderTra();
		cout << endl;
		int delSearchNum;
		cout << "请输入待删除、之后再次查找插入元素：" << endl;
		inputTo(delSearchNum);
		if (t.del(delSearchNum))
		{
			cout << "成功" << endl;
			cout << "当前先序遍历的序列：" << endl;
			t.printPreOrderTra();
			cout << endl;

		}
		else
		{
			cout << "失败" << endl;
		}

		if (t.insert(delSearchNum))
		{
			cout << "找到" << endl;
			cout << "当前先序遍历的序列：" << endl;
			t.printPreOrderTra();
			cout << endl;

		}
		else
		{
			cout << "查找失败，已经插入" << endl;
			cout << "当前先序遍历的序列：" << endl;
			t.printPreOrderTra();
			cout << endl;

		}
	}

	if (true)
	{
		cout << " == 第三部分 哈希表 ==" << endl;
		size_t keyNum, hashSize;
		cout << "依次输入项的个数和哈希表的容量：" << endl;
		inputTo(keyNum); inputTo(hashSize);
		Hash<string> h(hashSize, hashString);
		string curStr;
		size_t *addr = new size_t[hashSize];
		size_t *findCnt = new size_t[hashSize];
		for (size_t i = 0; i < keyNum; i++)
		{
			cout << "请输入第 " << i + 1 << " 个元素：" << endl; 
			inputTo(curStr);
			findCnt[i] = h.insert_rLUCnt(curStr, addr[i]);
		}

		cout << "输入的各个元素所在地址：" << endl;
		for (size_t i = 0; i < keyNum; i++)
		{
			if (i)
				cout << ' ';
			cout << addr[i];
		}
		cout << endl;
		cout << "输入的各个元素被查找的次数：" << endl;
		for (size_t i = 0; i < keyNum; i++)
		{
			if (i)
				cout << ' ';
			cout << findCnt[i];
		}
		cout << endl;

		cout << "请输入要查找的元素：" << endl;
		size_t currAddr, currFindCnt;
		inputTo(curStr);
		currFindCnt = h.search_rLUCnt(curStr, currAddr);
		cout << "查找了 " << currFindCnt << " 次，地址为 " << currAddr << " ，该地址里的数据 " << h.arr[currAddr].data << endl;

		cout << "（第二次）请输入要查找的元素：" << endl;
		inputTo(curStr);
		currFindCnt = h.search_rLUCnt(curStr, currAddr);
		cout << "查找了 " << currFindCnt << " 次，地址为 " << currAddr << " ，该地址里的数据 " << h.arr[currAddr].data << endl;

		delete[]addr;
		delete[]findCnt;
	}

	return 0;
}