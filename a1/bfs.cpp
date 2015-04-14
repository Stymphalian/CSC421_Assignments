// bfs.cpp
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
#include <set>
#include <map>

#include "Model.h"
#include "Graph.h"
#include "Testing.h"
#include "bfs.h"
 
bool bfs::run(Model& m,std::vector<Node*>& solution){        
    begin();
    isSolved = _run(m,solution);
    end();    
    return isSolved;    
}


bool bfs::_run(Model& m,std::vector<Node* >& solution){        
    // we are already done.
    if(m.start_tile == m.goal_tile){
        solution.push_back(m.start_tile);
        return true;
    }

    Node* currentNode;    
    std::map<Node*,Node*> cameFrom;
    std::set<Node*> explored;    
    std::queue<Node*> frontier;

    cameFrom[m.start_tile] = NULL;
    frontier.push(m.start_tile);    
    explored.insert(m.start_tile);

    while(frontier.empty() == false){    

        // get the next node from the queue
        currentNode = frontier.front();
        frontier.pop();
        numNodesVisited += 1;

        // add the children into the search space
        Node* cand;
        for(int i = 0; i < 6; ++i){
            cand = (*currentNode)[i];
            if( currentNode->canMove(i)== false){continue;}            
            numGenNodes += 1;

            // not in the explored set,so add it to the frontier
            if( explored.find(cand) == explored.end()){
                cameFrom[cand] = currentNode;
                frontier.push(cand);       
                explored.insert(cand);

                // test against the goal
                if( cand == m.goal_tile){
                    // create the solution chain
                    solution.insert(solution.begin(),m.goal_tile);
                    while( currentNode != NULL){                        
                        solution.insert(solution.begin(),currentNode);
                        currentNode = cameFrom[currentNode];
                    }                    
                    return true;
                }
                
            }
        }
    }

    return false;
}

