/* 
 * File:   MultilayerNN.cpp
 * Author: Austo89
 * 
 * Created on September 28, 2015, 7:18 AM
 */

#include "MultilayerNN.h"
#include <math.h>
#include <random>
#include <iostream>
#include <vector>

MultilayerNN::MultilayerNN(int hiddenLayers, int hiddenNodes) {
    hiddenLayerCount = hiddenLayers;
    hiddenNodesPerLayer = hiddenNodes;
}

MultilayerNN::MultilayerNN(const MultilayerNN& orig) {
}

MultilayerNN::~MultilayerNN() {
}

void MultilayerNN::trainOne(vector<float> tuple) {
    // Set up topography & randomize weights if first run
    if (!topoSet) {
        setTopo(tuple);
    }

    // Set input nodes to training tuple values
    for (int i = 0; i < inputNodes.size(); i++) {
        inputNodes.at(i) = tuple.at(i);
    }

    // FIRST PASS: Feed forward through network
    feedForward();


}

void MultilayerNN::setTopo(vector<float> tuple) {
    random_device rd;                                               // Initialize random device & distribution
    uniform_real_distribution<float> dist(-0.5f, 0.5f);
    outputNodes.resize(1);                                          // We create 1 output node
    inputNodes.resize(tuple.size() - 1);                            // One node per input
    // Setup weights vector
    weights.resize(2+(hiddenLayerCount-1));                         // One set of weights between each layer
    weights.at(0).resize(inputNodes.size()*hiddenNodesPerLayer);    // Weights between input and hidden(0)
    for (int l = 1; l < hiddenLayerCount; l++) {                    // Weights between hidden layers
        weights.at(l).resize(hiddenNodesPerLayer * hiddenNodesPerLayer);
    }
    weights.push_back(vector<float>(hiddenNodesPerLayer*outputNodes.size()));

    // Randomize weights
    // Input layer to first hidden layer
    for (auto &weight : weights.at(0)) {
        weight *= dist(rd);
    }

    // Between hidden layers
    for (int l = 1; l < hiddenLayerCount; l++) {
        for (auto &weight : weights.at(l)) {
            weight *= dist(rd);
        }
    }

    // Hidden to output
    for (auto &weight : weights.at(hiddenLayerCount+1)) {
        weight *= dist(rd);
    }

    topoSet = true;                                                 // Set topography to "set"
}

void MultilayerNN::feedForward() {
    // First we go from input layer to first hidden layer
    // Loop per hidden node
    for (int hiddenNode = 0; hiddenNode < hiddenNodesPerLayer; hiddenNode++) {
        // Zero out hidden node
        hiddenNodes.at(0).at(hiddenNode) = 0.0f;
        // Then per input node
        for (int inNode = 0; inNode < inputNodes.size(); inNode++) {
            hiddenNodes.at(0).at(hiddenNode) += inputNodes.at(inNode) * weight_input_hidden(inNode, hiddenNode);
        }
        // Activate hidden node!!!
        hiddenNodes.at(0).at(hiddenNode) = activate(hiddenNodes.at(0).at(hiddenNode));
    }
}

float MultilayerNN::activate(float S) {
    return (1.0f / (1.0f + exp(-1.0f * S)));
}


