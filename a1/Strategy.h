// Strategy.h
#ifndef _STRATEGY_H
#define _STRATEGY_H

#include <ctime>
#include <vector>

// forward declaration
class Model;
class Node;

class Strategy{
protected:
    clock_t start;    

    #ifdef __unix__
    struct timeval start_tv;
    #endif

    void begin();
    void record();
    void end();
public: 
    // variables
    double runningTime; // seconds
    int numGenNodes; // number of nodes generated
    int numNodesVisited; // number of nodes visited
    int isSolved; // see if the thing was solved

    float spaceTotal; // avg numbers of nodes created
    float timeTotal;  // avg number of nodes visited
    double runningTimeTotal; // seconds  
    int totalSolved; // how many problems were solved correctly
    int numRuns; // number of times we have run this strategy.
    

    Strategy();
    virtual ~Strategy();
    virtual bool run(Model& m,std::vector<Node*>& solution);    
    virtual bool test();
    void print();  
    void reset();  
};

#endif

