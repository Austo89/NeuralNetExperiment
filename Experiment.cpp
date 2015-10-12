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
#include <string>
#include <random>
#include <float.h>
#include <sstream>
#include <string.h>

Experiment::Experiment(vector<Algorithm *> _a, int _inputs, int _n) {
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

Experiment::Experiment() {

}

void Experiment::nextIteration() {
    // Clear out training and testing data
    trainingData.clear();
    testingData.clear();

    unordered_set<int> selectedIndices;
    int n = dataset.size() / 2;
    int max = dataset.size();
    random_device rd;
    uniform_int_distribution<int> dist(0, max - 1);

    while (selectedIndices.size() < n) {
        selectedIndices.insert(dist(rd));
    }
    // Add each tuple to proper set set
    for (int i = 0; i < dataset.size(); i++) {
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
    for (int t = 0; t < n; t = t + 1) {
        vector<float> newTuple;                 // Create empty vector
        dataset.push_back(newTuple);            // Add new tuple to dataset

        // Loop through each input, plus output
        for (int n = 0; n < inputs + 1; n = n + 1) {
            getline(dataStream, cell, ',');      // Read next "block" from data file
            //dataset.at(t).push_back(stof(cell));       // Add this cell to dataset
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
            //dataset.at(t).push_back(stof(cell));       // Add this cell to dataset
        }
    }

    //printMatrix(dataset);
}

void Experiment::printMatrix(vector<vector<float>> v) {
    vector<vector<float> >::iterator row;
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

void Experiment::getDoubleData(string inputFile) {
    //prep file reader
    ifstream dataReader;
    string junk;

    try {
        dataReader.open(inputFile);
    } catch (ifstream::failure e) {
        cout << "failed to open file!";
        cin >> junk;
    }

    //get the input size and number of data points
    int input_Size, data_Num;
    dataReader >> input_Size >> data_Num;

    inputs = input_Size;
    n = data_Num;

    //read in all the inputs
    for (int i = 0; i < data_Num; i++) {
        dataReader >> junk;
        vector<double> temp_vec;
        for (int j = 0; j < input_Size; j++) {
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

}

void Experiment::normalizeData() {
    //find min and max
    double min = DBL_MAX;
    double max = DBL_MIN;

    for (int i = 0; i < doubleInputData.size(); i++) {
        if (doubleOutputData[i] > max) {
            max = doubleOutputData[i];
        }
        if (doubleOutputData[i] < min) {
            min = doubleOutputData[i];
        }

        for (int j = 0; j < doubleInputData[i].size(); j++) {
            if (doubleInputData[i][j] > max) {
                max = doubleInputData[i][j];
            }
            if (doubleInputData[i][j] < min) {
                min = doubleInputData[i][j];
            }
        }
    }

    //normalize the data points between -1 and 1
    for (int i = 0; i < doubleInputData.size(); i++) {
        doubleOutputData[i] = -1 + (doubleOutputData[i] - min) * (1 - (-1)) / (max - min);

        for (int j = 0; j < doubleInputData[i].size(); j++) {
            doubleInputData[i][j] = -1 + (doubleInputData[i][j] - min) * (1 - (-1)) / (max - min);
        }
    }
}

void Experiment::splitData(){
    //clear old testing and training data
    vector<vector<double>> clr1;
    vector<double> clr2;
    vector<vector<double>> clr3;
    vector<double> clr4;
    doubleTrainingInput = clr1;
    doubleTrainingOutput = clr2;
    doubleTestingInput = clr3;
    doubleTestingOutput = clr4;


    //split training and testing data in half
    srand(time(NULL));
    int trainCount = 0;
    int testCount = 0;
    int iter = 0;
    int total = doubleInputData.size() / 2;
    while (trainCount < total && testCount < total){

        // coin flip on whether it goes in training or testing
        int flip = rand() % 2;
        if (flip == 0){
            doubleTrainingInput.push_back(doubleInputData[iter]);
            doubleTrainingOutput.push_back(doubleOutputData[iter]);
            trainCount++;
        } else {
            doubleTestingInput.push_back(doubleInputData[iter]);
            doubleTestingOutput.push_back(doubleOutputData[iter]);
            testCount++;
        }
        iter++;
    }

    //dump remaining elements in the other half
    if(trainCount > testCount){
        while(iter < doubleInputData.size()){
            doubleTestingInput.push_back(doubleInputData[iter]);
            doubleTestingOutput.push_back(doubleOutputData[iter]);
            iter++;
        }
    } else {
        while(iter < doubleInputData.size()){
            doubleTrainingInput.push_back(doubleInputData[iter]);
            doubleTrainingOutput.push_back(doubleOutputData[iter]);
            iter++;
        }
    }

}

void Experiment::runRBFNExperiment() {
    //initialize
    double sqr_err;
    double sqr_err2;
    double sqr_err3;

    //normalize the read in data
    normalizeData();

    ostringstream convert;   // stream used for the conversion

    convert << inputs;      // insert the textual representation of 'Number' in the characters in the stream

    string inputString = convert.str();

    //create file names for output
    string output1 = "RBFN" + inputString + "I7K";
    string output2 = "RBFN" + inputString + "I9K";
    string output3  = "RBFN" + inputString + "I13K";

    // open document streams
    ofstream dataWriter1;
    dataWriter1.open(output1);
    ofstream dataWriter2;
    dataWriter2.open(output2);
    ofstream dataWriter3;
    dataWriter3.open(output3);

    // 5x2 Cross Validation
    for (int i = 0; i < 5; i++) {
        //create RBFNs with 7, 9, 13 clusters
        RBFNN testNetwork(7, inputs);
        RBFNN testNetwork2(9, inputs);
        RBFNN testNetwork3(13, inputs);

        double temp1;
        double temp2;
        double temp3;

        //do a new random split of the data
        splitData();

        //train each network for this split
        testNetwork.trainNetwork(doubleTrainingInput, doubleTrainingOutput);
        testNetwork2.trainNetwork(doubleTrainingInput, doubleTrainingOutput);
        testNetwork3.trainNetwork(doubleTrainingInput, doubleTrainingOutput);

        //test each network on this split
        temp1 = testNetwork.runTestData(doubleTestingInput, doubleTestingOutput);
        temp2 = testNetwork2.runTestData(doubleTestingInput, doubleTestingOutput);
        temp3 = testNetwork3.runTestData(doubleTestingInput, doubleTestingOutput);

        //accumulate the error
        sqr_err += temp1;
        sqr_err2 += temp2;
        sqr_err3 += temp3;

        //write to file for analysis
        dataWriter1 << temp1 << " ";
        dataWriter2 << temp2 << " ";
        dataWriter3 << temp3 << " ";

        //flip the training and testing around and re-run
        testNetwork.trainNetwork(doubleTestingInput, doubleTestingOutput);
        testNetwork2.trainNetwork(doubleTestingInput, doubleTestingOutput);
        testNetwork3.trainNetwork(doubleTestingInput, doubleTestingOutput);

        temp1 = testNetwork.runTestData(doubleTrainingInput, doubleTrainingOutput);
        temp2 = testNetwork2.runTestData(doubleTrainingInput, doubleTrainingOutput);
        temp3 = testNetwork3.runTestData(doubleTrainingInput, doubleTrainingOutput);

        sqr_err += temp1;
        sqr_err2 += temp2;
        sqr_err3 += temp3;

        //write to file
        dataWriter1 << temp1 << " ";
        dataWriter2 << temp2 << " ";
        dataWriter3 << temp3 << " ";
    }

    //average square error
    sqr_err = sqr_err / 10;
    sqr_err2 = sqr_err2 / 10;
    sqr_err3 = sqr_err3 / 10;

    //output to file
    dataWriter1 << sqr_err;
    dataWriter2 << sqr_err2;
    dataWriter3 << sqr_err3;

    //close files
    dataWriter1.close();
    dataWriter2.close();
    dataWriter3.close();
}