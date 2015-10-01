//
// Created by Fred Vollmer on 9/29/15.
//

#ifndef GIT_EXPERIMENT_H
#define GIT_EXPERIMENT_H

#include <vector>

using namespace std;

class Experiment {

    vector<vector<float>> dataset;
    vector<vector<float>> trainingData;
    vector<vector<float>> testingData;

public:
    Experiment();
    bool runExperiment();
private:
    void getData();
    void nextFold();                // For re-folding
    void nextIteration();           // For next iteration of CV
};


#endif //GIT_EXPERIMENT_H
