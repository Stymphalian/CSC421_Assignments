// hillClimb.h
#ifndef _HILLCLIMB_H_
#define _HILLCLIMB_H_

#include <vector>
#include <set>
#include "Strategy.h"
class Model;
class Node;

class hillClimb : public Strategy {
public:
    hillClimb(){}
    virtual ~hillClimb(){}

    bool run(Model& m,std::vector<Node*>& solution);
    bool test();

private:
    bool _run(Model& m, std::vector<Node*>& solution,    
        std::set<Node*>& explored, Node* currentNode);
};

#endif

