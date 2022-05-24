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

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<int>::max()

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
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, int duration, int c, int w): dest(d), weight(w), capacity(c), duration(duration), flux(0){}


/*************************** Graph  **************************/

template <class T>
class Graph {
private:
    int numberNodes, numberEdges;
    std::vector<Vertex<T> *> vertexSet;    // vertex set

    //Fp05
    Vertex<T> * initSingleSource(const T &orig);
    bool relax(Vertex<T> *v, Vertex<T> *w, int weight);
    int ** W = nullptr;   // dist
    int **P = nullptr;   // path
    int findVertexIdx(const T &in) const;

public:
    ~Graph();
    Vertex<T> *findVertex(const T &in) const;
    bool addVertex(const T &in);
    bool addEdge(const T &sourc, const T &dest, int d, int c, int w);
    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;

    int getNumberNodes() const;
    int getNumberEdges() const;
    void setNumberNodes(int numberNodes);
    void setNumberEdges(int numberEdges);

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
void Graph<T>::dijkstraShortestPath(const T &origin) {
    auto s = initSingleSource(origin);
    MutablePriorityQueue<Vertex<T>> q;
    q.insert(s);
    while( ! q.empty() ) {
        auto v = q.extractMin();
        for(auto e : v->adj) {
            auto oldDist = e.dest->dist;
            if (relax(v, e.dest, e.weight)) {
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

//sets all fluxes to 0
template<class T>
void Graph<T>::zeroFlux() {
    for(auto v: vertexSet){
        for(auto edge: v->adj){
            edge.setFlux(0);
        }
    }
}


#endif /* GRAPH_H_ */