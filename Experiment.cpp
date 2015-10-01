//
// Created by Fred Vollmer on 9/29/15.
//

#include "Experiment.h"
#include <unordered_set>
#include <stdlib.h>

Experiment::Experiment() {
    // Constructor
}

void Experiment::nextIteration () {
    // Clear out training and testing data
    trainingData.clear();
    testingData.clear();

    unordered_set<int> selectedIndices;
    int n = dataset.size() / 2;
    int max = dataset.size();
    while (selectedIndices.size() < n) {
        selectedIndices.insert(rand() % (max+1));
    }
    // Add selected index to training set
    for(int i = 0; i < dataset.size(); i++) {
        if (selectedIndices.count(i) > 0) {
            trainingData.push_back(dataset.at(i));
        } else {
            testingData.push_back(dataset.at(i));
        }
    }
}