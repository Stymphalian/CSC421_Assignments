// Graph.h
#ifndef _GRAPH_H_
#define _GRAPH_H_


#include <cstdio>
#include <map>

class Node{
public:
    enum dir_e {NE = 0, E = 1, SE = 2, SW = 3, W = 4, NW = 5};

    // variables
    unsigned id; 
    float x,y;
    int gradient;

    // methods
    Node(unsigned id);
    virtual ~Node();

    Node*& operator[] (const int x);
    bool removeEdgeByNodeId(unsigned id);
    bool removeEdgeByDirection(dir_e dir);
    bool hasNeighbourAndCanMove(Node* n);
    bool hasNeighbour(Node* n);
    void printNode();
    void setPos(float x, float y);
    bool canMove(int dir);

private:
    Node* _nodes[6];
    bool _canMove[6];
};

class Graph{
public:
    // variables
    std::map<unsigned, Node*> nodes;
    int edges_count;
    int obstacles_count;    

    int default_edges_count;
    int default_obstacles_count;

    // methods    
    Graph();
    virtual ~Graph();

    void printGraph();
    void clear();
    bool removeEdge(Node* n1, Node* n2);
    bool removeEdge(unsigned n1, unsigned n2);
    // bool addEdge(Node* n1, Node* n2);
    // bool addNode(Node* n1, Node* start,int dir);
    // bool removeNode(Node* n1);
    float distance(Node* n1, Node* n2);
    float gradientDiff(Node* n1, Node* n2);
    int getRingLevel(int n);

    int numNodes();
    int numEdges();
    int numObstacles();    
    int numBoundaries();
};

#endif

