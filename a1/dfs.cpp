//dfs.cpp
#include <cstdio>
#include <cstdlib>
#include <stack>
#include <vector>
#include <set>
#include <map>

#include "Model.h"
#include "Graph.h"
#include "dfs.h"
#include "Testing.h"
 
bool dfs::run(Model& m,std::vector<Node*>& solution){        
    begin();
    isSolved = _run(m,solution);
    end();    
    return isSolved;    
}


bool dfs::_run(Model& m,std::vector<Node* >& solution){
    // we are already done.
    if(m.start_tile == m.goal_tile){
        solution.push_back(m.start_tile);
        return true;
    }

    // local variables
    Node* currentNode;        
    std::set<Node*> explored;
    std::stack<action_t> frontier;
    std::stack<Node*> solutionStack;
    action_t action;

    // push into the frontier and mark as explored.
    action.init(m.start_tile,dfs::action_t::FINISH);
    frontier.push(action);

    action.init(m.start_tile,dfs::action_t::EXPLORE);
    frontier.push(action);

    explored.insert(m.start_tile);

    while(frontier.empty() == false){    
        // get the next node from the queue
        action = frontier.top();
        frontier.pop();        
        currentNode = action.next;
        

        if( action.type == dfs::action_t::EXPLORE){
            // this is the first time we have seen this node
            // therefore record it in our solution stack
            solutionStack.push(currentNode);        
        }else if(action.type == dfs::action_t::FINISH){
            // we are back-tracking from the node
            // therefore pop it off the solution stack
            solutionStack.pop();
        }        

        numNodesVisited += 1;

        // add the children into the search space
        Node* cand;
        for(int i = 0; i < 6; ++i){
            cand = (*currentNode)[i];
            if(currentNode->canMove(i) == false){continue;}
            // if( cand == NULL){continue;}
            numGenNodes += 1;

            // not in the explored set,so add it to the frontier
            if( explored.find(cand) == explored.end()){

                action.init(cand,dfs::action_t::FINISH);
                frontier.push(action);   
                action.init(cand,dfs::action_t::EXPLORE);
                frontier.push(action);
                
                explored.insert(cand);

                // test against the goal
                if( cand == m.goal_tile){
                    // create the solution chain
                    solution.insert(solution.begin(),m.goal_tile);
                    while( solutionStack.empty() == false ){
                        Node* top = solutionStack.top();
                        solutionStack.pop();
                        solution.insert(solution.begin(),top);
                    }

                    return true;
                }
                
            }
        }
    
    }

    return false;
}

