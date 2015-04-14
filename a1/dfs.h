//dfs.h
#ifndef _DFS_H_
#define _DFS_H_

#include <vector>
#include "Strategy.h"
class Model;
class Node;

class dfs : public Strategy {
public:
    dfs(){}
    virtual ~dfs(){}

    bool run(Model& m,std::vector<Node*>& solution);    
private:

    //An inner struct used to keep track of the solution stack
    struct action_t{
        public:
        enum action_e { EXPLORE, FINISH};
        void init(Node* n, action_e type){
            next = n;
            this->type = type;
        }

        Node* next;
        
        // EXPLORE - explore this node and its children
        // FINSIH - we are back-tracking from the search, clean up any
        // changes to the solution stack
        action_e  type;
    };

    bool _run(Model& m, std::vector<Node*>& solution);
};

#endif

