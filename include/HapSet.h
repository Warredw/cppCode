

#ifndef CPPCODE_HAPSET_H
#define CPPCODE_HAPSET_H

#include<iostream>
#include<vector>
#include <fstream>
#include <ilcplex/ilocplex.h>


using std::string;
using std::vector;

class HapSet {

public:

    int** hapSetSecondHalf;
    int** hapSetFirstHalf;

    int nrTeams;
    string dNotation;

    explicit HapSet(const string& dNotation);
    void makeHapSetFirstHalf();
    void makeHapSetSecondHalf();
};

#endif //CPPCODE_HAPSET_H
