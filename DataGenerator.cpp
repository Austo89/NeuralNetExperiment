/* 
 * File:   DataGenerator.cpp
 * Author: Austo89
 * 
 * Created on September 28, 2015, 7:13 AM
 */

#include "DataGenerator.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>

#include <bits/algorithmfwd.h>

#include <time.h>


using namespace std;

DataGenerator::DataGenerator(int in_inputNum) {
    inputNum = in_inputNum;
}

DataGenerator::DataGenerator(const DataGenerator &orig) {
}

DataGenerator::~DataGenerator() {
}

void DataGenerator::generateData(int dataNum) {
    // Seed rand
    srand(time(NULL));

    //prepare to write to data file
    ofstream dataWriter;
    dataWriter.open("dataxinput.txt");
    dataWriter << inputNum << " " << dataNum << "\n";
    srand(time(NULL));

    for (int d = 0; d < dataNum; d++) {
        //initialize new inputs array
        double x[inputNum];
        dataWriter << "x ";


        //generate inputs
        for (int i = 0; i < inputNum; i++) {
            x[i] = ((double) std::rand() / (double) RAND_MAX) * 2;
            dataWriter << std::fixed << std::setprecision(8) << x[i];
            dataWriter << " ";
        }


        //double x[inputNum];

        dataWriter << "y ";
        double sum = 0;



        for (int i = 0; i < inputNum - 1; i++) {
            sum += ((1 - x[i]) * (1 - x[i])) + 100 *
                                               ((x[i + 1] - (x[i] * x[i])) * (x[i + 1] - (x[i] * x[i])));
        }

        dataWriter << std::fixed << std::setprecision(8) << sum;
        dataWriter << ",";

    }
    dataWriter.close();
}

void DataGenerator::normalizeData(vector<vector<float>> &data) {
    // Loop column by column
    float max;
    float min;
    vector<float> tempCol;
    for (int col = 0; col < data[0].size(); col++) {
        // Create column vector
        tempCol.resize(data.size());
        for (int r = 0; r < data.size(); r++) {
            tempCol.push_back(data[r][col]);
        }
        // Find min/max
        min = *(min_element(begin(tempCol), end(tempCol)));
        max = *(max_element(begin(tempCol), end(tempCol)));

        // Normalize column
        for (int r = 0; r < data.size(); r++) {
            data[r][col] = -1 + (data[r][col] - min) * (1 - (-1)) / (max - min);
        }
    }
}

