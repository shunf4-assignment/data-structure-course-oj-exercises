/*1652270 CS2 fengshun*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <climits>

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

	Hash(size_t size_, size_t (*hashFunc_)(const E&e))
	{
		arr = new HashItem[size_];
		size = size_;
		hashFunc = hashFunc_;
	}

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
				std::cout << h;
#endif
				while ((arr[h].data != e && arr[h].cnt != 0))
				{
					h += 1;
					h %= size;
					lookupCount++;
#ifdef _FS_DEBUG
					std::cout << "=" <<lookupCount<< "=>" << h;
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

	size_t search_rSRCnt(const E&e, size_t &addrRecv) //wrong
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

size_t hashString(const std::string &s)
{
	size_t base = 37;
	size_t currBase = 1;
	size_t result = 0;
	for (auto p = s.begin(); p != s.end(); p++)
	{
		result += static_cast<size_t>(*p) * currBase;
		currBase *= base;
	}
	return result;
}

#define FS_INFILE
//#define FS_OUTFILE
int main()
{
#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	std::ifstream f0;
	f0.open("a2p3.txt", std::ios::in);
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
	using std::endl;
	using std::string;

	size_t keyNum, hashSize;
	cin >> keyNum >> hashSize;
	Hash<string> h(hashSize, hashString);
	string curStr;
	size_t *addr = new size_t[hashSize];
	size_t *findCnt = new size_t[hashSize];
	for (size_t i = 0; i < keyNum; i++)
	{
		cin >> curStr;
		findCnt[i] = h.insert_rLUCnt(curStr, addr[i]);
	}

	for (size_t i = 0; i < keyNum; i++)
	{
		if (i)
			cout << ' ';
		cout << addr[i];
	}
	cout << endl;
	for (size_t i = 0; i < keyNum; i++)
	{
		if (i)
			cout << ' ';
		cout << findCnt[i];
	}

	delete[]addr;
	delete[]findCnt;


#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	f0.close();
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))
	f1.close();
#endif
	return 0;
}

