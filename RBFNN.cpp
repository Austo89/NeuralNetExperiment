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
#include "RBFNN.h"

using namespace std;

RBFNN::RBFNN(int in_K, int in_inputSize) {
    K = in_K;
    eta = 0.002;
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





void RBFNN::trainNetwork(vector<vector<double>> data, vector<double> ys) {
    //find K means - using Lloyd's algorithm
    //pick random mu values
    srand(time(NULL));

    for (int j = 0; j < K; j++) {
        int start_point = rand() % data.size();
        vector<double> temp_mu;
        for (int i = 0; i < inputSize; i++){
            temp_mu.push_back(data[start_point][i]);
        }
        mus.push_back(temp_mu);
    }

    //find gamma, 1 / (2 * variance)
    //find mean
    vector<double> mean;
    for (int i = 0; i < inputSize; i++){
        mean.push_back(0);
    }

    for (int i = 0; i < inputSize; i++){
        for(int j = 0; j < data.size(); j++){
            mean[i] += data[j][i];
        }
    }

    for (int i = 0; i < inputSize; i++){
        mean[i] = mean[i] / (double)data.size();
    }

    //find variance
    double variance = 0;
    for (int i = 0; i < data.size(); i++){
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
    for (int i = 0; i < data.size();i++){
        vector<double> temp_vec;
        for (int k = 0; k < K; k++){
            temp_vec.push_back(phi(data[i],mus[k]));
        }
        phi_matrix.push_back(temp_vec);
    }

    double d_err = DBL_MAX;
    double last_err = DBL_MAX;
//    while (d_err > .001){
//        double current_err = adeline(data,phi_matrix,ys);
//        d_err = abs(last_err - current_err);
//        last_err = current_err;
//    }

    adeline(data,phi_matrix,ys);
}


//////////////////////////////////////////////////////////////
// Given a set of Mu values, find a new set of cluster for the data
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




double RBFNN::adeline(vector<vector<double>> data, vector<vector<double>> phi_matrix, vector<double> ys) {
    vector<double> newWeights;
    vector<double> modelOutput;
    double err_diff = 0;
    double d_err = DBL_MAX;
    double last_err;

    while (d_err > .001){
        double random = runModel(data[3]);
        random = ys[3] - random;
        err_diff = random * random;
        random = random * eta;

        for (int i = 0; i < K; i++){
            double what = phi(data[3],mus[i]) * random;
            newWeights.push_back(what);
        }

        for (int i = 0; i < K; i++){
            weights[i] += newWeights[i];
        }

        d_err = abs(last_err - err_diff);
        last_err = err_diff;
    }




//    //find the change in difference
//    double err_diff = 0;
//
//    //calculate current model outputs
//    for (int i = 0; i < data.size(); i++){
//        double observed = runModel(data[i]);
//        modelOutput.push_back(observed);
//        double diff = (ys[i] - modelOutput[i]);
//        err_diff += diff * diff;
//        modelOutput[i] = eta * diff;
//    }
//
//    err_diff = err_diff / (double)data.size();
//
//    for(int k = 0; k < K; k++){
//        double dot_product = 0;
//        for (int i = 0; i < data.size(); i++){
//            double temp = phi(data[i],mus[k]) * modelOutput[i];
//            dot_product += temp;
//        }
//        newWeights.push_back(dot_product);
//
//        //update the weights
//        weights[k] = weights[k] + newWeights[k];
//    }

    return err_diff;
}

double RBFNN::runModel(vector<double> input) {
    double output = 0;
    for (int i = 0; i < K; i++){
        double temp =  phi(input, mus[i]) * weights[i];
        output += temp;
    }

    return output;
}

double RBFNN::phi(vector<double> x, vector<double> mu){
    double d_sum = 0;
    for (int j = 0; j < inputSize; j++) {
        d_sum += (mu[j] - x[j]) * (mu[j] - x[j]);
    }
    double dist = sqrt(d_sum);
    double temp = exp(dist * dist * (-1 * gamma));

    return temp;
}
