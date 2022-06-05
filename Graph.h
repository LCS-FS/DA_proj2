///\file
/// Graph, Vertex and Edge Classes aswell as the algorithms for each problem

/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "MutablePriorityQueue.h"
#include <set>
#include <stack>
#include <map>
#include "minHeap.h"

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<int>::max()
#define NINF std::numeric_limits<int>::min()

/************************* Vertex  **************************/

template <class T>
class Vertex {
    T info;                // contents
    std::vector<Edge<T> > adj;  // outgoing edges
    bool visited;          // auxiliary field
    int dist = 0;
    Vertex<T> *path = nullptr;
    int queueIndex = 0; 		// required by MutablePriorityQueue

    void addEdge(Vertex<T> *dest, int dur, int c, int w);
    int lt, et;
    int cap;

public:
    Vertex(T in);
    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
    T getInfo() const;
    int getDist() const;
    Vertex *getPath() const;
    friend class Graph<T>;
    friend class MutablePriorityQueue<Vertex<T>>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, int dur, int c, int w) {
    adj.push_back(Edge<T>(d, dur, c, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
int Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
    return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
    Vertex<T> * dest;      // destination vertex
    int weight;
    // edge weight -> residual capacity
    int capacity;
    int duration;
    int flux;
    

public:
    int getWeight() const;
    void setWeight(int weight);
    int getCapacity() const;
    void setCapacity(int capacity);
    int getFlux() const;
    void setFlux(int flux);
    int getDuration() const;
    void setDuration(int duration);
    Edge(Vertex<T> *d, int duration, int c, int w);
    friend class Graph<T>;
    friend class Vertex<T>;
    bool operator<(const Edge<T> & edge) const;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, int duration, int c, int w): dest(d), weight(w), capacity(c), duration(duration), flux(0){}


/*************************** Graph  **************************/

template <class T>
class Graph {
private:
    int numberNodes, numberEdges;
    std::vector<Vertex<T> *> vertexSet;    // vertex set
    std::vector<Vertex<T> *> vertex;

    //Fp05
    Vertex<T> * initSingleSource(const T &orig);
    bool relax(Vertex<T> *v, Vertex<T> *w, int weight);
    int ** W = nullptr;   // dist
    int **P = nullptr;   // path
    int findVertexIdx(const T &in) const;

public:
    map<vector<T>, int> paths;
    ~Graph();
    void printPath(std::map<vector<T>, T> printablePath); // prints a string meaning n subjects go through path
    Vertex<T> *findVertex(const T &in) const;
    bool addVertex(const T &in);
    bool addEdge(const T &sourc, const T &dest, int d, int c, int w);
    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;
    std::map<vector<T>, T> FindPathGivenGroupSize(T st, T ta, int groupSize);
    int getNumberNodes() const;
    int getNumberEdges() const;
    int firstAlgorithm(T start, T end);
    bool heapComp(const Vertex<T>* v1,const Vertex<T>* v2) const;
    void setNumberNodes(int numberNodes);
    void setNumberEdges(int numberEdges);
    void paretoOptimalGroupSizeAndTransportShift(T origin, T target);
    int recursivePathFinderLimited(T current, T target, vector<T> currentPath, int maxCapEdges, int bfsCap, int currentCap);

    // Single-source shortest path - Greedy
    void dijkstraShortestPath(const T &s);
    void unweightedShortestPath(const T &s);

    // FP03B - Single-shource shortest path - Dynamic Programming - Bellman-Ford
    void bellmanFordShortestPath(const T &s); //TODO...
    std::vector<T> getPath(const T &origin, const T &dest) const; //TODO...

    // FP03B - All-pair shortest path -  Dynamic Programming - Floyd-Warshall
    void floydWarshallShortestPath(); //TODO...
    std::vector<T> getfloydWarshallPath(const T &origin, const T &dest) const; //TODO...

    int edmondKarpFlux(T st, T ta);
    Graph<T> residualGrid();
    void zeroFlux();

    int increaseGroupSize(T st, T ta, int inc);
    void auxTest2_4();

    void topoSort(T st, std::stack<Vertex<T>*> &stack);
    int longestPath(T st, T ta);

    void printGraph();

    void vertexTime(T st, T ta);
    vector<vector<T>> capacityOrEdges(T st, T ta);
    map<vector<int>, int> filterPathsByDominance();
};

template<class T>
int Edge<T>::getDuration() const {
    return duration;
}

template<class T>
void Edge<T>::setDuration(int duration) {
    Edge::duration = duration;
}

template<class T>
int Edge<T>::getFlux() const {
    return flux;
}

template<class T>
void Edge<T>::setFlux(int flux) {
    Edge::flux = flux;
}

template<class T>
int Edge<T>::getWeight() const {
    return weight;
}

template<class T>
void Edge<T>::setWeight(int weight) {
    Edge::weight = weight;
}

template<class T>
int Edge<T>::getCapacity() const {
    return capacity;
}

template<class T>
void Edge<T>::setCapacity(int capacity) {
    Edge::capacity = capacity;
}


template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->info == in)
            return v;
    return nullptr;
}

