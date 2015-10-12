/* 
 * File:   RBFNN.cpp
 * Author: Austo89
 * 
 * Created on September 28, 2015, 7:18 AM
 */

#include <stdlib.h>
#include <ctime>
#include <float.h>
#include <tgmath.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "RBFNN.h"

using namespace std;

RBFNN::RBFNN(int in_K, int in_inputSize) {
    //parameter initilization
    K = in_K;
    eta = 0.02;
    gamma = 1;
    inputSize = in_inputSize;

    //randomize weights
    for (int j = 0; j < K; j++) {
        double temp = ((double) rand() / (double) RAND_MAX) * 10;
        weights.push_back(temp);
    }
}

RBFNN::RBFNN(const RBFNN &orig) {
}

RBFNN::~RBFNN() {
}

///////////////////////////////////////////
// trainNetwork:
//    Train the network given a set of passed in data points and
//    expected values
///////////////////////////////////////////

void RBFNN::trainNetwork(vector<vector<double>> data, vector<double> ys) {
    //find K means - using Lloyd's algorithm
    //pick random mu values
    
    //reset new random seed based on system clock
    srand(time(NULL));

    for (int j = 0; j < K; j++) {
        int start_point = rand() % data.size();
        vector<double> temp_mu;
        for (int i = 0; i < inputSize; i++) {
            temp_mu.push_back(data[start_point][i]);
        }
        mus.push_back(temp_mu);
    }

    //find gamma, 1 / (2 * variance)
    //find mean
    vector<double> mean;
    for (int i = 0; i < inputSize; i++) {
        mean.push_back(0);
    }

    for (int i = 0; i < inputSize; i++) {
        for (int j = 0; j < data.size(); j++) {
            mean[i] += data[j][i];
        }
    }

    for (int i = 0; i < inputSize; i++) {
        mean[i] = mean[i] / (double) data.size();
    }

    //find variance
    double variance = 0;
    for (int i = 0; i < data.size(); i++) {
        double d_sum = 0;
        for (int j = 0; j < inputSize; j++) {
            d_sum += (mean[j] - data[i][j]) * (mean[j] - data[i][j]);
        }
        double dist = sqrt(d_sum);
        variance += dist * dist;
    }
    variance = variance / data.size();
    gamma = 1 / (2 * variance);

    //find initial clustering
    double prev_error = findClustering(data);
    double delta_error = DBL_MAX - prev_error;

    //loop until we find optimal mu values
    while (delta_error > .0001) {
        findMus(data);

        double new_error = findClustering(data);
        delta_error = abs(prev_error - new_error);
        prev_error = new_error;
    }

    //now we just need to find the weights

    //calculate Phi matrix
    vector<vector<double>> phi_matrix;
    for (int i = 0; i < data.size(); i++) {
        vector<double> temp_vec;
        for (int k = 0; k < K; k++) {
            temp_vec.push_back(phi(data[i], mus[k]));
        }
        phi_matrix.push_back(temp_vec);
    }


    //do 3 random restarts to minimize the error
    for (int i = 0; i < 3; i++){

        //iterate over all the data points and minimize the error for each
        for (int j = 0; j < data.size(); j++){
            double d_err = DBL_MAX;
            double last_err = DBL_MAX;
            while (d_err > .001) {
                double current_err = adaline(data, phi_matrix, ys,j);
                d_err = abs(last_err - current_err);
                last_err = current_err;
            }
        }
    }

}


//////////////////////////////////////////////////////////////
// findClustering:
//   Given a set of Mu values, find a new set of cluster for the data
//////////////////////////////////////////////////////////////

