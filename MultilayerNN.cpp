/* 
 * File:   MultilayerNN.cpp
 * Author: Austo89
 * 
 * Created on September 28, 2015, 7:18 AM
 */

#include "MultilayerNN.h"
#include <random>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <string>

MultilayerNN::MultilayerNN(string _nickname, int hiddenLayers, int hiddenNodes, string _actFunc, float _momentum, float _learningRate,
                           int _iterations, float _targetMSE) {
    nickname = _nickname;
    hiddenLayerCount = hiddenLayers;
    hiddenNodesPerLayer = hiddenNodes;
    momentum = _momentum;
    learningRate = _learningRate;
    iterations = _iterations;
    targetMSE = _targetMSE;
    if (_actFunc.compare("sigmoid") == 0) sigmoid = true;
}

MultilayerNN::MultilayerNN(const MultilayerNN &orig) {
}

MultilayerNN::~MultilayerNN() {
}

vector<float> MultilayerNN::train(vector<vector<float>> tset) {
    ofstream dataWriter;
    ofstream dataWriter2;

    random_device rd;
    uniform_int_distribution<int> dist;

    dataWriter.open("MLP_ITerror.txt", ofstream::out | ofstream::trunc);
    dataWriter2.open("MLP_weights.txt", ofstream::out | ofstream::trunc);

    bool deltaStatus = true;
    float changeRate = 0;
    float mse = 999.99;
    int iteration = 0;
    vector<float> errors;
    while (deltaStatus && iteration < iterations) {
        //&& iteration < iterations) {
        // Zero out MSE
        mse = 0;
        // Shuffle training set for randomness
        //random_shuffle(tset.begin(), tset.end());

        // Train with each tuple
        /* for (auto &tuple : tset) {
             mse += trainOne(tuple);
         } */

        for (int i = 0; i < tset.size(); i++) {
            mse += trainOne(tset.at(dist(rd) % tset.size()));
        }

        dataWriter2 << iteration << "," << weights.at(0).at(2) << "," << weights.back().at(2) << endl;

        mse /= tset.size();

        // Save mse for this iteration
        //errors.push_back(mse / tset.size());
        cout << iteration << " ====> " << mse << endl;
        dataWriter << iteration << "," << mse << endl;

        // Determine whether error rate of change is sufficient to continue
        if (iterations > 20) {
            if (mse - lastError > -0.001) {
                noDecreaseCount++;
            } else {
                noDecreaseCount = 0;
            }

            if (noDecreaseCount == 5) deltaStatus = false;
        }

        lastError = mse;

        iteration++;
    }
    return errors;
}

float MultilayerNN::trainOne(vector<float> tuple) {

    float error;
    // Set up topography & randomize weights if first run
    if (!topoSet) {
        setTopo(tuple);
    }

    // Set previous weights if size = 0
    //if (previousWeights.size() == 0) previousWeights = weights;

    // Set input nodes to training tuple values
    for (int i = 0; i < inputNodes.size(); i++) {
        inputNodes.at(i) = tuple.at(i);
    }

    // FIRST PASS: Feed forward through network
    feedForward();

    // SECOND PASS: Calculate error, propagate deltas back through network given desired output
    error = addErrorForIteration(tuple.back());
    backProp(tuple.back());

    // THIRD PASS: Update weights
    updateWeights();

    return error;
}

void MultilayerNN::setTopo(vector<float> tuple) {
    random_device rd;                                               // Initialize random device & distribution
    uniform_real_distribution<float> dist(-0.3f, 0.3f);
    outputNodes.resize(1);                                          // We create 1 output node
    inputNodes.resize(tuple.size() - 1);                            // One node per input
    hiddenNodes.resize(hiddenLayerCount);                           // Set hidden layers
    for (auto &layer : hiddenNodes) {
        layer.resize(hiddenNodesPerLayer);                          // Set hidden nodes in each layer
    }

    // Setup weights vector
    weights.resize(hiddenLayerCount + 1);                             // One set of weights between each layer
    weights.at(0).resize(inputNodes.size() * hiddenNodesPerLayer);    // Weights between input and hidden(0)
    for (int l = 1; l <= hiddenLayerCount; l++) {                    // Weights between hidden layers
        weights.at(l).resize(hiddenNodesPerLayer * hiddenNodesPerLayer);
    }
    weights.back().resize(hiddenNodesPerLayer * outputNodes.size());   // Weights between hidden and output

    // Setup delta vectors
    outputDeltas.resize(outputNodes.size());
    inputDelats.resize(inputNodes.size());
    hiddenDeltas.resize(hiddenLayerCount);
    for (auto &layer : hiddenDeltas) {
        layer.resize(hiddenNodesPerLayer);                          // Set hidden deltas in each layer
    }

    // Randomize weights
    // Input layer to first hidden layer
    for (auto &weight : weights.at(0)) {
        weight = dist(rd);
    }

    // Between hidden layers
    for (int l = 1; l <= hiddenLayerCount; l++) {
        for (auto &weight : weights.at(l)) {
            weight = dist(rd);
        }
    }

    // Hidden to output
    for (auto &weight : weights.at(hiddenLayerCount)) {
        weight = dist(rd);
    }

    topoSet = true;                                                 // Set topography to "set"
}

