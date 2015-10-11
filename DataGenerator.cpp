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
<<<<<<< HEAD
#include <random>
#include <iomanip>
#include <time.h>
=======
#include <iomanip>
//#include <random>
#include <random>
#include <iomanip>
>>>>>>> 9d0019a4075fd420dca2b461d9d1ee4dfd380b91

using namespace std;

DataGenerator::DataGenerator(int in_inputNum) {
    inputNum = in_inputNum;
}

DataGenerator::DataGenerator(const DataGenerator &orig) {
}

DataGenerator::~DataGenerator() {
}

void DataGenerator::generateData(int dataNum) {
    // Sed rand
    srand(time(NULL));

    //prepare to write to data file
    ofstream dataWriter;
    dataWriter.open("data.txt", ofstream::out | ofstream::trunc);


    // Check for stream error
    if (dataWriter.fail()) {
        cerr << "open stream failure: " << strerror(errno) << '\n';
    }

<<<<<<< HEAD
    double x[inputNum];
=======

        //double x[inputNum];

        dataWriter << "y ";
        double sum = 0;
        //dataWriter.open("data.txt", ofstream::out | ofstream::trunc);

        // Random data generator
        //random_device rd;

        // Check for stream error
        // if (dataWriter.fail()) {
        //    cerr << "open stream failure: " << strerror(errno) << '\n';
        //}

        //int x[inputNum];

        //initialize new inputs array
>>>>>>> 9d0019a4075fd420dca2b461d9d1ee4dfd380b91

    for (int d = 0; d < dataNum; d++) {

        //generate inputs
<<<<<<< HEAD
=======
        //for (int i = 0; i < inputNum; i++) {
        //    x[i] = rd() % 2;
        //    dataWriter << x[i];
        //    dataWriter << ",";
        //}
>>>>>>> 9d0019a4075fd420dca2b461d9d1ee4dfd380b91
        for (int i = 0; i < inputNum; i++) {
            x[i] = ((float) std::rand() / (float) RAND_MAX) * 10;
            dataWriter << std::fixed << std::setprecision(8) << x[i];
            dataWriter << ",";
        }

<<<<<<< HEAD
        double sum = 0;
=======
        //dataWriter << "y ";

        //double sum = 0;
        //double sum = sin(x[0] + x[1]);
        // double sum = sin(x[0] * x[1] * x[3]);
        //int sum;
        //if ((x[0] == 1 || x[1] == 1) && (x[0] + x[1] < 2)) sum = 1;
        //else sum = 0;
>>>>>>> 9d0019a4075fd420dca2b461d9d1ee4dfd380b91

        //calculate rosenbrock function for the inputs

        for (int i = 0; i < inputNum - 1; i++) {
<<<<<<< HEAD
            sum += ((1 - x[i]) * (1 - x[i])) + 100 *
                                               ((x[i + 1] - (x[i] * x[i])) * (x[i + 1] - (x[i] * x[i])));
        }
        dataWriter << sum;
        dataWriter << ",";
=======
            sum += ((1 - x[i]) * (1 - x[i])) + 100 * ((x[i + 1] - (x[i] * x[i])) * (x[i + 1] - (x[i] * x[i])));
        }
        dataWriter << std::fixed << std::setprecision(8) << sum;
        dataWriter << "\n";

        //dataWriter << sum;
        //dataWriter << ",";
        //((x[i + 1] - (x[i] * x[i])) * (x[i + 1] - (x[i] * x[i])));
>>>>>>> 9d0019a4075fd420dca2b461d9d1ee4dfd380b91
    }
    //dataWriter << sum;
    //dataWriter << ",";


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
        //min = *(min_element(begin(tempCol), end(tempCol)));
        //max = *(max_element(begin(tempCol), end(tempCol)));

        // Normalize column
        for (int r = 0; r < data.size(); r++) {
            data[r][col] = -1 + (data[r][col] - min) * (1 - (-1)) / (max - min);
        }
    }
}