/*
 * Finds the index of the vertex with a given content.
 */
template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->info == in)
            return i;
    return -1;
}
/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, int d, int c, int w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, d, c, w);
    return true;
}


/**************** Single Source Shortest Path algorithms ************/

/**
 * Initializes single source shortest path data (path, dist).
 * Receives the content of the source vertex and returns a pointer to the source vertex.
 * Used by all single-source shortest path algorithms.
 */
template<class T>
Vertex<T> * Graph<T>::initSingleSource(const T &origin) {
    for(auto v : vertexSet) {
        v->dist = INF;
        v->path = nullptr;
        v->visited = false;
    }
    auto s = findVertex(origin);
    s->dist = 0;
    s->visited = true;
    return s;
}

/**
 * Analyzes an edge in single source shortest path algorithm.
 * Returns true if the target vertex was relaxed (dist, path).
 * Used by all single-source shortest path algorithms.
 */
template<class T>
inline bool Graph<T>::relax(Vertex<T> *v, Vertex<T> *w, int weight) {
    if (v->dist + weight < w->dist) {
        w->dist = v->dist + weight;
        w->path = v;
        return true;
    }
    else
        return false;
}

template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) { //uses duration instead of weight
    auto s = initSingleSource(origin);
    MutablePriorityQueue<Vertex<T>> q;
    q.insert(s);
    while( ! q.empty() ) {
        auto v = q.extractMin();
        for(Edge<T> e : v->adj) {
            auto oldDist = e.dest->dist;
            if (relax(v, e.dest, e.duration)) {
                if (oldDist == INF)
                    q.insert(e.dest);
                else
                    q.decreaseKey(e.dest);
            }
        }
    }
}

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {
    auto s = initSingleSource(orig);
    std::queue< Vertex<T>* > q;
    q.push(s);
    s->visited = true;
    while( ! q.empty() ) {
        auto v = q.front();
        q.pop();
        for(auto e: v->adj)
            if (relax(v, e.dest, 1)) {
                q.push(e.dest);
                e.dest->visited = true;
            }
    }
}


template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
    // TODO implement this
}

///Determines path based on destination and origin
///Steps back from destination's path to previous vertex until it reaches the origin vertex
///\param origin number associated with Origin Vertex
///\param dest number associated with Destination Vertex
///@return Vector of numbers associated with Vertexes in path, in order
template<class T>
std::vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
    std::vector<T> res;
    Vertex<T> pred = *findVertex(dest); //pred = dest
    if(!pred.visited) return res;
    res.push_back(pred.getInfo());
    while(pred.info != origin){
        pred = *(pred.path);
        res.push_back(pred.getInfo());
    }
    //reverse vector
    std::reverse(res.begin(), res.end());

    return res;
}


/**************** All Pairs Shortest Path  ***************/

