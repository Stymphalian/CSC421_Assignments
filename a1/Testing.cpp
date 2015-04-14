// Testing.cpp
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <vector>

#include "Testing.h"

#include "Graph.h"
#include "Model.h"

void Testing::compareSolutions(bool want,bool get, Model& m, 
        std::vector<Node*>& solution){    
    if( want != get){
        printf("want != get\n");
        printSolution(solution);        
        return;
    }

    if(get == true){
        if( checkValidSolution(m,solution) == false){
            printf("invalid solution\n");
            printSolution(solution);
            return;
        }
    }
}

// prints the id of the nodes
void Testing::printSolution(std::vector<Node*>& solution){    
    int n = solution.size();
    printf("[");
    for(int i = 0;i<n ;++i){
        printf("%d ", solution[i]->id);
    }
    printf("]\n");
}

bool Testing::checkValidSolution(Model& m, std::vector<Node*>& get){
    if( get[0] != m.start_tile || get[get.size()-1] != m.goal_tile){
        return false;
    }
    
    Node* current = get[0];
    int n = get.size();
    for(int i = 0;i < n-1; ++i ){
        // if( current->hasNeighbour(get[i+1]) == false){
        if( current->hasNeighbourAndCanMove(get[i+1]) == false){
            return false;
        }
        current = get[i+1];
    } 
    return true;
}

void Testing::check(bool truthy,const char* msg,...){
    if( truthy){return;}

    // false so print the message
    va_list args;
    va_start(args,msg);
    vprintf(msg,args);
    va_end(args);
}

