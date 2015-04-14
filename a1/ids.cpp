//ids.cpp
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
#include <set>
#include <map>

#include "Model.h"
#include "Graph.h"
#include "Testing.h"
#include "ids.h"
 
bool ids::run(Model& m,std::vector<Node*>& solution){        
    begin();
    isSolved = _run(m,solution);
    end();    
    return isSolved;    
}

// 0 is good
// 1 is failure
// 2 is failure by cutoff
int ids::_dfs(Model& m, std::vector<Node*>& solution,
    int limit,Node* currentNode, std::set<Node*>& explored)
{
    numNodesVisited += 1;

    if(currentNode == m.goal_tile){
        // the goal tile has been found
        // add to the solutoin chain and return success
        solution.insert(solution.begin(),currentNode);
        return 0;
    }else if( limit == 0){
        // we have reach a cut-off depth
        return 2;
    }else{
        bool cutoff_occured = false;
        int rs;
        Node* cand;

        explored.insert(currentNode);
        
        // recursively explore each of the neighbours
        for(int i = 0;i < 6; ++i){
            cand = (*currentNode)[i];
            if(currentNode->canMove(i) == false){continue;}            
            if( explored.find(cand) != explored.end()){continue;}
            numGenNodes++;

            rs = _dfs(m,solution,limit-1,cand,explored);
            if( rs == 2 ){
                // cutoff failure occured
                cutoff_occured = true;
            }else if( rs == 0){                
                // we have found a solution, add to the solution chain
                // and return success
                solution.insert(solution.begin(),currentNode);
                return 0;
            }            
        }


        if( cutoff_occured){
            // a cutoff occured, and we stil haven't found a solution.
            return 2;
        }else{
            // a normal failure
            return 1;
        }
    }
    return 1;
}

bool ids::_run(Model& m,std::vector<Node* >& solution){    
    int rs;
    int limit = 0;
    std::set<Node*> explored;

    for(;;){
        explored.clear();
        rs = _dfs(m,solution,limit,m.start_tile,explored);
        if( rs == 2){limit += 1;}        
        if( rs == 1){return false;}
        if( rs == 0){return true;}        
    }
    return false;    
}

