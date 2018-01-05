/*1652270 CS2 fengshun*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>

template <typename VertexElem, typename EdgeWeight>
class MinSpanTree;

typedef int ufsize;
const ufsize defaultSize = 100;
class UFSets
{
	int * arr;
	int size;
public :
	UFSets(ufsize size_ = defaultSize)
	{
		size = size_;
		arr = new int[size];
		for (size_t i = 0; i < size; i++)
		{
			arr[i] = -1;
		}
	}

	void uni(int r1, int r2) //Union
	{
		arr[r1] += arr[r2];
		arr[r2] = r1;
	}

	int find(int x)
	{
		while (arr[x] >= 0)
		{
			x = arr[x];
		}
		return x;
	}

};

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
		E top = arr[0];
		if (n == 0)
			throw "HEAP IS EMPTY";
		if (n == 1)
		{
			n = 0;
			return arr[0];
		}
		arr[0] = arr[--n];
		shiftDown(0);
		return top;
	}

	~Heap()
	{
		if (arr)
			delete[] arr;
	}
};

template <typename VertexElem, typename EdgeWeight>
class Graph
{
public:
	bool directed;
	bool weighted;
	size_t vertexNum;
	EdgeWeight defaultWeight;

	Graph()
	{
		setDirected(false);
		setWeighted(false);
		setNonDirectWeight(1);
		vertexNum = 0;
	}

	Graph(bool directed, bool weighted, EdgeWeight defaultWeight)
	{
		setDirected(directed);
		setWeighted(weighted);
		setNonDirectWeight(defaultWeight);
		vertexNum = 0;
	}

	void setNonDirectWeight(EdgeWeight defaultWeight)
	{
		this->defaultWeight = defaultWeight;
	}

	void setDirected(bool d)
	{
		directed = d;
	}

	void setWeighted(bool w)
	{
		weighted = w;
	}
};

template <typename E>
class Matrix
{
private:
	E * matrix_array;

public:
	size_t rows;
	size_t cols;

	Matrix()
	{
		rows = 0;
		cols = 0;
		matrix_array = NULL;
	}
	void init(size_t rows, size_t cols, E* arr = NULL)
	{
		if (matrix_array)
			delete[] matrix_array;
		matrix_array = new E[rows * cols]();
		this->rows = rows;
		this->cols = cols;

		if (arr)
#ifdef _MSC_VER
			memcpy_s(matrix_array, sizeof(E) * rows * cols, arr, sizeof(E) * rows * cols);
#else
			memcpy(matrix_array, arr, sizeof(E) * rows * cols);
#endif
	}
	Matrix(unsigned rows, unsigned cols)
	{
		init(rows, cols);
	}
	Matrix(unsigned rows, unsigned cols, E * arr)
	{
		init(rows, cols, arr);
	}
	~Matrix()
	{
		if (matrix_array)
			delete[] matrix_array;
	}
	E * operator[](size_t i)
	{
		if (i >= rows)
			throw "INDEX OUT OF RANGE";
		return matrix_array + i * cols;
	}

	void print(std::ostream&out)
	{
		for (size_t r = 0; r < rows; r++)
		{
			for (size_t c = 0; c < cols; c++)
			{
				out << std::setw(3) << " " << (*this)[r][c];
			}
			out << std::endl;
		}
	}
};

template <typename E>
struct Node
{
	E data;
	Node<E> *next;
	Node()
	{
		next = NULL;
	}
	~Node()
	{
		if (next)
			delete next;
	}
};

template <typename EdgeWeight>
struct WeightedDest
{
	size_t vi;
	EdgeWeight we;
};

template <typename VertexElem, typename EdgeWeight>
struct AdjListElem
{
	VertexElem v;
	Node<WeightedDest<EdgeWeight>> *adjV;

	AdjListElem()
	{
		adjV = NULL;
	}

	~AdjListElem()
	{
		if (adjV)
			delete adjV;
	}

	void print(std::ostream &out, bool printWeight)
	{
		out << v << "-->";
		auto aP = adjV;
		while (aP)
		{
			if (aP != adjV)
				out << ' ';
			out << aP->data.vi;
			if (printWeight)
				out << ',' << aP->data.we;
			aP = aP->next;
		}
	}
};

template <typename VertexElem, typename EdgeWeight>
class Graph_Matrix : public Graph<VertexElem, EdgeWeight>
{
private:
	Matrix<EdgeWeight> e;
	VertexElem * v;

public:
	Graph_Matrix(bool directed, bool weighted, EdgeWeight defaultWeight)
	{
		Graph<VertexElem, EdgeWeight>::Graph(directed, weighted, defaultWeight);
		v = NULL;
	}

	Graph_Matrix() : Graph<VertexElem, EdgeWeight>()
	{
		v = NULL;
	}

	size_t indexOfVertex(const VertexElem &vtx) const
	{
		for (size_t i = 0; i < e.cols; i++)
		{
			if (vtx == v[i])
				return i;
		}
		throw "NOT FOUND";
	}

	~Graph_Matrix()
	{
		if (v)
			delete[] v;
	}

	template <typename VertexElem_, typename EdgeWeight_>
	friend std::istream& operator >> (std::istream &in, Graph_Matrix<VertexElem_, EdgeWeight_> &gm);
	template <typename VertexElem_, typename EdgeWeight_>
	friend std::ostream& operator << (std::ostream &out, Graph_Matrix<VertexElem_, EdgeWeight_> &gm);

	void printVertex(std::ostream& out)
	{
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			if (i)
				out << ' ';
			out << v[i];
		}
	}

	VertexElem &getVertex(size_t i)
	{
		if (i < Graph<VertexElem, EdgeWeight>::vertexNum)
			return v[i];
		else
			throw "VERTEX OUT OF RANGE";
	}

	size_t firstNeighbor(size_t i)
	{
		size_t x;
		for (x = 0; x < Graph<VertexElem, EdgeWeight>::vertexNum; x++)
		{
			if (e[i][x] != 0)
			{
				break;
			}
		}
		if (x == Graph<VertexElem, EdgeWeight>::vertexNum)
			return SIZE_MAX;
		else
			return x;
	}

	size_t nextNeighbor(size_t i, size_t cur)
	{
		size_t x;
		for (x = cur + 1; x < Graph<VertexElem, EdgeWeight>::vertexNum; x++)
		{
			if (e[i][x] != 0)
			{
				break;
			}
		}
		if (x == Graph<VertexElem, EdgeWeight>::vertexNum)
			return SIZE_MAX;
		else
			return x;
	}
};

template <typename VertexElem, typename EdgeWeight>
class Graph_List : public Graph<VertexElem, EdgeWeight>
{
private:
	AdjListElem<VertexElem, EdgeWeight> * v;

public:
	Graph_List(bool directed, bool weighted, EdgeWeight defaultWeight) : Graph<VertexElem, EdgeWeight>(directed, weighted, defaultWeight)
	{
		v = NULL;
	}

	Graph_List() : Graph<VertexElem, EdgeWeight>()
	{
		v = NULL;
	}

	~Graph_List()
	{
		if (v)
			delete[] v;
	}

	size_t indexOfVertex(const VertexElem &vtx) const
	{
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			if (vtx == v[i].v)
				return i;
		}
		throw "NOT FOUND";
	}

	template <typename VertexElem_, typename EdgeWeight_>
	friend std::istream& operator >> (std::istream &in, Graph_List<VertexElem_, EdgeWeight_> &gm);
	template <typename VertexElem_, typename EdgeWeight_>
	friend std::ostream& operator << (std::ostream &out, Graph_List<VertexElem_, EdgeWeight_> &gm);

	void printVertex(std::ostream& out)
	{
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			if (i)
				out << ' ';
			out << v[i].v;
		}
	}

	VertexElem &getVertex(size_t i)
	{
		if (i < Graph<VertexElem, EdgeWeight>::vertexNum)
			return v[i].v;
		else
			throw "VERTEX OUT OF RANGE";
	}

	size_t firstNeighbor(size_t i)
	{
		if (v[i].adjV == NULL)
			return SIZE_MAX;
		return v[i].adjV->data.vi;
	}

	size_t nextNeighbor(size_t i, size_t cur)
	{
		if (v[i].adjV == NULL)
			return SIZE_MAX;
		auto currAdjV = v[i].adjV;
		while (currAdjV && cur != currAdjV->data.vi)
		{
			currAdjV = currAdjV->next;
		}
		if (currAdjV == NULL || currAdjV->next == NULL)
			return SIZE_MAX;
		return currAdjV->next->data.vi;
	}
	
	friend class MinSpanTree<VertexElem, EdgeWeight>;
};

template <typename EdgeWeight>
class MSTEdge
{
public:
	size_t vs;
	size_t ve;
	EdgeWeight e;

	MSTEdge() = default;

	MSTEdge(size_t vs_, size_t ve_, EdgeWeight e_)
	{
		vs = vs_;
		ve = ve_;
		e = e_;
	}

	void set(size_t vs_, size_t ve_, EdgeWeight e_)
	{
		vs = vs_;
		ve = ve_;
		e = e_;
	}

	bool operator>=(const MSTEdge<EdgeWeight> &e2)const { return e >= e2.e; }
	bool operator<=(const MSTEdge<EdgeWeight> &e2)const { return e <= e2.e; }
	bool operator==(const MSTEdge<EdgeWeight> &e2)const { return e == e2.e; }
	bool operator<(const MSTEdge<EdgeWeight> &e2)const { return e < e2.e; }
	bool operator>(const MSTEdge<EdgeWeight> &e2)const { return e > e2.e; }
};

template <typename VertexElem, typename EdgeWeight>
class MinSpanTree
{
public:
	MSTEdge<EdgeWeight> *arr;
	size_t size;
	size_t n;

	MinSpanTree(size_t size_)
	{
		init(size_);
	}

	void init(size_t size_)
	{
		size = size_;
		arr = new MSTEdge<EdgeWeight>[size];
		n = 0;
	}

	MinSpanTree(const Graph_List<VertexElem, EdgeWeight> &g)
	{
		init(g.vertexNum);
		Heap<MSTEdge<EdgeWeight>> h(MINHEAP, g.vertexNum * g.vertexNum);
		for (size_t i = 0; i < g.vertexNum; i++)
		{
			auto p = g.v[i].adjV;
			while (p)
			{
				//insert(i, p->data.vi, p->data.we);
				h.insert(MSTEdge<EdgeWeight>(i, p->data.vi, p->data.we));
				p = p->next;
			}
		}

		MSTEdge<EdgeWeight> currEdge;
		UFSets u(static_cast<ufsize>(g.vertexNum));
		int vsRoot, veRoot;
		while (h.n != 0)
		{
			currEdge = h.cut_top();
			vsRoot = u.find(static_cast<ufsize>(currEdge.vs));
			veRoot = u.find(static_cast<ufsize>(currEdge.ve));
			if (vsRoot != veRoot)
			{
				insert(currEdge);
				u.uni(vsRoot, veRoot);
			}
		}
	}

	void insert(size_t vs, size_t ve, EdgeWeight e)
	{
		arr[n++].set(vs, ve, e);
	}

	void insert(MSTEdge<EdgeWeight> mstn)
	{
		arr[n++] = mstn;
	}

	EdgeWeight totalWeight()
	{
		EdgeWeight e = EdgeWeight();
		for (size_t i = 0; i < n; i++)
		{
			e += arr[i].e;
		}
		return e;
	}

	~MinSpanTree()
	{
		delete[]arr;
	}
};


typedef enum { NONE, DIRECTED, DIRECTED_NET, UNDIRECTED, UNDIRECTED_NET } GraphType;

template <typename VertexElem, typename EdgeWeight>
std::istream & operator >> (std::istream & in, Graph_Matrix<VertexElem, EdgeWeight> &gm)
{
	if (gm.v)
		delete[]gm.v;

	gm.setDirected(false);
	gm.setWeighted(true);
	gm.setNonDirectWeight(1);

	size_t vCnt, eCnt;
	in >> vCnt >> eCnt;

	gm.vertexNum = vCnt;

	gm.v = new VertexElem[vCnt];
	gm.e.init(vCnt, vCnt);

	for (size_t i = 0; i < vCnt; i++)
	{
		gm.v[i] = static_cast<VertexElem>(i);
	}

	VertexElem vS, vE;
	EdgeWeight eW;

	eW = gm.defaultWeight;

	for (size_t i = 0; i < eCnt; i++)
	{
		in >> vS >> vE;
		if (gm.weighted)
		{
			in >> eW;
		}

		gm.e[gm.indexOfVertex(vS)][gm.indexOfVertex(vE)] = eW;
		if (!gm.directed)
		{
			gm.e[gm.indexOfVertex(vE)][gm.indexOfVertex(vS)] = eW;
		}
	}
	return in;
}

template <typename VertexElem, typename EdgeWeight>
std::istream & operator >> (std::istream & in, Graph_List<VertexElem, EdgeWeight> &ga)
{
	if (ga.v)
		delete[]ga.v;

	ga.setDirected(false);
	ga.setWeighted(true);
	ga.setNonDirectWeight(1);

	size_t vCnt, eCnt;
	in >> vCnt >> eCnt;

	ga.vertexNum = vCnt;

	ga.v = new AdjListElem<VertexElem, EdgeWeight>[vCnt]();


	for (size_t i = 0; i < vCnt; i++)
	{
		ga.v[i].v = static_cast<VertexElem>(i);
	}

	VertexElem vS, vE;
	EdgeWeight eW;

	eW = ga.defaultWeight;
	Node<WeightedDest<EdgeWeight>> *curNP, *newNP;

	size_t vsi;
	size_t vei;

	for (size_t i = 0; i < eCnt; i++)
	{
		in >> vS >> vE;
		vS--, vE--;
		vsi = ga.indexOfVertex(vS);
		vei = ga.indexOfVertex(vE);
		if (ga.weighted)
		{
			in >> eW;
		}

		curNP = ga.v[vsi].adjV;
		newNP = new Node<WeightedDest<EdgeWeight>>;
		newNP->next = curNP;
		newNP->data.vi = vei;
		newNP->data.we = eW;

		ga.v[vsi].adjV = newNP;

		if (!ga.directed)
		{
			curNP = ga.v[vei].adjV;
			newNP = new Node<WeightedDest<EdgeWeight>>;
			newNP->next = curNP;
			newNP->data.vi = vsi;
			newNP->data.we = eW;
			ga.v[vei].adjV = newNP;
		}
	}
	return in;
}

#define FS_INFILE
//#define FS_OUTFILE
int main()
{
#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	std::ifstream f0;
	f0.open("ap1.txt", std::ios::in);
	auto cinbackup = std::cin.rdbuf();
	std::cin.set_rdbuf(f0.rdbuf());
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))

	std::ofstream f1;
	f1.open("out.txt", std::ios::out);
	auto coutbackup = std::cout.rdbuf();
	std::cout.set_rdbuf(f1.rdbuf());
#endif

	if (1)
	{
		Graph_List<int, int> g;
		std::cin >> g;
		MinSpanTree<int, int> mst(g);
		std::cout << mst.totalWeight();
	}


#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	f0.close();
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))
	f1.close();
#endif
	return 0;
}

