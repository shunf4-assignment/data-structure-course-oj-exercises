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
			out << "����"; break;
		case STATFULL:
			out << "����"; break;
		case STATMISPLACE:
			out << "λ�ô�"; break;
		case STATEMPTY:
			out << "���"; break;
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
	//���캯��
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
	//�жϱ���
	bool isFull()
	{
		if (s[1].c == -1)
		{
			return true;
		}
		return false;
	}
	//�жϱ��
	bool isEmpty()
	{
		if (s[0].c == -1)
			return true;
		return false;
	}
	//�����
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
	//����
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
	//����
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
	//ɾ��
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
	//����
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
	//ȡԪ��
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
	//�滻
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
	//����
	template <typename E_>
	friend std::ostream & operator <<(std::ostream& out, const SLL<E_>& sll);
	//��ӡ״̬
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
	//����
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
	cout << "������Ҫ�����ľ�̬����ռ��С��" << endl;
	inputTo(sllSize);
	cout << "������Ҫ�����ľ�̬����Ԫ�ظ�����" << endl;
	inputTo(itemCount);
	
	SLL<string_> sll(sllSize);
	string_ str;
	for (int i = 0; i < itemCount; i++)
	{
		cout << "������� " << i + 1 << " ��Ԫ�أ�" << endl;
		inputTo(str);
		sll.append(str);
	}

	cout << "��ǰ��̬���������" << endl;
	sll.print(cout);

	status stat;

	int insertNo;
	string_ insertStr;
	cout << "������Ҫ�����λ�á�Ҫ������ַ�����" << endl;
	inputTo(insertNo);
	inputTo(insertStr);
	stat = sll.insert(insertNo, insertStr);
	cout << "��������" << endl << stat << endl;

	cout << sll << endl;
	sll.print(cout);

	cout << "���ڶ��Σ�������Ҫ�����λ�á�Ҫ������ַ�����" << endl;
	inputTo(insertNo);
	inputTo(insertStr);
	stat = sll.insert(insertNo, insertStr);
	cout << "��������" << endl << stat << endl;
	cout << sll << endl;
	sll.print(cout);

	int delNo;
	string_ delStr;
	cout << "������Ҫɾ����λ�ã�" << endl;
	inputTo(delNo);
	stat = sll.del(delNo, delStr);
	cout << "�����" << endl << stat << endl;
	cout << "�Ѿ�ɾ�����ַ�����" << delStr << endl;
	cout << sll << endl;
	sll.print(cout);

	cout << "���ڶ��Σ�������Ҫɾ����λ�ã�" << endl;
	inputTo(delNo);
	stat = sll.del(delNo, delStr);
	cout << "�����" << endl << stat << endl;
	cout << "�Ѿ�ɾ�����ַ�����" << delStr << endl;
	cout << sll << endl;
	sll.print(cout);

	string_ searchStr;
	int searchNo;
	cout << "������Ҫ���ҵ��ַ�����" << endl;
	inputTo(searchStr);
	searchNo = sll.search(searchStr);
	cout << "���ҵ������صڼ������������-1��" << endl;
	cout << searchNo << endl;
	cout << "����״̬��" << endl;

	if (searchNo != -1)
	{
		cout << "�������Σ���ǰ�β��ҽ��ǰ���룬������Ҫ������ַ�����" << endl;
		inputTo(insertStr);
		stat = sll.insert(searchNo, insertStr);
		cout << "��������" << endl << stat << endl;
		cout << sll << endl;
		sll.print(cout);

		cout << "�����ĴΣ���ǰ�β��ҽ��ǰ���룬������Ҫ������ַ�����" << endl;
		inputTo(insertStr);
		stat = sll.insert(searchNo, insertStr);
		cout << "��������" << endl << stat << endl;
		cout << sll << endl;
		sll.print(cout);
	}

	

	int retrieveNo;
	string_ retrieveStr;
	cout << "������Ҫ��ȡ��Ԫ��λ�ã���0��ʼ����" << endl;
	inputTo(retrieveNo);
	retrieveStr = sll[retrieveNo];
	cout << "��ȡ�����ַ�����" << endl;
	cout << retrieveStr << endl;

	int replaceNo;
	string_ replaceStr;
	cout << "������Ҫ�滻��Ԫ��λ�ã���1��ʼ����Ҫ�滻�ɵ��ַ�����" << endl;
	inputTo(replaceNo);
	inputTo(replaceStr);
	stat = sll.replace(replaceNo, replaceStr);
	cout << "�滻�����" << endl;
	cout << stat << endl;
	cout << sll << endl;
	sll.print(cout);

	cout << "��ǰ�����" << endl << sll.isEmpty() << endl;
	cout << "��ǰ������" << endl << sll.isFull() << endl;

	cout << "�����" << endl;
	sll.clear();
	cout << sll << endl;
	sll.print(cout);

	cout << "��ǰ�����" << endl << sll.isEmpty() << endl;
	cout << "��ǰ������" << endl << sll.isFull() << endl;

	return 0;
}

