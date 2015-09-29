/* 
 * File:   DataGenerator.cpp
 * Author: Austo89
 * 
 * Created on September 28, 2015, 7:13 AM
 */

#include "DataGenerator.h"
#include <cstdlib>
#include <iostream>

DataGenerator::DataGenerator(int in_inputNum) {
    inputNum = in_inputNum;
}

DataGenerator::DataGenerator(const DataGenerator& orig) {
}

DataGenerator::~DataGenerator() {
}

void DataGenerator::generateData(double data[][2], int dataNum){
    std::ofstream dataWriter;
    dataWriter.open("data.txt");
    int inputs[inputNum] = {};
    for (int i = 0; i < inputNum; i++){
        inputs[i] = std::rand() % 100;
        dataWriter << inputs[i];
    }
    
    
}

