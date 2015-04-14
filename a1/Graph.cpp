//Graph.cpp
#include <cstdio>
#include <cmath>
#include <map>

#include "Graph.h"

// ------------------
// Node
// ------------------
Node::Node(unsigned id){
    this->id = id;
    for(int i = 0;i < 6; ++i){
        _nodes[i] = NULL;
        _canMove[i] = true;
    }
    x = y = 0;
    gradient = 0;
}
Node::~Node(){}

Node*& Node::operator[] (const int x){
    return _nodes[x];
}

void Node::printNode(){
    printf("%4d (%8.4f,%8.4f) %3d -> [",id,x,y, gradient);
    for(int i = 0; i < 6; ++i){
        if( _nodes[i] == NULL || _canMove[i] == false){
            if( i != 5){
                printf("   ,");
            }else{
                printf("    ");
            }                
        }else{ 
            if( i != 5){
                printf("%3d,",_nodes[i]->id);
            }else{
                printf("%3d ",_nodes[i]->id);
            }
        }
    }
    printf("]\n");
}

bool Node::removeEdgeByNodeId(unsigned id){
    for(int i = 0; i < 6; ++i){
        if(_nodes[i] != NULL){
            if( _nodes[i]->id == id){
                _canMove[i] = false;
                // _nodes[i] = NULL;
                return true;
            }
        }
    }

    return false;
}
bool Node::removeEdgeByDirection(Node::dir_e dir){
    if( _nodes[(int)dir] == NULL){
        return false;
    }else{
        _nodes[(int)dir] = NULL;
        return true;
    }    
}

bool Node::hasNeighbourAndCanMove(Node* n){
    if( n == NULL){return false;}    
    for(int i = 0;i < 6; ++i){
        if( _nodes[i] == n){
            return canMove(i);
        }
    }
    return false;
}

bool Node::hasNeighbour(Node* n){
    if( n == NULL){return false;}    
    for(int i = 0;i < 6; ++i){
        if( _nodes[i] == n){return true;}
    }
    return false;
}

void Node::setPos(float x, float y){
    this->x = x;
    this->y = y;
}

bool Node::canMove(int dir){
    if( _nodes[dir] == NULL){return false;}
    return _canMove[dir];
}


// ------------------
// Graph 
// ------------------
Graph::Graph(){
    edges_count = 0;    
}
Graph::~Graph(){    
    clear();
}

void Graph::printGraph(){
    // print out the map of nodes
    printf("edges = %d, obstacles = %d\n",numEdges(),numObstacles());
    std::map<unsigned,Node*>::iterator it;
    for(it = nodes.begin(); it != nodes.end(); ++it){
        it->second->printNode();
    }            
}

void Graph::clear(){
    std::map<unsigned,Node*>::iterator it;
    for(it = nodes.begin(); it != nodes.end(); ++it){
       delete it->second;
    }
    nodes.clear();

    edges_count = 0;
    obstacles_count = 0;
}

bool Graph::removeEdge(Node* n1, Node* n2){
    if( n1 == NULL || n2 == NULL){return false;}
    if(n1->hasNeighbour(n2) && n2->hasNeighbour(n1)){
        n1->removeEdgeByNodeId(n2->id);
        n2->removeEdgeByNodeId(n1->id);        

        edges_count -= 2;
        obstacles_count += 1;
        return true;
    }else{
        return false;
    }
}

// remove the edge between the two nodes
bool Graph::removeEdge(unsigned n1, unsigned n2){
    return removeEdge(nodes[n1],nodes[n2]);
}

float Graph::distance(Node* n1, Node* n2){
    if(n1 == NULL || n2 == NULL){return 0;}
    float x = (n2->x - n1->x);
    float y = (n2->y - n2->y);
    return sqrt(x*x + y*y);
}

float Graph::gradientDiff(Node* n1, Node* n2){
    if(n1 == NULL || n2 == NULL){return 0;}
    return fabs(n1->gradient - n2->gradient);
}

int Graph::getRingLevel(int n){
    if( n == 0){return 0;}
    if( n >= 1  && n <= 6){return 1;}

    int count = 2;
    int i = 7;
    while(i < n){
        for(int j = 0;j < 6; ++j){
            for( int k = 0; k < count; ++k ){
                if (i == n){
                    return count;
                }
                i++;                
            }            
        }
        count++;    
    }    
    return count;
}

int Graph::numNodes(){
    return nodes.size();
}
int Graph::numEdges(){
    return edges_count/2;
}
int Graph::numObstacles(){
    return obstacles_count;
}
int Graph::numBoundaries(){
    return edges_count/2 + obstacles_count;
}
