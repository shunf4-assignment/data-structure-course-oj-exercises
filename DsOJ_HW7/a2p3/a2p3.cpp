#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <climits>


using std::stringstream;
using std::cin;
using std::cout;
using std::endl;

template <typename VertexElem, typename EdgeWeight>
class Graph
{
public:
	bool directed;
	bool weighted;
	size_t vertexNum;
	EdgeWeight defaultWeight;
	EdgeWeight infinity;

	Graph()
	{
		setDirected(false);
		setWeighted(false);
		setNonDirectWeight(1);
		vertexNum = 0;
		setInfinity(INT_MAX);
	}

	Graph(bool directed, bool weighted, EdgeWeight defaultWeight)
	{
		setDirected(directed);
		setWeighted(weighted);
		setNonDirectWeight(defaultWeight);
		vertexNum = 0;
		setInfinity(INT_MAX);
	}

	void setNonDirectWeight(EdgeWeight defaultWeight)
	{
		this->defaultWeight = defaultWeight;
	}

	void setInfinity(EdgeWeight infinityWeight)
	{
		this->infinity = infinityWeight;
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

	typedef enum { NOTVISITED, VISITING, VISITED } visitStatus;
	struct edgeDrawnStruct
	{
		size_t edgeNum;
		bool *edgeDrawn;
		edgeDrawnStruct() {
			edgeDrawn = NULL;
		}

		void init(size_t edgeNum_)
		{
			edgeNum = edgeNum_;
			if (edgeNum != 0) {
				edgeDrawn = new bool[edgeNum];
				for (size_t i = 0; i < edgeNum; i++)
				{
					edgeDrawn[i] = false;
				}
			}
			else
				edgeDrawn = NULL;
		}

		~edgeDrawnStruct()
		{
			if (edgeDrawn)
				delete[]edgeDrawn;
		}

		bool& operator[](size_t i)
		{
			return edgeDrawn[i];
		}
	};

	void DFS_eulerianPath()
	{
		const size_t maxEdgeNum = 100000;
		visitStatus *visited = new visitStatus[Graph<VertexElem, EdgeWeight>::vertexNum];
		VertexElem *stack = new VertexElem[maxEdgeNum];
		edgeDrawnStruct *edgeDrawn = new edgeDrawnStruct[this->vertexNum];
		for (size_t i = 0; i < this->vertexNum; i++)
		{

			auto p = v[i].adjV;
			size_t edgeNum = 0;
			while (p)
				edgeNum++, p = p->next;
			edgeDrawn[i].init(edgeNum);
		}
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			stack[0] = v[i].v;
#ifdef _FS_DEBUG_OUTPUT
			cout << "Now the starting vertex is : " << v[i].v << endl;
#endif
			DFS_eulerianPath_do(i, stack, 1, edgeDrawn, true);

			//to pass the oj
			break;
		}

		delete[] visited;
		delete[]stack;
		delete[]edgeDrawn;
	}

