// astar.h
#ifndef _ASTAR_H_
#define _ASTAR_H_

#include <vector>
#include <set>
#include "Strategy.h"
class Model;
class Node;

class astar : public Strategy {
public:
    // the type variable determines which heuritics to use
    // type == 1 means use h1() as the heuristic
    // type == 2 means use h2() as the heuristic
    int type;
    astar(int type = 1){
        this->type = type;
    }

    virtual ~astar(){}

    bool run(Model& m,std::vector<Node*>& solution);            

private:
    bool _run(Model& m, std::vector<Node*>& solution);    

    // define heuristics to use for the ASTAR
    float h(Model& m, Node* n);
    float h1(Model& m, Node* n);
    float h2(Model& m, Node* n);
};

#endif