template <class T>
void deleteMatrix(T **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

template <class T>
Graph<T>::~Graph() {
    deleteMatrix(W, vertexSet.size());
    deleteMatrix(P, vertexSet.size());
}

template<class T>
void Graph<T>::floydWarshallShortestPath() {
    //implement this
}


template<class T>
std::vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{
    std::vector<T> res;
    //implement this
    return res;
}

template<class T>
int Graph<T>::getNumberNodes() const {
    return numberNodes;
}

template<class T>
int Graph<T>::getNumberEdges() const {
    return numberEdges;
}

template<class T>
void Graph<T>::setNumberNodes(int numberNodes) {
    Graph::numberNodes = numberNodes;
}

template<class T>
void Graph<T>::setNumberEdges(int numberEdges) {
    Graph::numberEdges = numberEdges;
}

///Algorithm to calculate the maximum size of a group that can travel separately
///Based on the Edmond Karp variant of the Ford Fulkerson method for determining maximum flow
///Sets appropriate flux for each edge
///\param st number associated with start vertex
///\param ta number associated with target vertex
///@return maximum group size for the graph
template<class T>
int Graph<T>::edmondKarpFlux(T st, T ta) {
    Vertex<T> origin(st);
    std::vector<T> path;
    int resCap = INF;
    Graph<T> resGrid;

    zeroFlux();

    //determine residual grid
    resGrid = residualGrid();
    resGrid.unweightedShortestPath(st);
    path = resGrid.getPath(st, ta);
    Vertex<T> destination = *(resGrid.findVertex(ta));

    //while there is a path in the Residual Grid
    while(destination.visited){

        //find minimun Cf in path
        for(int i = 0; i < path.size() -1; i++){
            for(Edge<T> edge: (resGrid.findVertex(path[i]))->adj){
                //found the edge of the path
                if(edge.dest->info == resGrid.findVertex(path[i+1])->info){
                    resCap = std::min(edge.getWeight(), resCap);
                }
            }
        }

        for(int i = 0; i < path.size() -1; i++){
            for(Edge<T> &edge: findVertex(path[i])->adj){
                //found the edge of the path
                if(edge.dest->info == findVertex(path[i+1])->info){
                    edge.setFlux(resCap + edge.getFlux());
                }
            }
        }

        //determine residual grid
        resGrid = residualGrid();
        resGrid.unweightedShortestPath(st);
        path = resGrid.getPath(st, ta);
        destination = *(resGrid.findVertex(ta));
    }

    origin = *(findVertex(st));
    int maxFlux = 0;
    for(Edge<T> edge: origin.adj){
        maxFlux+= edge.getFlux();
    }
    return maxFlux;
}
///Creates a residual grid graph based on the current graph
///@return graph object of the residual grid
template<class T>
Graph<T> Graph<T>::residualGrid() {
    Graph<T> residualGrid;
    //add all vertexes
    for(auto v: vertexSet){
        residualGrid.addVertex(v->info);
    }
    for(Vertex<T>* v: vertexSet){
        for(Edge<T> edge : v->adj){
            //Cf(u,v)
            if(edge.capacity - edge.getFlux() > 0) residualGrid.addEdge(v->info, edge.dest->info, edge.duration, edge.capacity, edge.capacity - edge.getFlux());
        }
    }

    for(auto v: vertexSet){
        for(Edge<T> edge : v->adj){
            //Cf(v,u)
            if(edge.getFlux() > 0) residualGrid.addEdge(edge.dest->info ,v->info, edge.duration, edge.capacity, edge.getFlux());
        }
    }
    return residualGrid;
}
///Sets flux of every edge on the graph to zero
template<class T>
void Graph<T>::zeroFlux() {
    for(auto v: vertexSet){
        for(int i = 0; i < v->adj.size(); i++){
            v->adj[i].setFlux(0);
        }
    }
}

///Algorithm to increase group size based on previous paths
///Based on the Edmond Karp variant of the Ford Fulkerson method for determining maximum flux
///Sets appropriate flux for each edge
///\param st number associated with start vertex
///\param ta number associated with target vertex
///\param inc amount to increase group size by
///@return how much the group size was increased by or -1 if it cant be increased by the desired amount
template<class T>
int Graph<T>::increaseGroupSize(T st, T ta, int inc) {
    Vertex<T> origin = *(findVertex(st));
    std::vector<T> path;
    int resCap = INF;
    Graph<T> resGrid;

    //get initial flux
    int initialFlux = 0;
    for(Edge<T> edge: origin.adj){
        initialFlux += edge.getFlux();
    }
    int newFlux = initialFlux;

    //determine residual grid
    resGrid = residualGrid();
    resGrid.unweightedShortestPath(st);
    path = resGrid.getPath(st, ta);
    Vertex<T> destination = *(resGrid.findVertex(ta));

    //while there is a path in the Residual Grid
    while((newFlux - initialFlux < inc) && destination.visited){

        //find minimun Cf in path
        for(int i = 0; i < path.size() -1; i++){
            for(Edge<T> edge: (resGrid.findVertex(path[i]))->adj){
                //found the edge of the path
                if(edge.dest->info == resGrid.findVertex(path[i+1])->info){
                    resCap = std::min(edge.getWeight(), resCap);
                }
            }
        }

        for(int i = 0; i < path.size() -1; i++){
            for(Edge<T> &edge: findVertex(path[i])->adj){
                //found the edge of the path
                if(edge.dest->info == findVertex(path[i+1])->info){
                    edge.setFlux(resCap + edge.getFlux());
                }
            }
        }

        //determine residual grid
        resGrid = residualGrid();
        resGrid.unweightedShortestPath(st);
        path = resGrid.getPath(st, ta);
        destination = *(resGrid.findVertex(ta));

        //update origin
        origin = *(findVertex(st));
        //determine current max-flux
        newFlux = 0;
        for(Edge<T> edge: origin.adj){
            newFlux+= edge.getFlux();
        }
    }
    //if it reaches full flux before increasing enough, it means its impossible to increase by the desired amount
    if(newFlux - initialFlux < inc) return -1;
    return newFlux - initialFlux;
}

///Prints the caracteristics of all edges in the graph
///Show origin, destination, flux and capacity
/*
template<class T>
void Graph<T>::printGraph(){
    std::set<std::pair<int, int>> printed;
    for(auto v: vertexSet){
        for(Edge<T> edge: v->adj){
            //if has not been printed yet
            if(printed.find(std::pair(v->info, edge.dest->info) == printed.end()){
                std::cout << "Source: " << v->info << " Destination: " << edge.dest->info
                << " Flux: " << edge.getFlux() << " Capacity: " << edge.getCapacity() << std::endl;
                //add to set
                printed.insert(std::pair(v->info, edge.dest->info));
            }
        }
    }
}*/
///Computes the highest capacity path in the graph
///Works for unseperable groups
///Based on the algorithm for higest capacity path in the theory slides
///\param st number associated with start vertex
///\param ta number associated with target vertex
///@returns the minimum capacity, in the highest capacity path in the graph
template<class T>
int Graph<T>::firstAlgorithm(T start, T end) {
    MinHeap<Vertex<T>*, int> heap(numberNodes, nullptr);
    for(Vertex<T>* v: vertexSet){
        v->path = nullptr;
        v->cap = 0;
        v->visited = true;
        heap.insert(v, -(v->cap));
    }
    Vertex<T> * origin = findVertex(start);
    origin->cap = INF;
    heap.decreaseKey(origin, -INF);
    while(heap.getSize() > 0){
       Vertex<T>* vec = heap.removeMin();

        //std::pop_heap(heap.begin(), heap.end());
        for(Edge<T> edge: vec->adj){
            if(std::min(vec->cap,edge.capacity) > edge.dest->cap){
                edge.dest->cap = std::min(vec->cap,edge.capacity);
                edge.dest->path = vec;
                heap.decreaseKey(edge.dest, -(edge.dest->cap));
            }
        }
    }
    
    Vertex<T>* vertex;
    int mincap = INF;
    vector<T> path = getPath(start, end);
    for(T info : path){
        vertex = findVertex(info);
        mincap = std::min(mincap, vertex->cap);
    }

    return mincap;
}

template<class T>
bool Edge<T>::operator<(const Edge<T> & edge) const{
    return capacity < edge.getCapacity();
}

///Computes the longest path, in duration, between 2 nodes
///Similar to dijkstra's algorithm for shortest distance paths, using topological sort
///Should be used after a flux setting algorithm
///\param st number associated with start vertex
///\param ta number associated with target vertex
///@returns longest duration between ta and st nodes
template<class T>
int Graph<T>::longestPath(T st, T ta) {
    std::stack<Vertex<T>*> stack;
    //set all as not visited
    for(Vertex<T>* vertex: vertexSet){
        vertex->visited = false;
    }
    //store topological sort
    for(Vertex<T>* vertex: vertexSet){
        if(!(vertex->visited)) topoSort(vertex->info, stack);
    }

    //set all distances to infinite
    for(Vertex<T>* vertex: vertexSet){
        vertex->dist = NINF;
    }
    Vertex<T>* origin = findVertex(st);
    origin->dist = 0;

    Vertex<T>* dest;
    //process verteses in topological order
    while(stack.size()>0){
        Vertex<T>* node = stack.top();
        stack.pop();

        //adjacent
        if(node->dist != NINF){
            for(Edge<T> edge : node->adj){
                if(edge.flux != 0){
                    dest = edge.dest;
                    if(dest->dist < node->dist + edge.duration){
                        dest->dist = node->dist + edge.duration;
                        dest->path = findVertex(node->info);
                    }
                }
            }
        }
    }
    Vertex<T>* target = findVertex(ta);
    return target->dist;
}

///Inserts nodes on a stack, in topological order
///\param st number associated with start vertex
///\param reference to a stack of pointers of nodes, where the nodes will be inserted in
template<class T>
void Graph<T>::topoSort(T st, std::stack<Vertex<T>*> &stack) {
    Vertex<T>* origin = findVertex(st);
    origin->visited = true;
    for(Edge<T> edge : origin->adj){
        if(!(edge.dest->visited) && (edge.flux != 0)) topoSort((edge.dest)->info, stack);
    }
    stack.push(origin);
}

///Computes the earliest and latest arrival at every node
///Prints every node where people must wait for other people and how long they wait for
///Dijkstra's algorithm is used to compute earliest arrival
///LongestPath algorithm is used to compute latest arrival
template<class T>
void Graph<T>::vertexTime(T st, T ta) {
    dijkstraShortestPath(st);
    for(Vertex<T>* v : vertexSet){
        v->et = v->dist;
    }
    longestPath(st, ta);
    for(Vertex<T>* v : vertexSet){
        v->lt = v->dist;
    }

    int maxD = 0;
    std::vector<T> biggest;
    for(Vertex<T>* v : vertexSet){
        if(((v->lt) - (v->et) != 0) && ((v->lt) > NINF) && ((v->et) != INF)){
            std::cout << "Vertex: " << v->info << " Waiting: " << (v->lt) - (v->et) << std::endl;
            if((v->lt) - (v->et) > maxD){
                maxD = (v->lt) - (v->et);
                biggest.erase(biggest.begin(), biggest.end());
                biggest.push_back(v->info);
            }
            else if((v->lt) - (v->et) == maxD){
                biggest.push_back(v->info);
            }
        }
    }
    std::cout << "Biggest waiting time: " << maxD << '\n';
    for(auto node : biggest){
        std::cout << "Node: " << node << '\n';
    }
}

///Algorithm to calculate the paths for a splittable group of a given size
///Based on the Edmond Karp variant of the Ford Fulkerson method for determining maximum flow
///Sets appropriate flux for each edge
///\param st number associated with start vertex
///\param ta number associated with target vertex
///\param groupSize desired group size
///@return map of the paths the group should take
template<class T>
std::map<vector<T>, T> Graph<T>::FindPathGivenGroupSize(T st, T ta, int groupSize) {
    Vertex<T> origin(st);
    std::vector<T> path;
    std::vector<T> pathInfo;
    int resCap = INF;
    Graph<T> resGrid;
    std::map<vector<T>, T> printablePath;

//    zeroFlux();
    for(Vertex<T>* v: vertexSet){
        for(int i = 0; i < v->adj.size(); i++){
            v->adj[i].setFlux(0);
        }
    }

    resGrid = residualGrid();
    resGrid.unweightedShortestPath(st);
    path = resGrid.getPath(st, ta);

    //while there is a path in the Residual Grid
    while(groupSize != 0){
        resGrid = residualGrid();
        resGrid.unweightedShortestPath(st);
        path = resGrid.getPath(st, ta);
        if (path.empty()) {
            cout << "Couldn't find a path for the whole group. The biggest possible group's path goes as follows:\n";
            break;
        }

        //find minimun Cf in path
        for(int i = 0; i < path.size() -1; i++){
            for(Edge<T> edge: (resGrid.findVertex(path[i]))->adj){
                //found the edge of the path
                if(edge.dest->info == resGrid.findVertex(path[i+1])->info){
                    resCap = std::min(edge.getWeight(), resCap);
                }
            }
        }

        for(int i = 0; i < path.size() - 1; i++){
            for(Edge<T> &edge: findVertex(path[i])->adj){
                //found the edge of the path
                if(edge.dest->info == findVertex(path[i+1])->info){
                    edge.setFlux(resCap + edge.getFlux());
                }
            }
        }

        if (resCap > groupSize) {
            resCap = groupSize;
        }

        for (int i = 0; i < path.size(); i++) {
            pathInfo.push_back(path[i]);
        }

        if (printablePath.find(pathInfo) == printablePath.end()) {
            printablePath.insert(std::pair<vector<int>, int>(pathInfo, resCap));
        } else {
            printablePath.find(pathInfo)->second += resCap;
        }

        pathInfo.erase(pathInfo.begin(), pathInfo.end());

        groupSize -= resCap;
    }

    return printablePath;
}
///Calculates best paths for a unsplittable group based on capacity and number of nodes
///If multiple paths aren't better than the others in both capacity and number of nodes, all of them are returned
///\param st number associated with start vertex
///\param ta number associated with target vertex
///@return vector of paths as a vector of nodes T
template<class T>
vector<vector<T>> Graph<T>::capacityOrEdges(T st, T ta) {
    vector<T> bfsVec, maxCapVec;
    vector<vector<T>> res;
    int bfsEdges, maxCapEdges, bfsCap=INF, maxCapCap;
    unweightedShortestPath(st);
    bfsVec = getPath(st, ta);
    bfsEdges = bfsVec.size();
    for(T node: bfsVec){
        Vertex<T> * v = findVertex(node);
        bfsCap = std::min(bfsCap, v->cap);
    }

    maxCapCap = firstAlgorithm(st, ta);
    maxCapVec = getPath(st, ta);
    maxCapEdges = maxCapVec.size();
    if(bfsEdges == maxCapEdges && bfsCap == maxCapCap){ //equal
        res.push_back(bfsVec);
        res.push_back(maxCapVec);
    }
    else if(bfsEdges != maxCapEdges){ //diferent edges means bfs has less
        if(bfsCap == maxCapCap){ //means bfs is better in edges and the same in cap
            res.push_back(bfsVec);
        }
        else{ //diferent cap means bfs is better in edges and maxCap is better in cap
            res.push_back(bfsVec);
            res.push_back(maxCapVec);
        }
    }
    else{ //equal edges means equals or maxCap is better
        if(bfsCap == maxCapCap){ //bfs is better in edges and the same in cap
            res.push_back(bfsVec);
            res.push_back(maxCapVec);
        }
        else{ //means maxCap has better cap and same edges as bfs so maxcap is better
            res.push_back(maxCapVec);
        }
    }
    return res;
}

///Prints all the paths and how many people go through each of them
///\param printablePath a map of paths, as a vector of nodes T, and number of people that can go through them
template<class T>
void Graph<T>::printPath(map<vector<T>, T> printablePath)
 {
     for (auto x : printablePath) {
         cout << x.second << " subjects go through this path: ";
         for (auto i : x.first) {
             cout << i << ", ";
         }
         cout << "arrived." << std::endl;
     }
 }

template<class T>
void Graph<T>::paretoOptimalGroupSizeAndTransportShift(T origin, T target) {
    vector<T> bfsVec, maxCapVec;
    vector<vector<T>> res;
    int bfsEdges, maxCapEdges, bfsCap=INF, maxCapCap;
    unweightedShortestPath(origin);
    bfsVec = getPath(origin, target);
    bfsEdges = bfsVec.size();
    for(int i = 0; i < bfsVec.size() - 1; i++){
        Vertex<T> * v = findVertex(bfsVec[i]);
        for (auto e : v->adj) {
            if (e.dest->info == bfsVec[i+1]) {
                bfsCap = std::min(bfsCap, e.capacity);
            }
        }
    }

    maxCapCap = firstAlgorithm(origin, target);
    maxCapVec = getPath(origin, target);
    maxCapEdges = maxCapVec.size();

    vector<int> empty;
    pair<vector<int>, int> bfsPath, maxCapPath;

    bfsPath.first = bfsVec;
    bfsPath.second = bfsCap;

    maxCapPath.first = maxCapVec;
    maxCapPath.second = maxCapCap;

    recursivePathFinderLimited(origin, target, empty, maxCapEdges, bfsCap, INF);

    paths.insert(bfsPath);
    paths.insert(maxCapPath);

    paths = filterPathsByDominance();
}

template<class T>
int Graph<T>::recursivePathFinderLimited(T current, T target, vector<T> currentPath, int maxCapEdges, int bfsCap, int currentCap) {
    currentPath.push_back(current);
    if (currentPath.size() == maxCapEdges) {
        return 0;
    }
    if (current == target) {
        pair<vector<int>, int> res;
        res.first = currentPath;
        res.second = currentCap;
        paths.insert(res);
    } else {
        for (auto e : findVertex(current)->adj) {
            if (e.capacity > bfsCap) {
                recursivePathFinderLimited(e.dest->info, target, currentPath, maxCapEdges, bfsCap, std::min(e.capacity, currentCap));
            }
        }
    }
    return 0;
}

template<class T>
map<vector<int>, int> Graph<T>::filterPathsByDominance() {
    map<vector<int>, int> filtered;
    for (auto path : paths) {
        bool dominated = false;
        for (auto path2 : paths) {
            if ((path2.first.size() < path.first.size() && path2.second >= path.second) ||
                    (path2.second > path.second && path2.first.size() <= path.first.size())) {
                dominated = true;
            }
        }
        if (!dominated) {
            filtered.insert(path);
        }
    }
    return filtered;
}


#endif /* GRAPH_H_ */