/* 1652270 Computer_Class2 Feng Shun */
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <fstream>
using namespace std;

template <typename Elem>
class LinkQueue
{
private:
	Elem *list;
	Elem *front;
	Elem *rear;
public:
	unsigned long length;
	unsigned long capacity;

	LinkQueue(unsigned long capacity_)
	{
		list = new Elem[capacity_];
		capacity = capacity_;
		length = 0;
		front = list;
		rear = list;
	}

	~LinkQueue()
	{
		if(list != NULL)
			delete[]list;
	}

	bool enqueue(const Elem &e)
	{
		if (length == capacity)
			return false;
		if (length == 0)
		{
			//dont ++rear
			*rear = e;
		}
		else if (rear == list + capacity - 1)
		{
			rear = list;
			*rear = e;
		}
		else
		{
			*(++rear) = e;
		}
		length++;
		return true;
	}

	bool dequeue(Elem &receiver)
	{
		if (length == 0)
			return false;
		if (front == list + capacity - 1)
		{
			receiver = *front;
			front = list;
		}
		else
		{
			receiver = *(front++);
		}
		length--;
		if (length == 0)
		{
			front = rear;
		}
		return true;
	}

	void traverseInQueue(void(*func)(Elem &e, unsigned int i))
	{
		if (length == 0)
			return;
		unsigned count = 0;
		Elem* front2 = front;
		func(*front2, ++count);
		while (front2 != rear)
		{
			++front2;
			if (front2 == list + capacity)
			{
				front2 = list;
			}
			func(*front2, ++count);
		}
	}
};

void printNum(int &n, unsigned i)
{
	if (i != 1)
		cout << " ";
	cout << n;
}

int main()
{
#ifdef _FS_DEBUG
	FILE *f;
	fopen_s(&f, "p1.txt", "r");
	//freopen_s(&f, "p1.txt", "r", stdin);
	//ofstream f2;
	//f2.open("Output.txt", ios::out);
	//cout.set_rdbuf(f2.rdbuf());
#endif
	unsigned long n;
	cin >> n;
	LinkQueue<int> l(n);

	string cmd;

	while (true)
	{
		cin >> cmd;
		if (cmd == "quit")
		{
			l.traverseInQueue(printNum);
			break;
		}
		if (cmd == "dequeue")
		{
			int outQueue;
			if (l.dequeue(outQueue))
			{
				cout << outQueue;
				cout << endl;
			}
			else
			{
				cout << "Queue is Empty" << endl;
			}
		}
		if (cmd == "enqueue")
		{
			int inQueue;
			cin >> inQueue;
			if (l.enqueue(inQueue))
				;
			else
				cout << "Queue is Full" << endl;
		}
	}
#ifdef _FS_DEBUG
	fclose(f);
#endif
	return 0;
}