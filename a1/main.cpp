// main.cpp
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>

#include "Graph.h"
#include "GraphGenerator.h"
#include "Model.h"

#include "bfs.h"
#include "dfs.h"
#include "ids.h"
#include "greedy.h"
#include "astar.h"
#include "hillClimb.h"

int main(int argc, char** argv){

    // retrieve command-line paramters.
    if( argc != 2){
        printf("Usage %s number_interations\n",argv[0]);
        return 0;
    }
    
    // HARD-CODED configurations    
    Config configs[] = {
        Config(10,0.2),
        Config(10,0.5),
        Config(20,0.2),
        Config(20,0.5)        
    };

    // HARD-CODED strategies to use
    Strategy* strats[] = {
        new bfs(),
        new dfs(),
        new ids(),
        new greedy(1),
        new greedy(2),
        new astar(1),
        new astar(2),
        new hillClimb()
    };    

    
    // local variables
    const int num_iterations = atoi(argv[1]);        
    const int num_strats = sizeof(strats)/sizeof(Strategy*);
    const int num_configs = sizeof(configs)/sizeof(Config);    
            
    // //run through all the tests
    // for(int i = 0;i < num_strats; ++i){
    //     printf("test %d\n",i);
    //     strats[i]->test();
    //     strats[i]->reset();
    // }        

    // seed the rng
    srand(time(NULL));    

    // print out a header for the info
    printf("  %10s %10s %10s %10s | %10s\n","Space", "Time(vis)","Time(s)",
        "solved","failed");
    
    Strategy* current_strat;
    std::vector<Node*> solution;
    // iterate through each configuration
    for(int i = 0; i < num_configs; ++i){ 

        // go through all the number of iterations
        for( int j = 0; j < num_iterations; ++j){

            // create a new problem instance.
            Model m(configs[i]);

            // run the all the algorithms on the model
            for(int k = 0; k < num_strats; ++k){
                current_strat = strats[k];                
                current_strat->run(m,solution);
                solution.clear();
            }
        }

        // print out the statistics for all the algorithms
        for( int i = 0;i < num_strats; ++i){
            printf("%d,",i);
            strats[i]->print();
            strats[i]->reset();
        }
        printf("\n");
    }    
    

    // clean-up
    for(int i = 0;i < num_strats; ++i){
        delete strats[i];
    }
    return 0;
}

// int main(int argc, char** argv){
//     int num_locs = atoi(argv[1]);
//     float per = atof(argv[2]);
//     GraphGenerator g;
//     srand(0);
//     // Graph* gr = g.make_percent(10,0.5);    
//     Graph* gr = g.make_percent(num_locs,per);
//     g.applyGradient(gr,gr->nodes[0], num_locs*12,-12);
//     gr->printGraph();
    
//     delete gr;
//     return 0;
// }
// ----------------------------------
