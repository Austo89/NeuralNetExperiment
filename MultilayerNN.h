/* 
 * File:   MultilayerNN.h
 * Author: Austo89
 *
 * Created on September 28, 2015, 7:18 AM
 */

#include "Algorithm.h"
#include <vector>
#include <math.h>

using namespace std;

#ifndef MULTILAYERNN_H
#define	MULTILAYERNN_H

// Derivative of tanh
#define sech2(x)                                (1.0 - (tanh(x) * tanh(x)))

// Helper functions for finding a specific weight
// Input to first hidden
#define weight_input_hidden(input, hidden)      weights.at(0).at(hiddenNodesPerLayer * input + hidden)
#define past_weight_input_hidden(input, hidden) previousWeights.at(0).at(hiddenNodesPerLayer * input + hidden)
// Hidden to hidden
#define weight_hidden_hidden(layer, i, j)       weights.at(layer).at(i * hiddenNodesPerLayer + j)
#define past_weight_hidden_hidden(layer, i, j)  previousWeights.at(layer).at(i * hiddenNodesPerLayer + j)

// Hidden to output
#define weight_hidden_output(hidden, output)    weights.back().at(output * hiddenNodesPerLayer + hidden)
#define past_weight_hidden_output(hidden, output) previousWeights.back().at(output * hiddenNodesPerLayer + hidden)

// Helper functions to calculate deltas
// For output nodes
#define delta_output(outNode, target)           (target - outputNodes.at(outNode));
// outputNodes.at(outNode) * (1.0f- outputNodes.at(outNode)) *


class MultilayerNN : public Algorithm {

private:

    bool topoSet  = false;

    int hiddenLayerCount;
    int hiddenNodesPerLayer;
    float momentum;
    float learningRate;
    int iterations;
    float targetMSE;
    vector<vector<float>> weights;          // Weight between layer i and j
    vector<vector<float>> tempWeights;
    vector<vector<float>> previousWeights;  // Stores weights from last pattern
    vector<float> inputNodes;               // Value of node in input layer
    vector<float> outputNodes;              // Value of nodes in output layer
    vector<vector<float>> hiddenNodes;      // Values of node x in layer i (hidden layers)
    vector<float> inputDelats;              // Deltas for input layer
    vector<float> outputDeltas;             // Deltas for output layer
    vector<vector<float>> hiddenDeltas;     // Deltas for hidden layers

    void setTopo(vector<float> tuple);      // Creates network structure and randomizes weights
    void feedForward();                     // Calculate a network given training tuple
    float addErrorForIteration(float target);// Calculates squared error for one training pattern
    void backProp(float target);            // Calculate error and propagate deltas back
    void updateWeights();                   // Update weights
    float activate(float S);                // Sigmoid activation function (logistic)
    float trainOne(vector<float> tuple);

public:
    MultilayerNN(int hiddenLayers, int hiddenNodes, float momentum, float teachingStep,
                 int iterations, float targetMSE);
    MultilayerNN(const MultilayerNN& orig);
    virtual ~MultilayerNN();
    vector<float> train(vector<vector<float>> tset) override;
};

#endif	/* MULTILAYERNN_H */

