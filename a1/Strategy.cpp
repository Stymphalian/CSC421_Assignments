// Strategy.cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <ctime>

#ifdef __unix__
#include <sys/time.h>
#endif


#include "Strategy.h"
#include "Testing.h"
#include "Model.h"
#include "Graph.h"
#include "GraphGenerator.h"

Strategy::Strategy(){
    reset();
}

Strategy::~Strategy(){}

// This is an overridable method.
// sub-class will overide this method in order to execute
// a different algorithm
bool Strategy::run(Model& m,std::vector<Node*>& solution){
    return false;
}

void Strategy::print(){
    printf("%10f,%10f,%10f,%10d,%10d\n",
        spaceTotal/numRuns, timeTotal/numRuns,
        runningTimeTotal, totalSolved,numRuns - totalSolved);
}

void Strategy::reset(){
    runningTime = 0;
    numGenNodes = 0;
    numNodesVisited = 0;

    spaceTotal = 0;
    timeTotal = 0;
    runningTimeTotal = 0;
    totalSolved = 0;    
    numRuns = 0;
}

// keep track of time
void Strategy::begin(){
    start = clock();  

    #ifdef __unix__
    gettimeofday(&start_tv,NULL);
    #endif

    runningTime = 0;
    numGenNodes = 0;
    numNodesVisited = 0;    
}
void Strategy::record(){
    spaceTotal += numGenNodes;
    timeTotal += numNodesVisited;
    runningTimeTotal += runningTime;
    if(isSolved){
        totalSolved++;
    }
    numRuns++;
}
void Strategy::end(){    
    runningTime = ((double)clock() - start)/ CLOCKS_PER_SEC;

    #ifdef __unix__
    struct timeval end_tv;
    gettimeofday(&end_tv,NULL);    
    double t1 = end_tv.tv_sec + end_tv.tv_usec/1000000.0f;
    double t2 = start_tv.tv_sec + start_tv.tv_usec/1000000.0f;
    runningTime = t1 - t2;
    #endif

    record();
}

// Tests to run to see if the algorithm behaves as expected.
bool Strategy::test(){
    Testing t;
    GraphGenerator gen;
    Model m;
    std::vector<Node*> solution;

    // test with no inner obstacles
    m.world = gen.make(20,0);
    m.start_tile = m.world->nodes[0];
    m.goal_tile = m.world->nodes[19];
    gen.applyGradient(m.world,m.goal_tile,20*12,-12);
    solution.clear();
    bool rs = run(m,solution);    
    t.compareSolutions(true,rs,m,solution);
    
    // test with an inaccesible goal tile
    m.world->removeEdge(19,7);
    m.world->removeEdge(19,18);
    solution.clear();
    rs = run(m,solution);
    m.world->nodes[18]->gradient -= 1;
    m.world->nodes[7]->gradient -= 1;
    t.compareSolutions(false,rs,m,solution);

    // test a random generated map
    // NOTE: The srand() is implemented differently depending on the platform 
    // one is using.
    srand(12);
    delete m.world;
    m.world = gen.make_percent(40,0.5);
    m.start_tile = m.world->nodes[0];
    m.goal_tile = m.world->nodes[29];
    gen.applyGradient(m.world, m.goal_tile, 40*12, -12);
    solution.clear();
    rs = run(m,solution);    
    t.compareSolutions(true,rs,m,solution);


    // test a random generated large graph
    srand(0);
    delete m.world;
    m.world = gen.make_percent(4000,0.6);
    m.start_tile = m.world->nodes[0];
    m.goal_tile = m.world->nodes[3999];
    gen.applyGradient(m.world, m.goal_tile, 4000*12, -12);
    solution.clear();
    rs = run(m,solution);
    t.compareSolutions(true,rs,m,solution);

    return true;
}

