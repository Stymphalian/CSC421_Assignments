//bfs.h
#ifndef _BFS_H_
#define _BFS_H_

#include <vector>
#include "Strategy.h"
class Model;
class Node;

class bfs : public Strategy {
public:
    bfs(){}
    virtual ~bfs(){}

    bool run(Model& m,std::vector<Node*>& solution);    

private:
    bool _run(Model& m, std::vector<Node*>& solution);
};

#endif
