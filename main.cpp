/* 
 * File:   main.cpp
 * Author: Austo89
 *
 * Created on September 28, 2015, 7:12 AM
 */

#include <cstdlib>
#include "DataGenerator.h"
#include "Experiment.h"
#include "Algorithm.h"
#include "MultilayerNN.h"
#include <iostream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    //DataGenerator steve = DataGenerator(6);
    //steve.generateData(200);


    Algorithm mlp = MultilayerNN(1,3,0.001,.001,2000000000,0.01);
    Experiment e(mlp, 2, 500);
    //e.runExperiment();

    e.getDoubleData();

    //Experiment e(mlp, 6, 15000);
    //e.runExperiment();

    return 0;
}

