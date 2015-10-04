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

    Algorithm mlp = MultilayerNN(1, 4);
    Experiment e(mlp, 6, 100);
    e.runExperiment();

    return 0;
}

