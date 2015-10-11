//
// Created by Fred Vollmer on 10/1/15.
//

#ifndef ASSIGNMENT2_ALGORITHM_H
#define ASSIGNMENT2_ALGORITHM_H

#include <vector>
#include <string>

using namespace std;

class Algorithm {

public:

    struct runResult {
        string algo;
        float error;
    };

    virtual vector<float> train(vector<vector<float>> tset) {
        return vector<float>();
    }

    virtual runResult test(vector<vector<float>> testSet) {
        return runResult();
    }

    virtual void reset() {

    }
};


#endif //ASSIGNMENT2_ALGORITHM_H