void MultilayerNN::feedForward() {

    // If no hidden layers, feed from input to output layers
    if (hiddenLayerCount < 1) {
        // Loop through each output node
        for (int outNode = 0; outNode < outputNodes.size(); outNode++) {
            // Zero out output node
            outputNodes.at(outNode) = 0;
            // Loop through nodes in input layer
            for (int inNode = 0; inNode < inputNodes.size(); inNode++) {
                outputNodes.at(outNode) += inputNodes.at(inNode) * weights.at(0).at(inNode);
            }
            // Activate that shit
            //outputNodes.at(outNode) = activate(outputNodes.at(outNode));
        }
    } else {
        // Feed through hidden layers
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
            if (sigmoid) {
                hiddenNodes.at(0).at(hiddenNode) = activate(hiddenNodes.at(0).at(hiddenNode));
            }
        }

        // Next we feed between all hidden layers: hidLay represents hiddenNodes vector index
        // Runs only if more than one hidden layer
        for (int hidLay = 1; hidLay < hiddenLayerCount; hidLay++) {
            // For each node in hidLay, calculate S
            for (int thisNode = 0; thisNode < hiddenNodesPerLayer; thisNode++) {
                // Zero out node
                hiddenNodes.at(hidLay).at(thisNode) = 0;
                // Loop through each node in previous hidden layer to calc S
                for (int prevNode = 0; prevNode < hiddenNodesPerLayer; prevNode++) {
                    // Multiply value of node in this layer -1 by weight connecting these two layers
                    hiddenNodes.at(hidLay).at(thisNode) +=
                            hiddenNodes.at(hidLay - 1).at(prevNode) * weight_hidden_hidden(hidLay, prevNode, thisNode);
                }
                // Activate this node!!!
                if (sigmoid) {
                    hiddenNodes.at(hidLay).at(thisNode) = activate(hiddenNodes.at(hidLay).at(thisNode));
                }
            }
        }

        // Now we feed from last hidden layer to output
        // Loop through each output node
        for (int outNode = 0; outNode < outputNodes.size(); outNode++) {
            // Zero out output node
            outputNodes.at(outNode) = 0;
            // Loop through nodes in last hidden layer, find S for this output node
            for (int prevNode = 0; prevNode < hiddenNodesPerLayer; prevNode++) {
                outputNodes.at(outNode) += hiddenNodes.back().at(prevNode) * weight_hidden_output(prevNode, outNode);
            }
            // Activate that shit
            //outputNodes.at(outNode) = activate(outputNodes.at(outNode));
        }
    }
}

void MultilayerNN::backProp(float target) {
    // Calculate deltas for output nodes
    for (int outNode = 0; outNode < outputNodes.size(); outNode++) {
        outputDeltas.at(outNode) = delta_output(outNode, target);
    }

    // Only proceed ifg there are hidden layers
    if (hiddenLayerCount > 0) {

        // Calculate deltas for last hidden layer
        for (int hidNode = 0; hidNode < hiddenNodesPerLayer; hidNode++) {
            // Zero out delta from last iteration
            hiddenDeltas.back().at(hidNode) = 0;
            // Calculate summation across output nodes connected to this node
            for (int outNode = 0; outNode < outputNodes.size(); outNode++) {
                hiddenDeltas.back().at(hidNode) += outputDeltas.at(outNode) * weight_hidden_output(hidNode, outNode);
            }
            // Multiply by derivative of activation function
            hiddenDeltas.back().at(hidNode) *= sech2(hiddenNodes.back().at(hidNode));
        }

        // Now we go back through previous hidden layers
        // Start at second to last hidden layer
        for (int layer = hiddenLayerCount - 2; layer >= 0; layer--) {
            // Calculate summation across output nodes connected to this node
            for (int thisNode = 0; thisNode < hiddenNodesPerLayer; thisNode++) {
                // Zero out this node's delta form last iteration
                hiddenDeltas.at(layer).at(thisNode) = 0;
                // Calculate summation across adjacent (to right) layer's nodes connected to this node
                for (int nextNode = 0; nextNode < hiddenNodesPerLayer; nextNode++) {
                    hiddenDeltas.at(layer).at(thisNode) +=
                            hiddenDeltas.at(layer + 1).at(nextNode) *
                            weight_hidden_hidden(layer + 1, thisNode, nextNode);
                }
                // Multiply by derivative of activation function to complete delta rule for this node.
                hiddenDeltas.at(layer).at(thisNode) *= sech2(hiddenNodes.at(layer).at(thisNode));
            }
        }
    }
}

