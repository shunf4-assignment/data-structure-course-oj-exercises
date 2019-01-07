/*1652270 CS2 fengshun*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <windows.h>
using namespace std;
//�����ã���װָ��
template <typename E>
struct Pointer
{
	E *p;

	bool operator==(const Pointer<E>& p2) const { return (*p) == (*(p2.p)); }
	bool operator!=(const Pointer<E>& p2) const { return (*p) != (*(p2.p)); }
	bool operator>=(const Pointer<E>& p2) const { return (*p) >= (*(p2.p)); }
	bool operator<=(const Pointer<E>& p2) const { return (*p) <= (*(p2.p)); }
	bool operator>(const Pointer<E>& p2) const { return (*p) > (*(p2.p)); }
	bool operator<(const Pointer<E>& p2) const { return (*p) < (*(p2.p)); }

	Pointer() = default;

	Pointer(E *p_)
	{
		p = p_;
	}

	E& operator*()
	{
		return *p;
	}
};

template <typename E>
void brieflyView(Pointer<E>* arr, size_t len)
{
	if (len < 100)
	{
		cout << "�����У�" << endl;

		for (size_t i = 0; i < len; i++)
		{
			cout << *arr[i] << " ";
		}
		cout << endl;
	}
	else
	{
		cout << "�����е�ǰ������������" << endl;
		size_t lenLog = size_t(3*log2l(len));
		for (size_t i = 0; i < lenLog; i++)
		{
			cout << *arr[i] << " ";
		}
		cout << endl;
		cout << "����" << endl;
		for (size_t i = 0; i < lenLog; i++)
		{
			cout << *arr[len - lenLog + i] << " ";
		}
		cout << endl;

	}
}

//�ж��Ƿ������
template <typename E>
void checkSorted(Pointer<E>* arr, size_t len)
{
	size_t checkCnt = 0;
	size_t failCnt = 0;
	size_t randPos1, randPos2;

	size_t checkTime = len / 3;
	if (len > 0 && checkTime < 5) checkTime = 5;

	for (size_t i = 0; i < checkTime; i++)
	{
		randPos1 = (rand() * rand()) % len;
		randPos2 = (rand() * rand()) % len;
		E num1 = *arr[randPos1];
		E num2 = *arr[randPos2];
		checkCnt++;
		if (randPos1 < randPos2)
		{
			if (num1 > num2)
				failCnt++;
		}
		else if (randPos1 > randPos2)
		{
			if (num1 < num2)
				failCnt++;
		}
	}
	
	cout << "���ι���� " << checkCnt << " �Σ��������� " << failCnt << " �Ρ�" << endl;
	if (failCnt == 0)
		cout << "����Ϊ������������ġ�" << endl;
	else
		cout << "������û����á�" << endl;
}

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
	//�����жϺ������������С�Ѻ����ѵ��������
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
	//����
	void swap(size_t i1, size_t i2)
	{
		E tmp;
		tmp = arr[i1];
		arr[i1] = arr[i2];
		arr[i2] = tmp;
	}
	//����
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
	//�µ�
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
	//�ϵ�
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
	//��ӡ
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
	//ȥ��
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

//������������������
void randGen(int * arr, size_t totalNum)
{
	for (size_t i = 0; i < totalNum; i++)
	{
		arr[i] = static_cast<int>((rand() + rand() - rand() - rand()) * log10l(abs(rand())+1));
	}
}
//��������
template <typename E>
void insertionSort(Pointer<E> *sortedArr, size_t totalNum)
{
	size_t i, j;
	size_t cmpCnt = 0, movCnt = 0;
	for (i = 1; i < totalNum; i++)
	{
		auto x = sortedArr[i];
		for (j = 0; j < i; j++)
		{
			if (cmpCnt++, x <= sortedArr[j]) break;
		}
		for (size_t k = j; k < i; k++)
		{
			movCnt++, sortedArr[i - k + j] = sortedArr[i - k + j - 1];
		}
		sortedArr[j] = x;
	}
	cout << "�Ƚϴ�����" << cmpCnt << endl;
	cout << "�ƶ�������" << movCnt << endl;
}
//ѡ������
template <typename E>
void selectionSort(Pointer<E> *sortedArr, size_t totalNum)
{
	size_t i, j;
	Pointer<E> tmp;
	E minE;
	size_t minI;
	size_t cmpCnt = 0, movCnt = 0;

	for (i = 0; i < totalNum; i++)
	{
		minI = i;
		minE = *sortedArr[i];
		for (j = i + 1; j < totalNum; j++)
		{
			if (++cmpCnt, *sortedArr[j] < minE)
			{
				minI = j;
				minE = *sortedArr[j];
			}
		}
		movCnt += 2;
		tmp = sortedArr[i];
		sortedArr[i] = sortedArr[minI];
		sortedArr[minI] = tmp;
	}

	cout << "�Ƚϴ�����" << cmpCnt << endl;
	cout << "�ƶ�������" << movCnt << endl;
}
//ð������
template <typename E>
void bubbleSort(Pointer<E> *sortedArr, size_t totalNum)
{
	size_t i, j;
	Pointer<E> tmp;
	bool everChanged;
	size_t cmpCnt = 0, movCnt = 0;
	
	for (i = 0; i < totalNum; i++)
	{
		everChanged = false;
		for (j = totalNum - 1; j > i; j--)
		{
			if (++cmpCnt, sortedArr[j] < sortedArr[j - 1])
			{
				if (!everChanged) everChanged = true;
				movCnt += 2;
				tmp = sortedArr[j];
				sortedArr[j] = sortedArr[j - 1];
				sortedArr[j - 1] = tmp;
			}
		}
		if (!everChanged)
			break;
	}
	cout << "�Ƚϴ�����" << cmpCnt << endl;
	cout << "�ƶ�������" << movCnt << endl;
}
//�鲢����
template <typename E>
void mergeSort_do(Pointer<E> *sortedArr, size_t begI, size_t endI, size_t &cmpCnt, size_t &movCnt)
{
	if (endI - begI <= 1)
		return;
	Pointer<E> *newArr = new Pointer<E>[endI - begI];

	size_t midI = begI + (endI - begI) / 2;

	mergeSort_do(sortedArr, begI, midI, cmpCnt, movCnt);
	mergeSort_do(sortedArr, midI, endI, cmpCnt, movCnt);
	size_t i, j, k = 0;
	for (i = begI, j = midI; i < midI && j < endI; )
	{
		if (cmpCnt++, sortedArr[i] < sortedArr[j])
		{
			movCnt++;
			newArr[k++] = sortedArr[i++];
		}
		else
		{
			movCnt++;
			newArr[k++] = sortedArr[j++];
		}
	}

	while (i < midI)
	{
		movCnt++;
		newArr[k++] = sortedArr[i++];
	}

	while (j < endI)
	{
		movCnt++;
		newArr[k++] = sortedArr[j++];
	}

	for (i = begI; i < endI; i++)
	{
		movCnt++;
		sortedArr[i] = newArr[i - begI];
	}

	delete[]newArr;
}

template <typename E>
void mergeSort(Pointer<E> *sortedArr, size_t totalNum)
{
	size_t cmpCnt = 0, movCnt = 0;

	mergeSort_do(sortedArr, 0, totalNum, cmpCnt, movCnt);

	cout << "�Ƚϴ�����" << cmpCnt << endl;
	cout << "�ƶ�������" << movCnt << endl;
}
//shell����
template <typename E>
void shellSort_do(Pointer<E> *sortedArr, size_t totalNum, size_t gap, size_t &cmpCnt, size_t &movCnt)
{
	size_t j;
	for (size_t i = gap; i < totalNum; i++)
	{
		auto tmp = sortedArr[i];
		for (j = i - gap; j + gap >= gap &&(cmpCnt++, tmp < sortedArr[j]); j -= gap)
		{
			movCnt++;
			sortedArr[j + gap] = sortedArr[j];
		}
		movCnt++;
		sortedArr[j + gap] = tmp;
	}
}

template <typename E>
void shellSort(Pointer<E> *sortedArr, size_t totalNum)
{
	size_t cmpCnt = 0, movCnt = 0;
	for (size_t gap = totalNum / 2 / 2 * 2; gap != 0; gap-= 2)
	{
		shellSort_do(sortedArr, totalNum, gap, cmpCnt, movCnt);
	}
	shellSort_do(sortedArr, totalNum, 1, cmpCnt, movCnt);
	cout << "�Ƚϴ�����" << cmpCnt << endl;
	cout << "�ƶ�������" << movCnt << endl;

}
//��������
template <typename E>
size_t quickSort_part(Pointer<E> *sortedArr, size_t begI, size_t endI, size_t &cmpCnt, size_t &movCnt)
{
	auto pivotKey = sortedArr[begI];
	while (begI + 1 != endI)
	{
		while (begI + 1 != endI && (cmpCnt++, sortedArr[endI - 1] >= pivotKey))
			--endI;
		movCnt++;
		sortedArr[begI] = sortedArr[endI - 1];

		while (begI + 1 != endI && (cmpCnt++, sortedArr[begI] <= pivotKey))
			++begI;
		movCnt++;
		sortedArr[endI - 1] = sortedArr[begI];
	}
	movCnt++;
	sortedArr[begI] = pivotKey;

	return begI;
}

template <typename E>
void quickSort_do(Pointer<E> *sortedArr, size_t totalNum, size_t begI, size_t endI, size_t &cmpCnt, size_t &movCnt)
{
	if (endI - begI <= 1)
		return;
	size_t pivotI = quickSort_part(sortedArr, begI, endI, cmpCnt, movCnt);
	
	quickSort_do(sortedArr, totalNum, begI, pivotI, cmpCnt, movCnt);
	quickSort_do(sortedArr, totalNum, pivotI + 1, endI, cmpCnt, movCnt);
	
}

template <typename E>
void quickSort(Pointer<E> *sortedArr, size_t totalNum)
{
	size_t cmpCnt = 0, movCnt = 0;
	quickSort_do(sortedArr, totalNum, 0, totalNum, cmpCnt, movCnt);
	cout << "�Ƚϴ�����" << cmpCnt << endl;
	cout << "�ƶ�������" << movCnt << endl;
}

int main()
{
	typedef enum {INSERTION, SELECTION, BUBBLE, SHELL, QUICK, HEAP, MERGE, STD} sortType;
	bool willTest[] = { true, true, true, true, true, true, true, true };

	LARGE_INTEGER tick, fc_begin, fc_end;
	QueryPerformanceFrequency(&tick);

	srand(static_cast<unsigned>(time(NULL)));
	using elem = int;

	size_t totalNum;
	cout << "�����������Ԫ�ظ���" << endl;
	cin >> totalNum;
	elem *arr = new elem[totalNum];
	Pointer<elem> *sortedArr = new Pointer<elem>[totalNum];
	randGen(arr, totalNum);

	/*******ֱ�Ӳ�������******/
	if (willTest[INSERTION])
	{
		cout << " <ֱ�Ӳ�������>" << endl;
		for (size_t i = 0; i < totalNum; i++) { sortedArr[i].p = arr + i; }
		cout << " ==== ����ǰ ====" << endl;
		brieflyView(sortedArr, totalNum);
		cout << endl;

		QueryPerformanceCounter(&fc_begin);
		
		insertionSort(sortedArr, totalNum);

		QueryPerformanceCounter(&fc_end);

		cout << " ==== ����� ====" << endl;
		brieflyView(sortedArr, totalNum);
		checkSorted(sortedArr, totalNum);
		cout << "�������򹲺�ʱ " << fixed << setprecision(3) << (fc_end.QuadPart - fc_begin.QuadPart) / double(tick.QuadPart) << " �롣" << endl;
		cout << endl;
	}

	/*******ֱ��ѡ������******/
	if (willTest[SELECTION])
	{
		cout << " <ֱ��ѡ������>" << endl;
		for (size_t i = 0; i < totalNum; i++) { sortedArr[i].p = arr + i; }
		cout << " ==== ����ǰ ====" << endl;
		brieflyView(sortedArr, totalNum);
		cout << endl;

		QueryPerformanceCounter(&fc_begin);

		selectionSort(sortedArr, totalNum);

		QueryPerformanceCounter(&fc_end);

		cout << " ==== ����� ====" << endl;
		brieflyView(sortedArr, totalNum);
		checkSorted(sortedArr, totalNum);
		cout << "�������򹲺�ʱ " << fixed << setprecision(3) << (fc_end.QuadPart - fc_begin.QuadPart) / double(tick.QuadPart) << " �롣" << endl;
		cout << endl;

	}

	/*******ð������******/
	if (willTest[BUBBLE])
	{
		cout << " <ð������>" << endl;
		for (size_t i = 0; i < totalNum; i++) { sortedArr[i].p = arr + i; }
		cout << " ==== ����ǰ ====" << endl;
		brieflyView(sortedArr, totalNum);
		cout << endl;

		QueryPerformanceCounter(&fc_begin);

		bubbleSort(sortedArr, totalNum);

		QueryPerformanceCounter(&fc_end);

		cout << " ==== ����� ====" << endl;
		brieflyView(sortedArr, totalNum);
		checkSorted(sortedArr, totalNum);
		cout << "�������򹲺�ʱ " << fixed << setprecision(3) << (fc_end.QuadPart - fc_begin.QuadPart) / double(tick.QuadPart) << " �롣" << endl;
		cout << endl;

	}

	/*******SHELL����******/
	if (willTest[SHELL])
	{
		cout << " <ϣ������>" << endl;
		for (size_t i = 0; i < totalNum; i++) { sortedArr[i].p = arr + i; }
		cout << " ==== ����ǰ ====" << endl;
		brieflyView(sortedArr, totalNum);
		cout << endl;

		QueryPerformanceCounter(&fc_begin);

		shellSort(sortedArr, totalNum);

		QueryPerformanceCounter(&fc_end);

		cout << " ==== ����� ====" << endl;
		brieflyView(sortedArr, totalNum);
		checkSorted(sortedArr, totalNum);
		cout << "�������򹲺�ʱ " << fixed << setprecision(3) << (fc_end.QuadPart - fc_begin.QuadPart) / double(tick.QuadPart) << " �롣" << endl;
		cout << endl;

	}

	/*******��������******/
	if (willTest[QUICK])
	{
		cout << " <��������>" << endl;
		for (size_t i = 0; i < totalNum; i++) { sortedArr[i].p = arr + i; }
		cout << " ==== ����ǰ ====" << endl;
		brieflyView(sortedArr, totalNum);
		cout << endl;

		QueryPerformanceCounter(&fc_begin);

		quickSort(sortedArr, totalNum);

		QueryPerformanceCounter(&fc_end);

		cout << " ==== ����� ====" << endl;
		brieflyView(sortedArr, totalNum);
		checkSorted(sortedArr, totalNum);
		cout << "�������򹲺�ʱ " << fixed << setprecision(3) << (fc_end.QuadPart - fc_begin.QuadPart) / double(tick.QuadPart) << " �롣" << endl;
		cout << endl;

	}

	/*******����******/
	if (willTest[HEAP])
	{
		cout << " <������>" << endl;
		for (size_t i = 0; i < totalNum; i++) { sortedArr[i].p = arr + i; }
		cout << " ==== ����ǰ ====" << endl;
		brieflyView(sortedArr, totalNum);
		cout << endl;
		QueryPerformanceCounter(&fc_begin);
		Heap<Pointer<elem>> h(MINHEAP, totalNum);

		for (size_t i = 0; i < totalNum; i++)
		{
			h.insert(Pointer<elem>(&arr[i]));
		}

		for (size_t i = 0; i < totalNum; i++)
		{
			sortedArr[i] = h.cut_top();
		}
		QueryPerformanceCounter(&fc_end);

		cout << " ==== ����� ====" << endl;
		brieflyView(sortedArr, totalNum);
		checkSorted(sortedArr, totalNum);
		cout << "�������򹲺�ʱ " << fixed << setprecision(3) << (fc_end.QuadPart - fc_begin.QuadPart) / double(tick.QuadPart) << " �롣" << endl;
		cout << endl;

	}

	/*******�鲢����******/
	if (willTest[MERGE])
	{
		cout << " <�鲢����>" << endl;
		for (size_t i = 0; i < totalNum; i++) { sortedArr[i].p = arr + i; }
		cout << " ==== ����ǰ ====" << endl;
		brieflyView(sortedArr, totalNum);
		cout << endl;

		QueryPerformanceCounter(&fc_begin);

		mergeSort(sortedArr, totalNum);

		QueryPerformanceCounter(&fc_end);

		cout << " ==== ����� ====" << endl;
		brieflyView(sortedArr, totalNum);
		checkSorted(sortedArr, totalNum);
		cout << "�������򹲺�ʱ " << fixed << setprecision(3) << (fc_end.QuadPart - fc_begin.QuadPart) / double(tick.QuadPart) << " �롣" << endl;
		cout << endl;
	}

	/*******������******/
	if (willTest[STD])
	{
		cout << " <C++�⺯������>" << endl;
		for (size_t i = 0; i < totalNum; i++) { sortedArr[i].p = arr + i; }
		cout << " ==== ����ǰ ====" << endl;
		brieflyView(sortedArr, totalNum);
		cout << endl;

		QueryPerformanceCounter(&fc_begin);

		sort(sortedArr, sortedArr + totalNum - 1);

		QueryPerformanceCounter(&fc_end);

		cout << " ==== ����� ====" << endl;
		brieflyView(sortedArr, totalNum);
		checkSorted(sortedArr, totalNum);
		cout << "�������򹲺�ʱ " << fixed << setprecision(3) << (fc_end.QuadPart - fc_begin.QuadPart) / double(tick.QuadPart) << " �롣" << endl;
		cout << endl;
	}

	return 0;
}

