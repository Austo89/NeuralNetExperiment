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

    //MultilayerNN mlp = MultilayerNN("0layer",0,7,"sigmoid",-1,0.001,20000,0.02);
    MultilayerNN mlp2 = MultilayerNN("1layer",1,7,"sigmoid",-1,0.001,20000,0.02);
    Experiment e({&mlp2}, 6, 15000);
    e.runExperiment();

    //e.getDoubleData();

    return 0;
}

