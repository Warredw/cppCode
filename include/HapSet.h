

#ifndef CPPCODE_HAPSET_H
#define CPPCODE_HAPSET_H

#include<iostream>
#include<vector>
#include <fstream>
#include <ilcplex/ilocplex.h>

using std::string;
using std::vector;


class HapSet {

private:
    int** hapSetSecondHalf;
    int** hapSetFirstHalf;
    int nrTeams;
    string dNotation;
    string type; 

public:
    explicit HapSet(const string& dNotation, const string& type);
    ~HapSet(); 
    void makeHapSetFirstHalf();
    void makeHapSetSecondHalf();

    int** getHapSetSecondHalf () const;
    int** getHapSetFirstHalf () const;

};

#endif //CPPCODE_HAPSET_H




