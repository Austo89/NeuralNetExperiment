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

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    MultilayerNN mlp = MultilayerNN(1,3,0.001,.001,2000000000,0.01);
    MultilayerNN mlp2 = MultilayerNN(1,3,0.001,.001,2000000000,0.01);
    Experiment e({&mlp, &mlp2}, 6, 15000);
    e.runExperiment();

    return 0;
}

