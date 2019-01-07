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

using namespace std;

template <typename E>
void inputTo(E &e)
{
	cin >> e;
	cout << e << endl;
}

#define inputTo(e) inputTo<decltype(e)>(e)


template <typename VertexElem, typename EdgeWeight>
class MinSpanTree;

typedef int ufsize;
const ufsize defaultSize = 100;
class UFSets
{
	int * arr;
	int size;
public:
	UFSets(ufsize size_ = defaultSize)
	{
		size = size_;
		arr = new int[size];
		for (int i = 0; i < size; i++)
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
	bool directed;	//是否有向
	bool weighted;	//是否有权
	size_t vertexNum;
	EdgeWeight defaultWeight;	//默认权重（1）
	EdgeWeight infinity;	//默认权重最大值，不会被问题用到的
	//构造
	Graph()
	{
		setDirected(false);
		setWeighted(false);
		setNonDirectWeight(1);
		vertexNum = 0;
		setInfinity((std::numeric_limits<EdgeWeight>::max)());
	}
	//构造
	Graph(bool directed, bool weighted, EdgeWeight defaultWeight)
	{
		setDirected(directed);
		setWeighted(weighted);
		setNonDirectWeight(defaultWeight);
		vertexNum = 0;
		setInfinity((std::numeric_limits<EdgeWeight>::max)());
	}
	//设置默认权重
	void setNonDirectWeight(EdgeWeight defaultWeight)
	{
		this->defaultWeight = defaultWeight;
	}
	//设置权重无穷值
	void setInfinity(EdgeWeight infinityWeight)
	{
		this->infinity = infinityWeight;
	}
	//设置是否有向
	void setDirected(bool d)
	{
		directed = d;
	}
	//设置是否有权
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

//带权的终点，作为邻接表链表结点元素的辅助结构
template <typename EdgeWeight>
struct WeightedDest
{
	size_t vi;	//终点编号
	EdgeWeight we;	//该边权重
};

//邻接表项，从一个点出发的所有边
template <typename VertexElem, typename EdgeWeight>
struct AdjListElem
{
	VertexElem v;	//该点的内容
	Node<WeightedDest<EdgeWeight>> *adjV;	//边链表
	//构造
	AdjListElem()
	{
		adjV = NULL;
	}
	//析构
	~AdjListElem()
	{
		if (adjV)
			delete adjV;
	}

};

template <typename VertexElem, typename EdgeWeight>
class Graph_Matrix : public Graph<VertexElem, EdgeWeight>
{
private:
	Matrix<EdgeWeight> e;	//就是一个行列数可变的二维数组，在报告中不另外给出
	VertexElem * v;		//点的内容表


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
	//在点的内容中查找某个内容，获取该点编号
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
	//打印所有点
	void printVertex(std::ostream& out)
	{
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			if (i)
				out << ' ';
			out << v[i];
		}
	}
	//获取第i个点
	VertexElem &getVertex(size_t i)
	{
		if (i < Graph<VertexElem, EdgeWeight>::vertexNum)
			return v[i];
		else
			throw "VERTEX OUT OF RANGE";
	}
	//该点的第一个临近节点
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
	//下一个临近结点
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

	//用DFS遍历，打印连通分量
	std::string DFS_Components()
	{
		bool *visited = new bool[Graph<VertexElem, EdgeWeight>::vertexNum];
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			visited[i] = false;
		}
		std::stringstream components;
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			if (visited[i] == false)
			{
				components << "{";
				DFS_Components_do(i, visited, components, true);
				components << "}";

			}
		}
		return components.str();
	}
	//辅助函数
	void DFS_Components_do(size_t i, bool *visited, std::stringstream &components, bool exteralInvoke)
	{
		if (visited[i] == true)
			return;

		visited[i] = true;
		if (!exteralInvoke)
			components << " ";
		components << getVertex(i);

		size_t currConnVert = this->firstNeighbor(i);

		while (currConnVert != SIZE_MAX)
		{
			if (visited[currConnVert] == false)
				DFS_Components_do(currConnVert, visited, components, false);
			currConnVert = this->nextNeighbor(i, currConnVert);
		}

		return;
	}
	//用BFS遍历，打印连通分量
	std::string BFS_Components()
	{
		bool *visited = new bool[Graph<VertexElem, EdgeWeight>::vertexNum];
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			visited[i] = false;
		}
		std::stringstream components;
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			if (visited[i] == false)
			{
				components << "{";
				BFS_Components_do(i, visited, components, true);
				components << "}";

			}
		}
		return components.str();
	}
	//辅助函数
	void BFS_Components_do(size_t i, bool *visited, std::stringstream &components, bool externalInvoke)
	{
		const size_t QMax = 100000;
		static size_t queue[QMax] = { 0 };
		static size_t *front = queue;
		static size_t *rear = queue;
		if (externalInvoke)
		{
			front = rear = queue;
			*(rear++) = i;
			visited[i] = true;
			BFS_Components_do(i, visited, components, false);
			return;
		}

		while (front != rear)
		{
			size_t currV;
			if (front != queue)
				components << ' ';
			currV = *(front++);
			components << v[currV];

			auto currConnV = e[currV];
			for (size_t x = 0; x < Graph<VertexElem, EdgeWeight>::vertexNum; x++)
			{
				if (currConnV[x] != 0)
				{
					if (visited[x] == false)
					{
						visited[x] = true;
						*(rear++) = x;
					}
				}
			}
		}
	}
};

