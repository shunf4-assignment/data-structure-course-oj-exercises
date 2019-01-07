/*1652270 CS2 fengshun*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <climits>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

typedef enum {STATOK, STATFULL, STATMISPLACE, STATEMPTY} status;

std::ostream& operator<<(std::ostream&out, const status &s)
{
	switch (s)
	{
		case STATOK:
			out << "正常"; break;
		case STATFULL:
			out << "表满"; break;
		case STATMISPLACE:
			out << "位置错"; break;
		case STATEMPTY:
			out << "表空"; break;
	}
	return out;
}

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
	//构造函数
	SLL(int size_)
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
	//判断表满
	bool isFull()
	{
		if (s[1].c == -1)
		{
			return true;
		}
		return false;
	}
	//判断表空
	bool isEmpty()
	{
		if (s[0].c == -1)
			return true;
		return false;
	}
	//清除表
	void clear()
	{
		if (tail != 0)
		{
			s[tail].c = s[1].c;
			s[1].c = s[0].c;
			s[0].c = -1;
			tail = 0;
		}
	}
	//增添
	status append(const E &e)
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
		return STATOK;
	}
	//插入
	status insert(int no, const E &e)
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

		return STATOK;
	}
	//删除
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
	//查找
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
	//取元素
	E& operator[](int pos)
	{
		int index = 0;
		for (int i = 0; i < pos + 1; i++)
		{
			index = s[index].c;
			if (index == -1)
			{
				throw STATMISPLACE;
			}
		}
		return s[index].e;
	}
	//替换
	status replace(int position, const E& e)
	{
		try {
			(*this)[position - 1] = e;
		}
		catch (status &s)
		{
			return s;
		}
		
		return STATOK;
		
	}
	//输入
	template <typename E_>
	friend std::ostream & operator <<(std::ostream& out, const SLL<E_>& sll);
	//打印状态
	void print (std::ostream& out) const
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
	//析构
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

class string_ : public std::string
{
public:
	string_() : string()
	{
		static_cast<string &>(*this) = string("N/A");
	}
};

template <typename E>
void inputTo(E &e)
{
	cin >> e;
	cout << e << endl;
}

#define inputTo(e) inputTo<decltype(e)>(e)


int main()
{
	using std::stringstream;
	stringstream s;
	cin.set_rdbuf(s.rdbuf());

	s << "9 5 Jan Feb Mar Apr May 2 Jun 7 Oct 4 1 Apr Jul Dec 1 1 Aug" << endl;
	s.seekg(std::ios::beg);

	int sllSize, itemCount;
	cout << "请输入要建立的静态链表空间大小：" << endl;
	inputTo(sllSize);
	cout << "请输入要建立的静态链表元素个数：" << endl;
	inputTo(itemCount);
	
	SLL<string_> sll(sllSize);
	string_ str;
	for (int i = 0; i < itemCount; i++)
	{
		cout << "请输入第 " << i + 1 << " 个元素：" << endl;
		inputTo(str);
		sll.append(str);
	}

	cout << "当前静态链表情况：" << endl;
	sll.print(cout);

	status stat;

	int insertNo;
	string_ insertStr;
	cout << "请输入要插入的位置、要插入的字符串：" << endl;
	inputTo(insertNo);
	inputTo(insertStr);
	stat = sll.insert(insertNo, insertStr);
	cout << "插入结果：" << endl << stat << endl;

	cout << sll << endl;
	sll.print(cout);

	cout << "（第二次）请输入要插入的位置、要插入的字符串：" << endl;
	inputTo(insertNo);
	inputTo(insertStr);
	stat = sll.insert(insertNo, insertStr);
	cout << "插入结果：" << endl << stat << endl;
	cout << sll << endl;
	sll.print(cout);

	int delNo;
	string_ delStr;
	cout << "请输入要删除的位置：" << endl;
	inputTo(delNo);
	stat = sll.del(delNo, delStr);
	cout << "结果：" << endl << stat << endl;
	cout << "已经删除的字符串：" << delStr << endl;
	cout << sll << endl;
	sll.print(cout);

	cout << "（第二次）请输入要删除的位置：" << endl;
	inputTo(delNo);
	stat = sll.del(delNo, delStr);
	cout << "结果：" << endl << stat << endl;
	cout << "已经删除的字符串：" << delStr << endl;
	cout << sll << endl;
	sll.print(cout);

	string_ searchStr;
	int searchNo;
	cout << "请输入要查找的字符串：" << endl;
	inputTo(searchStr);
	searchNo = sll.search(searchStr);
	cout << "若找到，返回第几个；否则输出-1：" << endl;
	cout << searchNo << endl;
	cout << "链表状态：" << endl;

	if (searchNo != -1)
	{
		cout << "（第三次）在前次查找结果前插入，请输入要插入的字符串：" << endl;
		inputTo(insertStr);
		stat = sll.insert(searchNo, insertStr);
		cout << "插入结果：" << endl << stat << endl;
		cout << sll << endl;
		sll.print(cout);

		cout << "（第四次）在前次查找结果前插入，请输入要插入的字符串：" << endl;
		inputTo(insertStr);
		stat = sll.insert(searchNo, insertStr);
		cout << "插入结果：" << endl << stat << endl;
		cout << sll << endl;
		sll.print(cout);
	}

	

	int retrieveNo;
	string_ retrieveStr;
	cout << "请输入要获取的元素位置（从0开始）：" << endl;
	inputTo(retrieveNo);
	retrieveStr = sll[retrieveNo];
	cout << "获取到的字符串：" << endl;
	cout << retrieveStr << endl;

	int replaceNo;
	string_ replaceStr;
	cout << "请输入要替换的元素位置（从1开始），要替换成的字符串：" << endl;
	inputTo(replaceNo);
	inputTo(replaceStr);
	stat = sll.replace(replaceNo, replaceStr);
	cout << "替换结果：" << endl;
	cout << stat << endl;
	cout << sll << endl;
	sll.print(cout);

	cout << "当前表空吗？" << endl << sll.isEmpty() << endl;
	cout << "当前表满吗？" << endl << sll.isFull() << endl;

	cout << "清除表" << endl;
	sll.clear();
	cout << sll << endl;
	sll.print(cout);

	cout << "当前表空吗？" << endl << sll.isEmpty() << endl;
	cout << "当前表满吗？" << endl << sll.isFull() << endl;

	return 0;
}

