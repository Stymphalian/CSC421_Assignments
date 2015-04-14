// astar.cpp
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
#include <set>
#include <map>

#include "Model.h"
#include "Graph.h"
#include "Testing.h"
#include "astar.h"
 
bool astar::run(Model& m,std::vector<Node*>& solution){
    begin();
    isSolved = _run(m,solution);
    end();    
    return isSolved;    
}

float astar::h(Model& m, Node* currentNode){
    if( type == 1){
        return h1(m,currentNode);
    }else{
        return h2(m,currentNode);
    }
}
// the manhattan distance between the currentNode and the goal node.
float astar::h1(Model& m,Node* currentNode){
    return m.world->distance(currentNode, m.goal_tile);
}
// the gradient distacnce between the currentNode and the goal node.
float astar::h2(Model& m,Node* currentNode){
    return m.world->gradientDiff(currentNode,m.goal_tile);
}

// http://www.policyalmanac.org/games/aStarTutorial.htm
bool astar::_run(Model& m,std::vector<Node* >& solution){
    Node* currentNode;

    std::map<Node*,float> gCosts;
    std::map<Node*,Node*> cameFrom;
    std::set<Node*> explored;
    std::set<Node*> frontierSet;
    std::vector<Node*> frontier;
    
    gCosts[m.start_tile] = 0.0f;    
    frontier.push_back(m.start_tile);
    frontierSet.insert(m.start_tile);
    cameFrom[m.start_tile] = NULL;

    std::vector<Node*>::iterator it;
    while(frontier.empty() == false){

        // find the next currentNode
        std::vector<Node*>::iterator best = frontier.begin();
        float best_cost = gCosts[*best] + h(m,*best);
        for(it = frontier.begin(); it != frontier.end(); ++it){
            // calculat the f cost for the node
            // f() = g() + h()
            // g is the current cost of reaching this node
            // h is the heuristic cost of this node
            float g_cost = gCosts[*it];
            float h_cost = h(m,*it);
            float f_cost = g_cost + h_cost;

            // sort by f()
            if( f_cost < best_cost){
                best_cost = f_cost;
                best = it;
            }            
        }
        currentNode = *best;       

        // remove from the frontier
        frontier.erase(best);
        frontierSet.erase(currentNode);
        numNodesVisited++;

        // add to explored
        explored.insert(currentNode);

        // hey we found the node we wanted.
        if( currentNode == m.goal_tile){
            // create the solution chain
            while(currentNode != NULL){
                solution.insert(solution.begin(), currentNode);
                currentNode = cameFrom[currentNode];
            }            
            return true;
        }
    
        // add children to the frontier
        Node* cand;
        float currentNode_g_cost = gCosts[currentNode];
        for(int i = 0;i < 6; ++i){
            cand = (*currentNode)[i];            
            if( currentNode->canMove(i) == false){continue;}            

            // already explored, therefore skip
            if( explored.find(cand) != explored.end()){continue;}

            // node is already in the frontier
            if( frontierSet.find(cand) != frontierSet.end()){
                // update the g_cost if our g_cost is better
                if( currentNode_g_cost + 1 < gCosts[cand]){
                    gCosts[cand] = currentNode_g_cost;
                    cameFrom[cand] = currentNode;
                }
                continue;
            }

            // add to the frontier and record the parent of this node
            frontierSet.insert(cand);
            frontier.push_back(cand);
            cameFrom[cand] = currentNode;
            
            // the +1 is the move cost
            gCosts[cand] = currentNode_g_cost + 1;
            numGenNodes++;
        }
    }    

    return false;
}

