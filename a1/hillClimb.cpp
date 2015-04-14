// hillClimb.cpp
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
#include <set>
#include <map>

#include "Model.h"
#include "Graph.h"
#include "Testing.h"
#include "hillClimb.h"
 
bool hillClimb::run(Model& m,std::vector<Node*>& solution){
    begin();

    std::set<Node*> explored;
    isSolved = _run(m,solution,explored,m.start_tile);

    end();    
    return isSolved;    
}

// recursive method to traverse the state.
bool hillClimb::_run(Model& m, std::vector<Node*>& solution,
        std::set<Node*>& explored, Node* currentNode)
{
    numNodesVisited++;
    if(currentNode == m.goal_tile){
        solution.insert(solution.begin(),currentNode);        
        return true;
    }else{
        explored.insert(currentNode);

        // find the best neighbouring node.
        Node* best = currentNode;
        Node* cand;
        for(int i = 0; i < 6; ++i){
            cand = (*currentNode)[i];
            if( currentNode->canMove(i) == false){continue;}
            if( explored.find(cand) != explored.end()){continue;}
            numGenNodes++;

            if(cand->gradient > best->gradient){
                best = cand;
            }
        }

        if(best == currentNode){            
            return false;
        }else{
            // recursively call 
            bool rs = _run(m,solution,explored,best);
            if(rs){
                // we have found a solution.
                solution.insert(solution.begin(),currentNode);
                return true;
            }
        }
    }
    return false;    
}

bool hillClimb::test(){
    Testing t;
    GraphGenerator gen;
    Model m;
    std::vector<Node*> solution;
    bool rs;

    m.world = gen.make(20,0);
    m.start_tile = m.world->nodes[0];
    m.goal_tile = m.world->nodes[19];
    gen.applyGradient(m.world,m.goal_tile,20*12,-12);
    solution.clear();
    rs = run(m,solution);    
    t.compareSolutions(true,rs,m,solution);
    
    m.world->removeEdge(19,7);
    m.world->removeEdge(19,18);
    solution.clear();
    rs = run(m,solution);
    m.world->nodes[18]->gradient -= 1;
    m.world->nodes[7]->gradient -= 1;
    t.compareSolutions(false,rs,m,solution);

    srand(12);
    delete m.world;
    m.world = gen.make_percent(40,0.5);
    m.start_tile = m.world->nodes[0];
    m.goal_tile = m.world->nodes[29];
    gen.applyGradient(m.world, m.goal_tile, 40*12, -12);
    solution.clear();
    rs = run(m,solution);    
    t.compareSolutions(false,rs,m,solution);

    srand(0);
    delete m.world;
    m.world = gen.make_percent(4000,0.6);
    m.start_tile = m.world->nodes[0];
    m.goal_tile = m.world->nodes[3999];
    gen.applyGradient(m.world, m.goal_tile, 4000*12, -12);
    solution.clear();
    rs = run(m,solution);
    t.compareSolutions(false,rs,m,solution);

    return true;

}