template <typename VertexElem, typename EdgeWeight>
class Graph_List : public Graph<VertexElem, EdgeWeight>
{
public:
	//邻接表项构成的表
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
	//打印邻接表结构
	void printStruct(std::ostream &out)
	{
		for (size_t i = 0; i < this->vertexNum; i++)
		{
			out << v[i].v << "-->";
			auto aP = v[i].adjV;
			while (aP)
			{
				if (aP != v[i].adjV)
					out << ' ';
				out << v[aP->data.vi].v;
				if (this->weighted)
					out << ',' << aP->data.we;
				aP = aP->next;
			}
			out << endl;
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

	std::string DFS_Components()
	{
		bool *visited = new bool[Graph<VertexElem, EdgeWeight>::vertexNum];
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			visited[i] = false;
		}
		std::stringstream components;
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			if (visited[i] == false)
			{
				components << "{";
				DFS_Components_do(i, visited, components, true);
				components << "}";

			}
		}
		return components.str();
	}

	void DFS_Components_do(size_t i, bool *visited, std::stringstream &components, bool exteralInvoke)
	{
		if (visited[i] == true)
			return;

		visited[i] = true;
		if (!exteralInvoke)
			components << " ";
		components << getVertex(i);

		size_t currConnVert = this->firstNeighbor(i);

		while (currConnVert != SIZE_MAX)
		{
			if (visited[currConnVert] == false)
				DFS_Components_do(currConnVert, visited, components, false);
			currConnVert = this->nextNeighbor(i, currConnVert);
		}

		return;
	}