	void DFS_eulerianPath_do(size_t i, VertexElem *stack, size_t stackPos, edgeDrawnStruct *edgeDrawn, bool externalInvoke)
	{
		size_t edgeNum;
		size_t edgeNum_otherside;
		size_t nextVert;
#ifdef _FS_DEBUG_OUTPUT
		cout << "Checking all edges for " << v[i].v << endl;
#endif

		auto p = v[i].adjV;
		decltype(p) q = NULL;
		edgeNum = 0;
		while (p)
		{
			nextVert = p->data.vi;
#ifdef _FS_DEBUG_OUTPUT
			cout << "Checking Edge " << edgeNum << " : to " << v[nextVert].v << endl;
#endif
			if (edgeDrawn[i][edgeNum] == false)
			{
#ifdef _FS_DEBUG_OUTPUT
				cout << "Edge " << edgeNum << " is not being used." << endl;
#endif
				break;
			}

			p = p->next;
			edgeNum++;
		}

		if (p == NULL)
		{
#ifdef _FS_DEBUG_OUTPUT
			cout << "p is NULL. " << endl;
#endif
			bool allEdgesDrawn = true;
			for (size_t i = 0; i < this->vertexNum; i++)
			{
#ifdef _FS_DEBUG_OUTPUT
				cout << "Vertex " << v[i].v << ":";
#endif
				for (size_t j = 0; j < edgeDrawn[i].edgeNum; j++)
				{
#ifdef _FS_DEBUG_OUTPUT
					cout << edgeDrawn[i][j];
#endif
					if (edgeDrawn[i][j] == false)
						allEdgesDrawn = false;
				}
#ifdef _FS_DEBUG_OUTPUT
				cout << endl;
#endif
			}
			if (allEdgesDrawn)
			{
#ifdef _FS_DEBUG_OUTPUT
				cout << endl << "All Edge Drawn. " << endl;
#endif
				for (size_t i = 0; i < stackPos; i++)
				{
					std::cout << stack[i];
				}
				std::cout << std::endl;
			}
			return;
		}

		edgeDrawn[i][edgeNum] = true;

		if (!this->directed)
		{
#ifdef _FS_DEBUG_OUTPUT
			cout << "Checking opposite edges from " << v[nextVert].v << endl;
#endif
			q = v[nextVert].adjV;
			edgeNum_otherside = 0;
			while (q)
			{
#ifdef _FS_DEBUG_OUTPUT
				cout << "Checking edge " << edgeNum_otherside << " to " << v[q->data.vi].v << endl;
#endif
				if (edgeDrawn[nextVert][edgeNum_otherside] == false)
					if(q->data.vi == i)
					{
	#ifdef _FS_DEBUG_OUTPUT
						cout << "Edge " << edgeNum_otherside << " to " << v[q->data.vi].v << " is an opposite edge. " << endl;
	#endif
						break;
					}
				q = q->next;
				edgeNum_otherside++;
			}
			if (q == NULL)
			{
				throw "NO ANOTHER OPPOSITE EDGE FOR AN UNDIRECTED GRAPH";
			}
			edgeDrawn[nextVert][edgeNum_otherside] = true;

		}

		stack[stackPos] = v[nextVert].v;
#ifdef _FS_DEBUG_OUTPUT
		cout << v[nextVert].v << " pushed into stack." << endl;
		for (size_t i = 0; i < stackPos + 1; i++)
		{
			cout << stack[i] << " ";
		}
		cout << endl;
#endif

		while (true)
		{
#ifdef _FS_DEBUG_OUTPUT
			cout << "Entering next Iteration " << stackPos + 1 << endl;
#endif
			DFS_eulerianPath_do(nextVert, stack, stackPos + 1, edgeDrawn, false);
#ifdef _FS_DEBUG_OUTPUT
			cout << "Exiting Iteration " << stackPos + 1 << endl;
#endif

			edgeDrawn[i][edgeNum] = false;
			if (!this->directed)
				edgeDrawn[nextVert][edgeNum_otherside] = false;

			p = p->next;
			edgeNum++;

			while (p)
			{
				nextVert = p->data.vi;
#ifdef _FS_DEBUG_OUTPUT
				cout << "Checking Edge " << edgeNum << " : to " << nextVert << endl;
#endif
				if (edgeDrawn[i][edgeNum] == false)
				{
#ifdef _FS_DEBUG_OUTPUT
					cout << "Edge " << edgeNum << " is not being used." << endl;
#endif
					break;
				}

				p = p->next;
				edgeNum++;

			}

			if (p == NULL)
			{
#ifdef _FS_DEBUG_OUTPUT
				cout << "p is NULL. " << endl;
#endif
				bool allEdgesDrawn = true;
				for (size_t i = 0; i < this->vertexNum; i++)
				{
#ifdef _FS_DEBUG_OUTPUT
					cout << "Vertex " << v[i].v << ":";
#endif
					for (size_t j = 0; j < edgeDrawn[i].edgeNum; j++)
					{
#ifdef _FS_DEBUG_OUTPUT
						cout << edgeDrawn[i][j];
#endif
						if (edgeDrawn[i][j] == false)
							allEdgesDrawn = false;
					}
#ifdef _FS_DEBUG_OUTPUT
					cout << endl;
#endif
				}
				if (allEdgesDrawn)
				{
#ifdef _FS_DEBUG_OUTPUT
					cout << endl << "All Edge Drawn. " << endl;
#endif
					for (size_t i = 0; i < stackPos; i++)
					{
						std::cout << stack[i];
					}
					std::cout << std::endl;
				}
				return;
			}

			edgeDrawn[i][edgeNum] = true;

			if (!this->directed)
			{
#ifdef _FS_DEBUG_OUTPUT
				cout << "Checking opposite edges from " << v[nextVert].v << endl;
#endif
				q = v[nextVert].adjV;
				edgeNum_otherside = 0;
				while (q)
				{
#ifdef _FS_DEBUG_OUTPUT
					cout << "Checking edge " << edgeNum_otherside << " to " << v[q->data.vi].v << endl;
#endif
					if (edgeDrawn[nextVert][edgeNum_otherside] == false && q->data.vi == i)
					{
#ifdef _FS_DEBUG_OUTPUT
						cout << "Edge " << edgeNum_otherside << " to " << v[q->data.vi].v << " is an opposite edge. " << endl;
#endif
						break;
					}
					q = q->next;
					edgeNum_otherside++;
				}
				if (q == NULL)
				{
					throw "NO ANOTHER OPPOSITE EDGE FOR AN UNDIRECTED GRAPH";
				}
				edgeDrawn[nextVert][edgeNum_otherside] = true;
			}

			stack[stackPos] = v[nextVert].v;
#ifdef _FS_DEBUG_OUTPUT
			cout << v[nextVert].v << " pushed into stack." << endl;
			for (size_t i = 0; i < stackPos + 1; i++)
			{
				cout << stack[i] << " ";
			}
			cout << endl;
#endif
		}
		return;
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
	gm.setInifinity(INT_MAX);

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
	ga.setWeighted(false);
	ga.setNonDirectWeight(1);

	size_t vCnt, eCnt;
	in >> vCnt >> eCnt;

	ga.vertexNum = vCnt;

	ga.v = new AdjListElem<VertexElem, EdgeWeight>[vCnt]();


	for (size_t i = 0; i < vCnt; i++)
	{
		ga.v[i].v = static_cast<VertexElem>(i) + 1;
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

//#define FS_INFILE
//#define FS_OUTFILE
int main()
{
#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	std::ifstream f0;
	f0.open("a2p2.txt", std::ios::in);
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
		std::stringstream sin;

		sin << "5 8" << endl;

		sin << "5 4" << endl;
		sin << "5 3" << endl;
		sin << "4 3" << endl;
		sin << "2 5" << endl;
		sin << "2 3" << endl;
		sin << "1 5" << endl;
		sin << "1 3" << endl;
		sin << "1 2" << endl;

		sin.seekg(std::ios::beg);

		Graph_List<int, int> g;
		sin >> g;
		g.DFS_eulerianPath();

	}


#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	f0.close();
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))
	f1.close();
#endif
	return 0;
}