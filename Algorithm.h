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

    string nickname;

    struct runResult {
        string algo;
        float error;
    };

    virtual vector<float> train(vector<vector<float>> tset) {
        return vector<float>();
    }


    virtual float test(vector<vector<float>> testSet) {
        return 0;
    }

    virtual string const className() { return ""; }

    virtual void reset() {

    }
};


#endif //ASSIGNMENT2_ALGORITHM_H
