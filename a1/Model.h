// model.h
#ifndef _MODEL_H_
#define _MODEL_H_

#include "GraphGenerator.h"
class Node;

class Config{
public:
    Config(){}

    Config(int l, float o){
        locations = l;
        free_obstacles = o;
    }

    int locations;
    float free_obstacles;
};

class Model {
private:
    GraphGenerator gen;
public:    
    Graph* world;
    Node* goal_tile;
    Node* start_tile;

    Model();
    Model(Config c);
    virtual ~Model();    
};

#endif

