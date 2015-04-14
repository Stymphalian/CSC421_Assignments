//greedy.cpp
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
#include <set>
#include <map>

#include "Model.h"
#include "Graph.h"
#include "Testing.h"
#include "greedy.h"
 
bool greedy::run(Model& m,std::vector<Node*>& solution){        
    begin();

    isSolved = _run(m,solution);
    
    end();    
    return isSolved;    
}

float greedy::h(Model& m,Node* currentNode){
    if( type == 1){
        return h1(m,currentNode);
    }else{
        return h2(m,currentNode);
    }
}
// the manhattan distance between the currentNode and the goal node.
float greedy::h1(Model& m,Node* currentNode){
    return m.world->distance(currentNode, m.goal_tile);
}
// the gradient distacnce between the currentNode and the goal node.
float greedy::h2(Model& m,Node* currentNode){    
    return m.world->gradientDiff(currentNode,m.goal_tile);
}

bool greedy::_run(Model& m, std::vector<Node*>& solution){    
    std::set<Node*> explored;
    std::priority_queue<Node*, std::vector<Node*>,compareNode> pq(compareNode(*this,m));
    std::map<Node*,Node*> cameFrom;
    Node* currentNode;

    pq.push(m.start_tile);        
    cameFrom[currentNode] = NULL;

    while(pq.empty() == false){
        currentNode = pq.top();
        pq.pop();

        numNodesVisited++;

        // already explored this node. done.
        if( explored.find(currentNode) != explored.end()){
            continue;
        }

        // this is the goal tile, we are done.
        if( currentNode == m.goal_tile){
            // create the solution chain back to the start tile            
            while(currentNode != NULL){
                solution.insert(solution.begin(), currentNode);
                currentNode = cameFrom[currentNode];
            }

            return true;
        }

        // mark as explored.
        explored.insert(currentNode);
    

        // add in all the neighbours into the priority queue
        Node* cand;
        for(int i =0 ;i <6; ++i) {
            cand = (*currentNode)[i];
            if( currentNode->canMove(i) == false){continue;}
            if( explored.find(cand) != explored.end()){continue;}

            numGenNodes++;
            pq.push(cand);

            // record where we came from
            cameFrom[cand] = currentNode;
        }

    }

    // didn't find a solution
    return false;
}

bool greedy::test(){
    Testing t;
    GraphGenerator gen;
    Model m;
    std::vector<Node*> solution;
    bool rs= false;

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


    // this test case is special
    // for the greedly algorithm, this will fail
    srand(12);
    delete m.world;
    m.world = gen.make_percent(40,0.5);
    m.start_tile = m.world->nodes[0];
    m.goal_tile = m.world->nodes[29];
    gen.applyGradient(m.world, m.goal_tile, 40*12, -12);
    solution.clear();
    rs = run(m,solution);    
    t.compareSolutions(true,rs,m,solution);        


    srand(0);
    delete m.world;
    m.world = gen.make_percent(4000,0.6);
    m.start_tile = m.world->nodes[0];
    m.goal_tile = m.world->nodes[3999];
    gen.applyGradient(m.world, m.goal_tile, 4000*12, -12);
    solution.clear();
    rs = run(m,solution);
    t.compareSolutions(true,rs,m,solution);
    
    return false;
}