	std::string BFS_Components()
	{
		bool *visited = new bool[Graph<VertexElem, EdgeWeight>::vertexNum];
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			visited[i] = false;
		}
		std::stringstream components;
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			if (visited[i] == false)
			{
				components << "{";
				BFS_Components_do(i, visited, components, true);
				components << "}";

			}
		}
		return components.str();
	}

	void BFS_Components_do(size_t i, bool *visited, std::stringstream &components, bool externalInvoke)
	{
		const size_t QMax = 100000;
		static size_t queue[QMax] = { 0 };
		static size_t *front = queue;
		static size_t *rear = queue;
		if (externalInvoke)
		{
			front = rear = queue;
			*(rear++) = i;
			visited[i] = true;
			BFS_Components_do(i, visited, components, false);
			return;
		}

		while (front != rear)
		{
			size_t currV;
			if (front != queue)
				components << ' ';
			currV = *(front++);
			components << v[currV].v;

			auto currConnV = v[currV].adjV;
			while (currConnV)
			{
				if (visited[currConnV->data.vi] == false)
				{
					visited[currConnV->data.vi] = true;
					*(rear++) = currConnV->data.vi;
				}
				currConnV = currConnV->next;
			}
		}
	}
	//打印Dijkstra后firstVertex到各点的距离
	void output_dijkstra(std::ostream &out, size_t firstVertex)
	{
		struct closestEdge {
			EdgeWeight leastWeight;
			bool visited;
			size_t lastVertex;
		};
		if (Graph<VertexElem, EdgeWeight>::vertexNum == 0)
			return;
		closestEdge *c = new closestEdge[Graph<VertexElem, EdgeWeight>::vertexNum];
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			c[i].leastWeight = Graph<VertexElem, EdgeWeight>::infinity;
			c[i].visited = false;
			c[i].lastVertex = SIZE_MAX;
		}

		auto p = v[firstVertex].adjV;
		while (p)
		{
			c[p->data.vi].leastWeight = p->data.we;
			c[p->data.vi].lastVertex = firstVertex;
			p = p->next;
		}

		c[firstVertex].visited = true;
		c[firstVertex].leastWeight = 0;
		c[firstVertex].lastVertex = SIZE_MAX;

		while (true)
		{
			EdgeWeight currMin = Graph<VertexElem, EdgeWeight>::infinity;
			size_t minEdgeS = SIZE_MAX;
			size_t minEdgeE;
			for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
			{
				if (c[i].visited == false)
					continue;
				auto p = v[i].adjV;
				while (p)
				{
					/*if (c[p->data.vi].visited == false && c[i].leastWeight != Graph<VertexElem, EdgeWeight>::infinity && c[i].leastWeight + p->data.we < currMin)*/
					if (c[p->data.vi].visited == false && c[p->data.vi].leastWeight < currMin)
					{
						currMin = c[p->data.vi].leastWeight;
						minEdgeS = i;
						minEdgeE = p->data.vi;
					}
					p = p->next;
				}
			}
			/*if (currMin == Graph<VertexElem, EdgeWeight>::infinity)
			break;*/
			if (minEdgeS == SIZE_MAX)
				break;
			c[minEdgeE].visited = true;
			c[minEdgeE].leastWeight = currMin;
			c[minEdgeE].lastVertex = minEdgeS;

			auto p = v[minEdgeE].adjV;
			while (p)
			{
				if (c[minEdgeE].leastWeight != Graph<VertexElem, EdgeWeight>::infinity && c[minEdgeE].leastWeight + p->data.we < c[p->data.vi].leastWeight)
				{
					c[p->data.vi].leastWeight = c[minEdgeE].leastWeight + p->data.we;
					c[p->data.vi].lastVertex = minEdgeE;
				}
				p = p->next;
			}
		}

		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			out << v[firstVertex].v << "-- (" << c[i].leastWeight << ") -->" << v[i].v << endl;
		}

		delete[]c;
	}
	//DFS法拓扑排序
	typedef enum { NOTVISITED, VISITING, VISITED } visitStatus;
	bool DFS_TopoSort(size_t *vertexSeq)
	{
		visitStatus *visited = new visitStatus[Graph<VertexElem, EdgeWeight>::vertexNum];
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			visited[i] = NOTVISITED;
		}

		size_t *vertexSeq_modified = vertexSeq;
		size_t ** vertexSeqP = &vertexSeq_modified;
		bool succeed = true;

		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			size_t j = this->vertexNum - 1 - i;
			if (visited[j] == false)
			{
				succeed = succeed && DFS_TopoSort_do(j, visited, vertexSeqP, true);
			}
		}
		delete[] visited;

		if (!succeed)
			return false;

		size_t tmp;
		for (size_t i = 0; i <= (Graph<VertexElem, EdgeWeight>::vertexNum - 1) / 2; i++)
		{
			tmp = vertexSeq[i];
			vertexSeq[i] = vertexSeq[Graph<VertexElem, EdgeWeight>::vertexNum - 1 - i];
			vertexSeq[Graph<VertexElem, EdgeWeight>::vertexNum - 1 - i] = tmp;
		}
		return true;
	}
	//辅助函数
	bool DFS_TopoSort_do(size_t i, visitStatus *visited, size_t **vertexSeqP, bool externalInvoke)
	{
		static bool circled = false;
		if (externalInvoke)
		{
			circled = false;
		}
		if (visited[i] == VISITED)
			return true;

		if (circled)
			return false;

		visited[i] = VISITING;

		//size_t currConnVert = this->firstNeighbor(i);
		size_t currConnVert, k;
		for (k = 0; k < this->vertexNum; k++)
		{
			currConnVert = this->vertexNum - 1 - k;
			auto p = v[i].adjV;
			while (p)
			{
				if (p->data.vi == currConnVert)
					break;
				p = p->next;
			}
			if (p != NULL)
				break;
		}

		while (k != this->vertexNum)
		{
			if (visited[currConnVert] == NOTVISITED)
			{
				if (DFS_TopoSort_do(currConnVert, visited, vertexSeqP, false) == false)
					return false;
			}
			else if (visited[currConnVert] == VISITING)
			{
				circled = true;
				return false;
			}
			//currConnVert = this->nextNeighbor(i, currConnVert);

			for (k++; k < this->vertexNum; k++)
			{
				currConnVert = this->vertexNum - 1 - k;
				auto p = v[i].adjV;
				while (p)
				{
					if (p->data.vi == currConnVert)
						break;
					p = p->next;
				}
				if (p != NULL)
					break;
			}
		}

		visited[i] = VISITED;

		*((*vertexSeqP)++) = i;

		return true;
	}
	//删边法拓扑排序
	bool Trim_TopoSort(size_t *topoSeq)
	{
		int *inDegreeList = new int[this->vertexNum];
		for (size_t i = 0; i < this->vertexNum; i++)
		{
			inDegreeList[i] = 0;
		}
		for (size_t i = 0; i < this->vertexNum; i++)
		{
			auto p = v[i].adjV;
			while (p)
			{
				inDegreeList[p->data.vi]++;
				p = p->next;
			}
		}
		size_t currSeqIndex = 0;
		bool cycle = false;
		while (true)
		{
			bool allNegative = true;
			size_t i;
			for (i = 0; i < this->vertexNum; i++)
			{
				if (inDegreeList[i] > 0)
				{
					allNegative = false;
				}
				if (inDegreeList[i] == 0)
				{
					inDegreeList[i]--;
					auto p = v[i].adjV;
					while (p)
					{
						inDegreeList[p->data.vi]--;
						p = p->next;
					}
					topoSeq[currSeqIndex++] = i;
					break;
				}
			}
			if (i == this->vertexNum)
			{
				if (!allNegative)
					cycle = true;
				break;
			}
		}
		delete[]inDegreeList;

		if (cycle)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	//打印关键路径
	void printCritPath(std::ostream &out, size_t *topoSeq)
	{
		if (this->vertexNum == 0)
			throw "NO EMPTY GRAPH ALLOWED";
		EdgeWeight *earliest = new EdgeWeight[this->vertexNum];
		EdgeWeight *latest = new EdgeWeight[this->vertexNum];
		bool *critV = new bool[this->vertexNum];

		for (size_t i = 0; i < this->vertexNum; i++)
		{
			earliest[i] = 0;
			latest[i] = this->infinity;
			critV[i] = false;
		}

		//get earliest time of vertexes
		for (size_t i = 0; i < this->vertexNum; i++)
		{
			auto p = v[topoSeq[i]].adjV;
			while (p)
			{
				if (earliest[p->data.vi] < earliest[topoSeq[i]] + p->data.we)
				{
					earliest[p->data.vi] = earliest[topoSeq[i]] + p->data.we;
				}
				p = p->next;
			}
		}
		out << "工程完成时间：" << endl;
		out << (latest[topoSeq[this->vertexNum - 1]] = earliest[topoSeq[this->vertexNum - 1]]) << std::endl;
		//get latest time of vertexes
		size_t ri;
		for (size_t i = 0; i < this->vertexNum; i++)
		{
			ri = this->vertexNum - 1 - i;
			auto p = v[topoSeq[ri]].adjV;
			while (p)
			{
				if (latest[p->data.vi] - p->data.we < latest[topoSeq[ri]])
				{
					latest[topoSeq[ri]] = latest[p->data.vi] - p->data.we;
				}
				p = p->next;
			}
		}

		for (size_t i = 0; i < this->vertexNum; i++)
		{
			if (earliest[topoSeq[i]] == latest[topoSeq[i]])
			{
				critV[topoSeq[i]] = true;
			}
		}

		size_t curV = SIZE_MAX;
		size_t nextV = 0;

		if (1) {
			out << "所有关键事件（边）：" << endl;
			for (size_t i = 0; i < this->vertexNum; i++)
			{
				//if (critV[i])
				if (true)
				{
					auto p = v[i].adjV;
					while (p)
					{
						if (earliest[i] == latest[p->data.vi] - p->data.we)
						{
							out << v[i].v << "->" << v[p->data.vi].v << std::endl;
						}
						p = p->next;
					}
				}
			}
			out << std::endl;
		}

		if (1) {
			out << "其中一条关键路径：" << endl;
			for (size_t i = 0; i < this->vertexNum; i++)
			{
				if (!critV[topoSeq[i]])
					continue;
				curV = topoSeq[i];
				break;
			}
			while (true) {
				auto p = v[curV].adjV;
				while (p)
				{
					if (critV[p->data.vi])
					{
						nextV = p->data.vi;
						out << v[curV].v << "->" << v[nextV].v << std::endl;
						break;
					}
					p = p->next;
				}
				if (p)
				{
					curV = nextV;
				}
				else
				{
					break;
				}
			}
		}
		delete[] earliest;
		delete[] latest;
		delete[] critV;
	}
};

