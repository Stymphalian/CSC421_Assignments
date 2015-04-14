// Model.cpp
#include <cstdlib>
#include "Graph.h"
#include "Model.h"

Model::Model(){
    world = NULL;
    start_tile = NULL;
    goal_tile = NULL;
}

Model::Model(Config c){
    world = gen.make_percent(c.locations,c.free_obstacles);
    int start_id = rand() % c.locations;        
    int end_id = rand() % c.locations;
    start_tile = world->nodes[start_id];
    goal_tile = world->nodes[end_id];

    // magic numbers here is 12
    // each "ring" of the hex-grid will be atleast 12 units lower
    // than the previous ring. Therefore when the obstacles are added
    // the maximum descrease on any one tile is 6 
    // ( i.e the number of neighbours ) This ensures that we the goal_tile will
    // always be the global maximum.
    gen.applyGradient(world,goal_tile,c.locations*12,-12);
}

Model::~Model(){
    delete world;
    start_tile = NULL;
    goal_tile = NULL;
}    

