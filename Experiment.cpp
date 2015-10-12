//
// Created by Fred Vollmer on 9/29/15.
//

#include "Experiment.h"
#include "DataGenerator.h"
#include "Algorithm.h"
#include "MultilayerNN.h"
#include "RBFNN.h"
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
    DataGenerator::normalizeData(dataset);
}

Experiment::Experiment(vector<Algorithm*> _a, string file, int _inputs, int _n) {
    a = _a;
    inputs = _inputs;
    n = _n;
    readData(file);
    DataGenerator::normalizeData(dataset);
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

void Experiment::readData(string file) {
    ifstream dataStream;
    string cell;

    // Open data file for reading
    dataStream.open(file);
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

    vector<fstream> resultStream;
    for (int rs = 0; rs < a.size(); rs++) {
        resultStream.push_back(fstream());
        resultStream.back().open("results_" + a[rs]->className() + "_" + to_string(inputs) + "input_" + a[rs]->nickname + ".txt", ofstream::out | ofstream::trunc);
        // Check for stream error
        if (resultStream.back().fail()) {
            cerr << "open stream failure at rs: " << to_string(rs) << strerror(errno) << '\n';
        }
    }

    // 5X
    for (int i = 0; i < 5; i++) {
        nextIteration();
        // 2 CV
        for (int j = 0; j < 2; j++) {
            nextFold();

            // Train and test all algorithms
            for (int alg = 0; alg < a.size(); alg++) {
                a[alg] -> reset();
                a[alg] -> train(trainingData);
                resultStream.at(alg) << a[alg] -> test(testingData) << endl;
            }
        }
    }

    // Close writers
    for (int rs = 0; rs < a.size(); rs++) {
        resultStream[rs].close();
    }

    return true;
}

void Experiment::getDoubleData() {
    //prep file reader
    ifstream dataReader;
    string junk;

    try{
        dataReader.open("data2input.txt");
    } catch (ifstream::failure e){
        cout << "failed to open file!";
        cin >> junk;
    }

    //get the input size and number of data points
    int input_Size, data_Num;
    dataReader >> input_Size >> data_Num;


    //read in all the inputs
    for (int i = 0; i < data_Num; i++){
        dataReader >> junk;
        vector<double> temp_vec;
        for (int j = 0; j < input_Size; j++){
            double temp;
            dataReader >> temp;
            temp_vec.push_back(temp);
        }
        doubleInputData.push_back(temp_vec);
        double temp_output;
        dataReader >> junk;
        dataReader >> temp_output;
        doubleOutputData.push_back(temp_output);
    }

    //temporary set up of inputs and outputs for testing
    for (int i = 0; i < 100; i++){
        doubleTrainingInput.push_back(doubleInputData[i]);
        doubleTrainingOutput.push_back(doubleOutputData[i]);
    }
    for (int i = 100; i < 200; i++){
        doubleTestingInput.push_back(doubleInputData[i]);
        doubleTestingOutput.push_back(doubleOutputData[i]);
    }

    RBFNN steve = RBFNN(9, input_Size);
    steve.trainNetwork(doubleTrainingInput,doubleTrainingOutput);

    RBFNN tim = RBFNN(7, input_Size);
    tim.trainNetwork(doubleTrainingInput,doubleTrainingOutput);

    RBFNN eric = RBFNN(5, input_Size);
    eric.trainNetwork(doubleTrainingInput,doubleTrainingOutput);

    vector<double> test;
    test.push_back(6.90001032);
    test.push_back(5.05418397);

    vector<double> test2;
    test2.push_back(5.91490540);
    test2.push_back(5.54784913);

    vector<double> test3;
    test3.push_back(9.49143368);
    test3.push_back(3.07335744);

    double testrun1 = steve.runModel(test);
    double testrun2 = steve.runModel(test2);
    double testrun3 = steve.runModel(test3);

    double testrun4 = tim.runModel(test);
    double testrun5 = tim.runModel(test2);
    double testrun6 = tim.runModel(test3);

    double testrun7 = eric.runModel(test);
    double testrun8 = eric.runModel(test2);
    double testrun9 = eric.runModel(test3);
    cout << "yes!";
}