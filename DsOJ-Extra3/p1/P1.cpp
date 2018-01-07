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

typedef enum {STATOK, STATFULL, STATMISPLACE, STATEMPTY} status;

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

#define FS_INFILE
//#define FS_OUTFILE
int main()
{
#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	std::ifstream f0;
	f0.open("p1.txt", std::ios::in);
	auto cinbackup = std::cin.rdbuf();
	std::cin.set_rdbuf(f0.rdbuf());
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))

	std::ofstream f1;
	f1.open("out.txt", std::ios::out);
	auto coutbackup = std::cout.rdbuf();
	std::cout.set_rdbuf(f1.rdbuf());
#endif

	int sllSize, itemCount;
	cin >> sllSize >> itemCount;
	SLL<string_> sll(sllSize);
	string_ str;
	for (int i = 0; i < itemCount; i++)
	{
		cin >> str;
		sll.append(str);
	}
	sll.print(cout);

	status stat;

	int insertNo;
	string_ insertStr;
	cin >> insertNo >> insertStr;
	stat = sll.insert(insertNo, insertStr);

	switch (stat)
	{
		case STATOK:
			cout << sll << endl;
			break;
		case STATFULL:
			cout << "FULL" << endl;
			break;
		case STATMISPLACE:
			cout << -1 << endl;
			break;
	}

	int delNo;
	string_ delStr;
	cin >> delNo;
	stat = sll.del(delNo, delStr);

	switch (stat)
	{
		case STATOK:
			cout << delStr << endl;
			break;
		case STATEMPTY:
			cout << "EMPTY" << endl;
			break;
		case STATMISPLACE:
			cout << -1 << endl;
			break;
	}
	
	string_ searchStr;
	int searchNo;
	cin >> searchStr;
	searchNo = sll.search(searchStr);
	cout << searchNo << endl;

	string_ appendStr;
	cin >> appendStr;
	stat = sll.append(appendStr);
	switch (stat)
	{
		case STATOK:
			cout << sll << endl;
			break;
		case STATFULL:
			cout << "FULL" << endl;
			break;
	}

	sll.print(cout);

#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	f0.close();
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))
	f1.close();
#endif
	return 0;
}

