// greedy.h
#ifndef _GREEDY_H
#define _GREEDY_H

#include <vector>
#include <set>
#include "Strategy.h"
class Model;
class Node;

class greedy : public Strategy {
public:
    // the type variable determines which heuritics to use
    // type == 1 means use h1() as the heuristic
    // type == 2 means use h2() as the heuristic
    int type;
    greedy(int type = 1){
        this->type = type;
    }
    virtual ~greedy(){}

    bool run(Model& m,std::vector<Node*>& solution);    
    bool test();

private:
    class compareNode{
    public:
        greedy& g;
        Model& m;
        compareNode(greedy& _g,Model& _m):g(_g),m(_m){}            

        bool operator() (const Node* n1, const Node* n2) const{
            float f1 = g.h(m,(Node*)n1);
            float f2 = g.h(m,(Node*)n2);        
            return (f1 <= f2);
        }
    };

    bool _run(Model& m, std::vector<Node*>& solution);        

    // declare the heuristics to be used.
    float h(Model& m, Node* n);
    float h1(Model& m, Node* n);
    float h2(Model& m, Node* n);

    friend class compareNode;
};

#endif
