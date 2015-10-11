//
// Created by Fred Vollmer on 9/29/15.
//

#include "Experiment.h"
#include "DataGenerator.h"
#include "Algorithm.h"
#include "MultilayerNN.h"
#include <unordered_set>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>

Experiment::Experiment(vector<Algorithm*> _a, int _inputs, int _n) {
    // Set parameters
    a = _a;

    n = _n;
    inputs = _inputs;

    // Generate dataset
    getData();
}

void Experiment::nextIteration () {
    // Clear out training and testing data
    trainingData.clear();
    testingData.clear();

    unordered_set<int> selectedIndices;
    int n = dataset.size() / 2;
    int max = dataset.size();
    random_device rd;
    uniform_int_distribution<int> dist(0, max-1);

    while (selectedIndices.size() < n) {
        selectedIndices.insert(dist(rd));
    }
    // Add each tuple to proper set set
    for(int i = 0; i < dataset.size(); i++) {
        if (selectedIndices.count(i) > 0) {
            trainingData.push_back(dataset.at(i));
        } else {
            testingData.push_back(dataset.at(i));
        }
    }
}

// Flip training and testing data
void Experiment::nextFold() {
    vector<vector<float>> temp;
    temp = testingData;
    testingData = trainingData;
    trainingData = temp;
}

void Experiment::getData() {
    ifstream dataStream;
    string cell;

    // Generate data with given parameters
    DataGenerator gen = DataGenerator(inputs);
    gen.generateData(n);

    // Open data file for reading
    dataStream.open("data.txt");
    if (dataStream.fail()) {                    // Check for stream error
        cerr << "Read stream failure: " << strerror(errno) << '\n';
    }
    // Loop through each tuple
    for (int t = 0; t < n; t = t+1) {
        vector<float> newTuple;                 // Create empty vector
        dataset.push_back(newTuple);            // Add new tuple to dataset

        // Loop through each input, plus output
        for (int n = 0; n < inputs + 1; n = n+1) {
            getline(dataStream, cell, ',');      // Read next "block" from data file
            dataset.at(t).push_back(stof(cell));       // Add this cell to dataset
        }
    }

    //printMatrix(dataset);

}

void Experiment::printMatrix(vector<vector<float>> v) {
    vector< vector<float> >::iterator row;
    vector<float>::iterator col;
    for (row = v.begin(); row != v.end(); row++) {
        for (col = row->begin(); col != row->end(); col++) {
            cout << *col << " ";
        }
        cout << "\n";
    }
}

bool Experiment::runExperiment() {

    testResults.resize(10);

    MultilayerNN mlp = MultilayerNN(6,12,0.0002,0.0002,100,0.0000001);


    // 5X
    for (int i = 0; i < 5; i++) {
        nextIteration();
        // 2 CV
        for (int j = 0; j < 2; j++) {
            nextFold();

            // Train and test all algorithms
            for (auto algo : a) {
                algo -> reset();
                algo -> train(trainingData);
                testResults.push_back(algo -> test(testingData));
            }

        }
    }

    return true;
}