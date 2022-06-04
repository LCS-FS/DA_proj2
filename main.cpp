#include <iostream>
#include <fstream>
#include "Graph.h"
#include <string>
#include <chrono>

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

    /*graph.printPath(graph.FindPathGivenGroupSize(1, 6, 25));*/

//    cout << graph.edmondKarpFlux(1, 5000) << endl;
    cout << graph.edmondKarpFlux(1, 50) << endl;

    auto start = std::chrono::high_resolution_clock::now();

//    graph.vertexTime(1, 300);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::microseconds>(stop - start);
    long int time;
    time = duration.count();
    cout << endl;
    cout << time << endl;

//    cout << graph.firstAlgorithm(1, 5000);

    return 0;
}

bool loadFile(string fileName, Graph<int> &graph){
    ifstream stream;
    stream.open("../Tests/" + fileName, ifstream::in);
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