double RBFNN::findClustering(vector<vector<double>> data) {
    //clear old clusters
    vector<vector<int>> refresh;
    clusters = refresh;

    for (int j = 0; j < K; j++) {
        vector<int> set_vec;
        clusters.push_back(set_vec);
    }

    //associate data points with the mu values
    //look at every data point
    double error_sum = 0;
    for (int i = 0; i < data.size(); i++) {
        //compare it to every mu, to find the cluster it belongs to
        int index = 0;
        double min_dist = DBL_MAX;
        for (int j = 0; j < K; j++) {
            //find distance from mu
            double dist = DBL_MAX;
            double d_sum = 0;
            for (int k = 0; k < inputSize; k++) {
                d_sum += (mus[j][k] - data[i][k]) * (mus[j][k] - data[i][k]);
            }
            dist = sqrt(d_sum);

            //check if this mu is the best so far
            //update index and minimum distance accordingly
            if (dist < min_dist) {
                min_dist = dist;
                index = j;
            }
        }

        // add the data point to its cluster
        clusters[index].push_back(i);
        error_sum += min_dist * min_dist;
    }
    return error_sum;
}

///////////////////////////////////////////
// findMus:
//    Find the average points of the current clusters
///////////////////////////////////////////

void RBFNN::findMus(vector<vector<double>> data) {
    //clear old Mus
    vector<vector<double>> refresh;
    mus = refresh;

    for (int j = 0; j < K; j++) {
        vector<double> set_vec;
        mus.push_back(set_vec);
    }

    //calculate new cluster mus
    //there are K clusters, index j
    for (int j = 0; j < K; j++) {
        //vector of dimensions for mu
        vector<double> temp_vec;

        //calculate the average of each dimension, i-th dimension over cluster x's
        for (int i = 0; i < inputSize; i++) {
            //dimension total
            double xi_sum = 0;
            //grab each xi from data point in clusters
            for (int k = 0; k < clusters[j].size(); k++) {
                //cluster j, index k in cluster, for dimension xi
                xi_sum += data[clusters[j][k]][i];
            }

            //push back the average of this dimension
            temp_vec.push_back(xi_sum / (double) clusters[j].size());
        }

        mus[j] = temp_vec;
    }
}

///////////////////////////////////////////
// adaline:
//    Take the next weight update step in the adaline algorithm
///////////////////////////////////////////

double RBFNN::adaline(vector<vector<double>> data, vector<vector<double>> phi_matrix, vector<double> ys, int index) {
    //initialize
    vector<double> newWeights;
    double err_diff = 0;

    //calculate the difference in y and scale by the learning rate
    double random = runModel(data[index]);
    random = ys[index] - random;
    err_diff = random * random;
    random = random * eta;

    // multiply by the derivative
    for (int i = 0; i < K; i++) {
        double what = phi_matrix[index][i] * random;
        newWeights.push_back(what);
    }

    // add the change in weights to the current weights
    for (int i = 0; i < K; i++) {
        weights[i] += newWeights[i];
    }
    
    //return the new error rate
    return err_diff;
}


///////////////////////////////////////////
// runModel:
//    Run an input through the model to estimate an output.
//    Output is returned as a double.
///////////////////////////////////////////

double RBFNN::runModel(vector<double> input) {
    double output = 0;
    for (int i = 0; i < K; i++) {
        double temp = phi(input, mus[i]) * weights[i];
        output += temp;
    }

    return output;
}

///////////////////////////////////////////
// phi:
//    Calculate the gaussian function given an x vector
//    and a mean
///////////////////////////////////////////

double RBFNN::phi(vector<double> x, vector<double> mu) {
    double d_sum = 0;
    for (int j = 0; j < inputSize; j++) {
        if (isnan(mu[j])) {
            //do nothing
        } else {
            d_sum += (mu[j] - x[j]) * (mu[j] - x[j]);
        }

    }
    double dist = sqrt(d_sum);
    double temp = exp(dist * dist * (-1 * gamma));

    return temp;
}

///////////////////////////////////////////
// runTestData:
//    Take a set of inputs and outputs and return the square mean error
//    for the model outputs compared to the expected outputs
///////////////////////////////////////////

double RBFNN::runTestData(vector<vector<double>> xs, vector<double> ys) {
    //run all test points and find the mean squared error
    double err_sum = 0;
    for (int i = 0; i < xs.size(); i++) {
        double obs_y = runModel(xs[i]);
        double err = ys[i] - obs_y;

        //square the errors
        err = err * err;
        err_sum += err;
    }

    //average the sum of square errors
    err_sum = err_sum / (double) ys.size();
    return err_sum;
}