//最小生成树类的表项
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

//依据于某个Graph_List的最小生成树类
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

template <typename VertexElem, typename EdgeWeight>
void printMinSpanTree(ostream&out, MinSpanTree<VertexElem, EdgeWeight>& mst, Graph_List<VertexElem, EdgeWeight>& g)
{
	for (size_t i = 0; i < mst.n; i++)
	{
		out << g.getVertex(mst.arr[i].vs) << "-- (" << mst.arr[i].e << ") -->" << g.getVertex(mst.arr[i].ve) << endl;
	}
}

typedef enum { NONE, DIRECTED, DIRECTED_NET, UNDIRECTED, UNDIRECTED_NET } GraphType;

template <typename VertexElem, typename EdgeWeight>
std::istream & operator >> (std::istream & in, Graph_Matrix<VertexElem, EdgeWeight> &gm)
{
	if (gm.v)
		delete[]gm.v;

	GraphType gt;
	in >> reinterpret_cast<unsigned &>(gt);
	cout << reinterpret_cast<unsigned &>(gt) << endl;
	switch (gt)
	{
		case 1:
			gm.setDirected(true);
			gm.setWeighted(false);
			gm.setNonDirectWeight(1);
			break;
		case 2:
			gm.setDirected(true);
			gm.setWeighted(true);
			break;
		case 3:
			gm.setDirected(false);
			gm.setWeighted(false);
			gm.setNonDirectWeight(1);
			break;
		case 4:
			gm.setDirected(false);
			gm.setWeighted(true);
			break;
	}
	gm.setNonDirectWeight(1);
	gm.setInfinity((std::numeric_limits<EdgeWeight>::max)());

	size_t vCnt, eCnt;
	in >> vCnt >> eCnt;
	cout << vCnt << " " << eCnt << endl;

	gm.vertexNum = vCnt;

	gm.v = new VertexElem[vCnt];
	gm.e.init(vCnt, vCnt);

	for (size_t i = 0; i < vCnt; i++)
	{
		cin >> (gm.v[i]);
		cout << gm.v[i] << " ";
	}

	cout << endl;

	VertexElem vS, vE;
	EdgeWeight eW;

	eW = gm.defaultWeight;

	for (size_t i = 0; i < eCnt; i++)
	{
		in >> vS >> vE;
		cout << vS << " " << vE << " ";
		if (gm.weighted)
		{
			in >> eW;
			cout << eW << " ";
		}

		gm.e[gm.indexOfVertex(vS)][gm.indexOfVertex(vE)] = eW;
		if (!gm.directed)
		{
			gm.e[gm.indexOfVertex(vE)][gm.indexOfVertex(vS)] = eW;
		}
		cout << endl;
	}
	return in;
}

