#include <iostream>
#include <fstream>
#include "Graph.h"
#include <string>

using namespace std;
bool loadFile(string fileName, Graph<int> &graph); //loads stops and vehicles (nodes and edges) from file to the graph

int main() {
    Graph<int> graph;
    string fileName;
    cout << "Insert Dataset File name:\n";
    cin >> fileName;


    while(!loadFile(fileName, graph)){
        cout << "Error Loading File. Try again:\n";
        cin >> fileName;
        graph.~Graph();
        Graph<int> graph;
    }
    cout << "Files loaded\n";

//   int increase;
//    graph.auxTest2_4();
//    graph.printGraph();
//    cout << "=================================================" << endl;
//    increase = graph.increaseGroupSize(1, 6, 10);
//    graph.printGraph();
//    printf("increased by %d\n", increase);
//    graph.vertexTime(1, 8);
//    cout << graph.longestPath(1, 8);
    // cout << "done\n";
//    cout << "Biggest Possible group size: " << graph.firstAlgorithm(1, 6) << '\n';
//    vector<int> path = graph.getPath(1,5000);
//    if(path.size() == 0) printf("empty\n");
//    for(auto node: path){
//        printf("%d ->", node);
//    }
//    graph.printGraph();
    cout << "bfs\n";
    graph.unweightedShortestPath(1);
    for(int node: graph.getPath(1, 6)){
        cout << node << " ";
    }
    cout << '\n';
    cout << "maxCap\n";
    graph.firstAlgorithm(1, 6);
    for(int node: graph.getPath(1, 6)){
        cout << node << " ";
    }
    cout << '\n';
    std::vector<vector<int>> vector1 = graph.capacityOrEdges(1, 6);
    cout << "number of equal paths: " << vector1.size() << endl;
    for(int node: graph.getPath(1, 6)){
        cout << node << " ";
    }
    return 0;
}

bool loadFile(string fileName, Graph<int> &graph){
    ifstream stream;
    stream.open("Tests/" + fileName, ifstream::in);
    int nNodes, nEdges, origin, dest, cap, dur;
    //get nodes and edges
    stream >> nNodes >> nEdges;
    graph.setNumberNodes(nNodes);
    graph.setNumberEdges(nEdges);

    //create nodes and add them to the graph
    for(int i = 1; i <= nNodes; i++){
        if(!graph.addVertex(i)) return false;
    }
    //get edges and add them to the graph
    while(!stream.eof()){
        stream >> origin >> dest >> cap >> dur;
        if(!graph.addEdge(origin, dest, dur, cap, 1)) return false;
    }
    return true;
}
