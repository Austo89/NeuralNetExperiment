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
    //steve.generateData(400);

    //Algorithm mlp = MultilayerNN(1,3,0.001,.001,2000000000,0.01);
    Experiment e1 = Experiment();
    e1.getDoubleData("data2input.txt");
    e1.runRBFNExperiment();

    Experiment e2 = Experiment();
    e2.getDoubleData("data3input.txt");
    e2.runRBFNExperiment();

    Experiment e3 = Experiment();
    e3.getDoubleData("data4input.txt");
    e3.runRBFNExperiment();

    Experiment e4 = Experiment();
    e4.getDoubleData("data5input.txt");
    e4.runRBFNExperiment();

    Experiment e5 = Experiment();
    e5.getDoubleData("data6input.txt");
    e5.runRBFNExperiment();


    //e.runExperiment();

    //e.getDoubleData();

    //Experiment e(mlp, 6, 15000);
    //e.runExperiment();
    //MultilayerNN mlp = MultilayerNN(1,3,0.001,.001,2000000000,0.01);
    //MultilayerNN mlp2 = MultilayerNN(1,3,0.001,.001,2000000000,0.01);
    //Experiment e({&mlp, &mlp2}, 6, 15000);
    //e.runExperiment();

    return 0;
}

