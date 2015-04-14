//ids.h
#ifndef _IDS_H_
#define _IDS_H_

#include <vector>
#include <set>
#include "Strategy.h"
class Model;
class Node;

class ids : public Strategy {
public:
    ids(){}
    virtual ~ids(){}

    bool run(Model& m,std::vector<Node*>& solution);

private:
    int _dfs(Model& m, std::vector<Node*>& solution,
            int limit, Node* currentNode,std::set<Node*>& explored);
    bool _run(Model& m, std::vector<Node*>& solution);
};

#endif