void MultilayerNN::updateWeights() {
    // Save previous weights
    //tempWeights = weights;

    // If no hidden layers, only one weight layer to update
    if (hiddenLayerCount < 1) {
        for (int inNode = 0; inNode < inputNodes.size(); inNode++) {
            // Loop through each hidden node in first hidden layer
            for (int outNode = 0; outNode < outputNodes.size(); outNode++) {
                weights.at(0).at(inNode) +=
                        learningRate * outputDeltas.at(outNode) * inputNodes.at(inNode);
            }
            //momentum*(weight_input_hidden(inNode, hidNode) - past_weight_input_hidden(inNode, hidNode)) +
        }

    } else {
        // Go through all layers

        // Work from input towards output
        // Input to hidden--loop through each input node
        for (int inNode = 0; inNode < inputNodes.size(); inNode++) {
            // Loop through each hidden node in first hidden layer
            for (int hidNode = 0; hidNode < hiddenNodesPerLayer; hidNode++) {
                weight_input_hidden(inNode, hidNode) +=
                        learningRate * hiddenDeltas.at(0).at(hidNode) * inputNodes.at(inNode);
            }
            //momentum*(weight_input_hidden(inNode, hidNode) - past_weight_input_hidden(inNode, hidNode)) +
        }

        // Hidden to hidden
        // Loop through hidden layers
        for (int i = 1; i < hiddenLayerCount; i++) {
            // Then through nodes in "this" layer
            for (int j = 0; j < hiddenNodesPerLayer; j++) {
                // Then through nodes in "next" layer
                for (int k = 0; k < hiddenNodesPerLayer; k++) {
                    weight_hidden_hidden(i, j, k) +=
                            learningRate * hiddenDeltas.at(i).at(k) * hiddenNodes.at(i - 1).at(j);
                    //momentum*(weight_hidden_hidden(i, j, k) -past_weight_hidden_hidden(i, j, k)) +
                }
            }
        }

        // Hidden to output
        for (int outNode = 0; outNode < outputNodes.size(); outNode++) {
            for (int hidNode = 0; hidNode < hiddenNodesPerLayer; hidNode++) {
                weight_hidden_output(hidNode, outNode) +=
                        learningRate * outputDeltas.at(outNode) * hiddenNodes.back().at(hidNode);
                //momentum*(weight_hidden_output(hidNode, outNode) -past_weight_hidden_output(hidNode, outNode)) +
            }
        }
    }

    //previousWeights = tempWeights;
}

float MultilayerNN::addErrorForIteration(float target) {
    float squaredError = 0;
    // Calculate error for output nodes
    for (int outNode = 0; outNode < outputNodes.size(); outNode++) {
        // Error = target value minus actual value
        squaredError += pow((target - outputNodes.at(outNode)), 2.0);
    }
    return squaredError / (2 * outputNodes.size());
}

float MultilayerNN::activate(float S) {
    return tanh(S);
}

float MultilayerNN::test(vector<vector<float>> testSet) {
    //ofstream dataWriter;
    //ofstream dataWriter2;

    //dataWriter.open("nnOutput.txt", ofstream::out | ofstream::trunc);
    //dataWriter2.open("weights.txt", ofstream::out | ofstream::trunc);

    float mse = 0;

    for (int i = 0; i < testSet.size(); i++) {
        mse += testOne(testSet.at(i));
    }

    mse /= testSet.size();

    return mse;
}

float MultilayerNN::testOne(vector<float> tuple) {
    // Set input nodes to testing tuple values
    for (int i = 0; i < inputNodes.size(); i++) {
        inputNodes.at(i) = tuple.at(i);
    }
    // Run tuple through net
    feedForward();
    // Return squared error
    return addErrorForIteration(tuple.back());
}

void MultilayerNN::reset() {
    topoSet = false;
}