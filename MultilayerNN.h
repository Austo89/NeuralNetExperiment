/* 
 * File:   MultilayerNN.h
 * Author: Austo89
 *
 * Created on September 28, 2015, 7:18 AM
 */

#include "Algorithm.h"
#include <vector>

using namespace std;

#ifndef MULTILAYERNN_H
#define	MULTILAYERNN_H

// Helper functions for finding a specific weight
// Input to first hidden
#define weight_input_hidden(input, hidden)      weights.at(0).at(hiddenNodesPerLayer * input + hidden)


class MultilayerNN : public Algorithm {

private:

    bool topoSet  = false;

    int hiddenLayerCount;
    int hiddenNodesPerLayer;
    vector<vector<float>> weights;          // Weight between layer i and j
    vector<float> inputNodes;               // Value of node in input layer
    vector<float> outputNodes;              // Value of nodes in output layer
    vector<vector<float>> hiddenNodes;      // Values of node x in layer i (hidden layers)

    void setTopo(vector<float> tuple);      // Creates network structure and randomizes weights
    void feedForward();  // Calculate a network given training tuple
    float activate(float S);                // Sigmoid activation function (logistic)

public:
    MultilayerNN(int hiddenLayers, int hiddenNodes);
    MultilayerNN(const MultilayerNN& orig);
    virtual ~MultilayerNN();

    void trainOne(vector<float> tuple);

};

#endif	/* MULTILAYERNN_H */

