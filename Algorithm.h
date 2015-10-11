//
// Created by Fred Vollmer on 10/1/15.
//

#ifndef ASSIGNMENT2_ALGORITHM_H
#define ASSIGNMENT2_ALGORITHM_H

#include <vector>

using namespace std;

class Algorithm {

public:
    virtual vector<float> train(vector<vector<float>> tset) {
        return vector<float>();
    }
};


#endif //ASSIGNMENT2_ALGORITHM_H
