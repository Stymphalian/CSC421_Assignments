//GraphGenerator.cpp
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <set>
#include <queue>
#include <map>

#include "Graph.h"
#include "GraphGenerator.h"


// create a graph 
Graph* GraphGenerator::make(int num_locations, int num_obstacles){
    Graph *g = generate(num_locations);
    addObstacles(g,num_obstacles);
    return g;
}

Graph* GraphGenerator::make_percent(int num_locations, float percent_obstacles){
    if(percent_obstacles > 1 || percent_obstacles < 0 ){return NULL;}

    Graph *g = generate(num_locations);
    int num_obstacles = (int)(g->numEdges()*percent_obstacles);
    addObstacles(g,num_obstacles);
    return g;
}

// helper function used to measure the depth of reaching the currentNode
int getDepth(std::map<Node*,Node* >& cameFrom, Node *current){
    int depth = -1; 
    while(current != NULL){
        current = cameFrom[current];
        depth += 1;
    }
    return depth;
}

// perform a bfs search starting from the start node.
// the start node is given the startGradient, and each 
// each with depth d away is given the value startGradien - scale*d
Graph* GraphGenerator::applyGradient(Graph* g, Node* start, 
        int startGradient, int scale)
{
    // local variables
    std::set<Node*> explored;
    std::queue<Node*> frontier;
    std::map<Node*,Node*> cameFrom;

    // init the frontier and the explored.
    frontier.push(start);            
    explored.insert(start);
    cameFrom[start] = NULL;

    Node* currentNode;    
    while(frontier.empty() == false){
        currentNode = frontier.front();
        frontier.pop();        
        
        // determine the gradient value of the currentNode
        // depending on the depth value away from the startNode
        int ringLevel = getDepth(cameFrom,currentNode);
        explored.insert(currentNode);
        currentNode->gradient += startGradient + ringLevel*scale;

        // add neighobours to be explroed
        Node* cand;
        for(int i = 0;i< 6; ++i){
            cand = (*currentNode)[i];
            if(cand == NULL){continue;}
            if( explored.find(cand) != explored.end()){continue;}

            frontier.push(cand);            
            explored.insert(cand);
            cameFrom[cand] = currentNode;
        }        
    }

    return g;
}

Graph* GraphGenerator::generate(int num_locations){    
    Graph* g = new Graph();

    Node* next_node = NULL;

    // keep track of the number of times we must travel in a given direction
    int nextDir[] = {1,0,1,1,1,1};
    
    // a measures the height difference of two tiles vertically
    float a = sin(30*pi/180)*radius;
    // b measures the width difference of two tiles horizontally
    float b = cos(30*pi/180)*radius;
    float nextPos[][2] = {
        {b,3*a},
        {2*b,0},
        {b,-3*a},
        {-b,-3*a},
        {-2*b,0},
        {-b,3*a},
    };

    // keep track of the x,y coordinate of the current node.
    float x = 0.0f;
    float y = 0.0f;
    int nextDirIndex = 0;

    Node* n;
    int dir;
    int dirCount= 0;

    // generate the number base number of tiles
    for(int i = 0;i < num_locations; ++i){
        // create a new node.
        n = new Node(i);
        g->nodes[n->id] = n;
        n->setPos(x,y);

        // this is the first node we are adding, therefore
        // just add it to our map, and record it as the next node to use.
        if(next_node == NULL){            
            next_node = n;
            x += nextPos[nextDirIndex][0];
            y += nextPos[nextDirIndex][1];
            continue;
        }

        //get the dir to use for when fixing up the pointers.
        dir = nextDirIndex;                

        //local references to the neighbours of the the current node
        Node* a = next_node;
        Node* b = (*next_node)[(dir + 1)%6];
        Node* c = NULL;
        if( b != NULL){
            c = (*b)[dir];
        }
        
        // attach the new_node pointers
        (*n)[(dir + 1)%6] = c;
        (*n)[(dir + 2)%6] = b;
        (*n)[(dir + 3)%6] = a;

        // fix up the neighbour pointers.
        g->edges_count += 2;
        (*a)[dir] = n;
        if( b!= NULL){
            (*b)[(dir +5)%6] = n;
            g->edges_count +=2;
        }
        if( c != NULL){
            (*c)[(dir +4)%6] = n;
            g->edges_count +=2;
        }        
                
        // make the new node the next node of reference.
        next_node = n;

        // fix up the direction counter and values
        dirCount++;
        while(dirCount >= nextDir[nextDirIndex]){
            nextDir[nextDirIndex] += 1;
            nextDirIndex = (nextDirIndex +1)%6;
            dirCount = 0;
        }
        
        x += nextPos[nextDirIndex][0];
        y += nextPos[nextDirIndex][1];
    }

    // calculate the number of obstacles and edges
    g->obstacles_count = num_locations*6 - g->edges_count;

    g->default_obstacles_count = num_locations*6 - g->edges_count;
    g->default_edges_count = g->edges_count/2;
    return g;
}


// we want the graph to have 'num_obstacles' obstacles
Graph* GraphGenerator::addObstacles(Graph* g,int num_obstacles){     
    if( g->numEdges() - num_obstacles < 0 ){
        num_obstacles = g->numEdges();
    }

    // randomly choose tiles and edges to remove.
    int num_nodes = g->numNodes();
    for( int i = 0;i < num_obstacles; ++i ){            

        // keeping trying to remove edges
        for(;;){
            int node_id = rand() % num_nodes;
            int edge_dir = rand() % 6;

            if( g->nodes[node_id]->canMove(edge_dir) == false){
                continue;
            }

            // remove the edges between the nodes
            Node* a = g->nodes[node_id];
            Node* b = (*g->nodes[node_id])[edge_dir];
            g->removeEdge(a,b);

            // for each neighbour apply a gradient descent.
            a->gradient -= 1;
            b->gradient -= 1;

            break;
        }
    }

    return g;
}
