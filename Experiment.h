//
// Created by Fred Vollmer on 9/29/15.
//

#ifndef GIT_EXPERIMENT_H
#define GIT_EXPERIMENT_H

#include <vector>
#include "Algorithm.h"

using namespace std;

class Experiment {

    vector<Algorithm*> a;    // The algorithm object to run excperiemnt on
    int inputs;     // Number of x for Rosenbrock function
    int n;          // Sample size

    vector<vector<float>> dataset;
    vector<vector<float>> trainingData;
    vector<vector<float>> testingData;
    vector<vector<double>> doubleInputData;
    vector<double> doubleOutputData;
    vector<vector<double>> doubleTrainingInput;
    vector<double> doubleTrainingOutput;
    vector<vector<double>> doubleTestingInput;
    vector<double> doubleTestingOutput;

    vector<Algorithm::runResult> testResults;

public:
    Experiment(vector<Algorithm*> _a, int inputs, int n);
    Experiment(vector<Algorithm*> _a, string file, int _inputs, int _n);
    bool runExperiment();
    void getDoubleData();
private:
    void getData();
    void readData(string file);
    void nextFold();                            // For re-folding
    void nextIteration();                       // For next iteration of CV
    void printMatrix(vector<vector<float>> v);  // Helper method for outputting dataset

};


#endif //GIT_EXPERIMENT_H
