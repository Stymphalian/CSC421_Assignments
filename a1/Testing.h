// Testing.h
#ifndef _TESTING_H_
#define _TESTING_H_

#include <vector>
class Node;
class Model;

class Testing{
public:
    Testing(){}
    virtual ~Testing(){}

    void compareSolutions(bool want, bool get, Model& m,
         std::vector<Node*>& solution);
    void printSolution(std::vector<Node*>& solution);
    bool checkValidSolution(Model& m, std::vector<Node*>& solution);
    void check(bool truthy,const char* msg,...);
};

#endif

