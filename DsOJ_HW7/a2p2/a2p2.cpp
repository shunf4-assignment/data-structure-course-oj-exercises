/*1652270 CS2 fengshun*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <climits>

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
		for ( k = 0; k < this->vertexNum; k++)
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
				//std::cout << topoSeq[i] + 1 <<
					" ";
			}
			
		}
		//std::cout << std::endl;
		size_t curVI = 0;
		while (curVI < this->vertexNum && critV[topoSeq[curVI]] == false)
			curVI++;
		size_t curS = topoSeq[curVI];

		while (curVI < this->vertexNum)
		{
			curVI++;
			while (curVI < this->vertexNum && critV[topoSeq[curVI]] == false)
				curVI++;
			auto p = v[curS].adjV;
			while (p)
			{
				if (p->data.vi == topoSeq[curVI])
				{
					break;
				}
				p = p->next;
			}
			if (p != NULL)
			{
				out << v[curS].v +1 << "->" << v[p->data.vi].v +1 << std::endl;
				curS = p->data.vi;
			}
		}
		delete[] earliest;
		delete[] latest;
		delete[] critV;
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

	ga.setDirected(true);
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
		Graph_List<int, int> g;
		std::cin >> g;
		size_t *seq = new size_t[g.vertexNum];
		bool succeed = g.DFS_TopoSort(seq);
		if (!succeed)
		{
			std::cout << 0 << std::endl;
		}
		else
		{
			/*if (succeed)
			{
				for (size_t i = 0; i < g.vertexNum; i++)
				{
					if (i)
						std::cout << " ";
					std::cout << g.getVertex(seq[i]) + 1;
				}
				std::cout << std::endl;
			}*/
			g.printCritPath(std::cout, seq);
		}
		/**/
		delete[]seq;
	}


#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	f0.close();
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))
	f1.close();
#endif
	return 0;
}

