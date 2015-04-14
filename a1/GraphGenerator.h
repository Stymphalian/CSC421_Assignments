// GrpahGenerator.h
#ifndef _GRAPH_GENERATOR_
#define _GRAPH_GENERATOR_

class Node;
class Graph;

class GraphGenerator{
public:
    double pi;
    double radius;
    GraphGenerator(){
        pi = 3.14159265359;
        radius = 1.0f;
    }
    
    virtual ~GraphGenerator(){}

    Graph* make(int num_locations,int numObstacles);
    Graph* make_percent(int num_locations,float percent);
    
    Graph* generate(int num_locations);
    Graph* addObstacles(Graph* g, int numObstacles);
    Graph* applyGradient(Graph* g, Node* start, int startGradient, int scale);    
};

#endif