template <typename VertexElem, typename EdgeWeight>
std::istream & operator >> (std::istream & in, Graph_List<VertexElem, EdgeWeight> &ga)
{
	if (ga.v)
		delete[]ga.v;

	GraphType gt;
	in >> reinterpret_cast<unsigned &>(gt);
	cout << reinterpret_cast<unsigned &>(gt) << endl;
	switch (gt)
	{
		case 1:
			ga.setDirected(true);
			ga.setWeighted(false);
			ga.setNonDirectWeight(1);
			break;
		case 2:
			ga.setDirected(true);
			ga.setWeighted(true);
			break;
		case 3:
			ga.setDirected(false);
			ga.setWeighted(false);
			ga.setNonDirectWeight(1);
			break;
		case 4:
			ga.setDirected(false);
			ga.setWeighted(true);
			break;
	}
	ga.setNonDirectWeight(1);
	ga.setInfinity((std::numeric_limits<EdgeWeight>::max)());

	size_t vCnt, eCnt;
	in >> vCnt >> eCnt;
	cout << vCnt << " " << eCnt << endl;

	ga.vertexNum = vCnt;

	ga.v = new AdjListElem<VertexElem, EdgeWeight>[vCnt]();


	for (size_t i = 0; i < vCnt; i++)
	{
		in >> (ga.v[i].v);
		cout << (ga.v[i].v) << " ";
	}

	cout << endl;

	VertexElem vS, vE;
	EdgeWeight eW;

	eW = ga.defaultWeight;
	Node<WeightedDest<EdgeWeight>> *curNP, *newNP;

	size_t vsi;
	size_t vei;

	for (size_t i = 0; i < eCnt; i++)
	{
		in >> vS >> vE;
		cout << vS << " " << vE << " ";
		vsi = ga.indexOfVertex(vS);
		vei = ga.indexOfVertex(vE);
		if (ga.weighted)
		{
			in >> (eW);
			cout << (eW) << " ";
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
		cout << endl;
	}
	return in;
}


template<typename VertexElem_, typename EdgeWeight_>
std::ostream & operator<<(std::ostream & out, Graph_Matrix<VertexElem_, EdgeWeight_>& gm)
{
	gm.e.print(out);
	return out;
}

template<typename VertexElem_, typename EdgeWeight_>
std::ostream & operator<<(std::ostream & out, Graph_List<VertexElem_, EdgeWeight_>& ga)
{
	ga.printStruct(out);
	return out;
}

int main()
{

	stringstream s;
	cin.set_rdbuf(s.rdbuf());

	s << "\
1 4 4 abcd \
a b \
a c \
c d \
d a \
\
a \
y \
3 13 13 abcdefghijklm\
a b \
a c \
a f \
a l \
b m \
d e \
g h \
g i \
g k \
h k \
j l \
j m \
l m \
a \
y \
4 4 5 abcd \
a b 2 \
a c 2 \
a d 3 \
b c 4 \
c d 3 \
b \
y \
2 4 6 1234 \
1 2 2 \
2 3 2 \
2 4 1 \
1 3 5 \
3 4 3 \
1 4 4 \
1 \
y \
\
2 7 8 1234567 \
1 2 4 \
1 3 3 \
2 4 5 \
3 4 3 \
4 5 1 \
4 6 6 \
5 7 5 \
6 7 2 \
1 \
y \
2 4 6 1234 \
1 2 2 \
1 3 2 \
1 4 3 \
2 3 4 \
3 4 3 \
4 2 3 \
1 \
n \
	";
	s.seekg(ios::beg);

	while (true)
	{
		auto pos = s.tellg();
		if (1)
		{
			cout << " == 1. 邻接矩阵存储的图 ==" << endl;
			cout << " 请如此输入：" << endl;
			cout << "(1-有向图，2-有向网，3-无向图，4-无向网)" << endl;
			cout << "顶点数 边数" << endl;
			cout << "表示顶点的字符序列" << endl;
			cout << "（边）起始点 终点 [权值（若有）]" << endl;
			cout << "（边）起始点 终点 [权值（若有）]" << endl;
			cout << "……" << endl;
			Graph_Matrix<char, float> g0;
			cin >> g0;
			cout << "矩阵如下：" << endl;
			cout << g0;
			cout << endl;

			cout << "按照DFS遍历得到的遍历序列和得到的连通分量分类：" << endl;
			cout << g0.DFS_Components() << endl;
			cout << "按照BFS遍历得到的遍历序列和得到的连通分量分类：" << endl;
			cout << g0.BFS_Components() << endl;
		}

		s.seekg(pos);
		if (1)
		{
			cout << " == 2. 邻接表存储的图 ==" << endl;
			cout << " 请如此输入：" << endl;
			cout << "(1-有向图，2-有向网，3-无向图，4-无向网)" << endl;
			cout << "顶点数 边数" << endl;
			cout << "表示顶点的字符序列" << endl;
			cout << "（边）起始点 终点 [权值（若有）]" << endl;
			cout << "（边）起始点 终点 [权值（若有）]" << endl;
			cout << "……" << endl;
			Graph_List<char, float> g0;
			cin >> g0;
			cout << "邻接表组织结构如下：" << endl;
			cout << g0;
			cout << endl;

			cout << "按照DFS遍历得到的遍历序列和得到的连通分量分类：" << endl;
			cout << g0.DFS_Components() << endl;
			cout << "按照BFS遍历得到的遍历序列和得到的连通分量分类：" << endl;
			cout << g0.BFS_Components() << endl;
			if (g0.directed == false)
			{
				cout << "该无向图的最小生成树：" << endl;
				MinSpanTree<char, float> mst(g0);
				printMinSpanTree<char, float>(cout, mst, g0);
			}

			cout << "输入Dijkstra算法中的起始点" << endl;
			char c;
			inputTo(c);

			cout << "该图的Dijkstra算法得到的到该点的最短距离：" << endl;
			g0.output_dijkstra(cout, g0.indexOfVertex(c));

			if (g0.directed && g0.weighted)
			{
				size_t *seq = new size_t[g0.vertexNum];
				bool succeed = g0.Trim_TopoSort(seq);
				if (succeed)
				{
					cout << "该图用删边法获得拓扑序列：" << endl;
					for (size_t i = 0; i < g0.vertexNum; i++)
					{
						if (i)
							std::cout << " ";
						std::cout << g0.getVertex(seq[i]);
					}
					cout << endl;
					succeed = g0.DFS_TopoSort(seq);
					assert(succeed);
					cout << "该图用DFS获得拓扑序列：" << endl;
					for (size_t i = 0; i < g0.vertexNum; i++)
					{
						if (i)
							std::cout << " ";
						std::cout << g0.getVertex(seq[i]);
					}
					cout << endl;
					cout << "打印关键事件和关键路径：" << endl;
					g0.printCritPath(std::cout, seq);
				}
				else
				{
					cout << "该图有圈。" << endl;
				}
				
				delete[] seq;
			}
		}
		char c;
		cout << "重新开始？" << endl;
		inputTo(c);
		if (c != 'y')
		{
			break;
		}
	}

	return 0;
}

