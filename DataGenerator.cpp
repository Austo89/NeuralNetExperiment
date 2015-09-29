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

using namespace std;

DataGenerator::DataGenerator(int in_inputNum) {
    inputNum = in_inputNum;
}

DataGenerator::DataGenerator(const DataGenerator& orig) {
}

DataGenerator::~DataGenerator() {
}

void DataGenerator::generateData(int dataNum) {
    //prepare to write to data file
    ofstream dataWriter;
    dataWriter.open("data.txt");

    for (int d = 0; d < dataNum; d++) {
        //initialize new inputs array
        int x[inputNum] = {};
        dataWriter << "x ";

        //generate inputs
        for (int i = 0; i < inputNum; i++) {
            x[i] = std::rand() % 10;
            dataWriter << x[i];
            dataWriter << " ";
        }


        dataWriter << "y ";
        int sum = 0;
        //calculate rosenbrock function for the inputs
        for (int i = 0; i < inputNum - 1; i++) {
            sum += ((1 - x[i]) * (1 - x[i])) + 100 *
                    ((x[i + 1] - (x[i] * x[i])) * (x[i + 1] - (x[i] * x[i])));
        }
        dataWriter << sum;
        dataWriter << "\n";
    }

    dataWriter.close();
}

