/* 
 * File:   RBFNN.cpp
 * Author: Austo89
 * 
 * Created on September 28, 2015, 7:18 AM
 */

#include <stdlib.h>
#include <ctime>
#include <float.h>
#include "RBFNN.h"

using namespace std;

RBFNN::RBFNN(int in_K, int in_inputSize) {
    K = in_K;
    gamma = 1;
    inputSize = in_inputSize;
}

RBFNN::RBFNN(const RBFNN& orig) {
}

RBFNN::~RBFNN() {
}

void RBFNN::trainNetwork(vector<vector<double>> data){
    //find K means - using Lloyd's algorithm
    //pick random mu values
    srand(time(NULL));

    for(int j = 0; j < K; j++){
        vector<double> temp_vec;

        for(int i = 0; i < inputSize; i++){
            double temp = ((double) rand() / (double) RAND_MAX) * 10;
            temp_vec.push_back(temp);
        }
        mus.push_back(temp_vec);
    }


    //find initial clustering
    double prev_error = findClustering(data);
    double delta_error = DBL_MAX - prev_error;

    //loop until we find optimal mu values
    while(delta_error > .0001){
        findMus(data);

        double new_error = findClustering(data);
        delta_error = prev_error - new_error;
        prev_error = new_error;
    }

    //now we just need to find the weights
}

double RBFNN::findClustering(vector<vector<double>> data){
    //clear old clusters
    vector<vector<int>> refresh;
    clusters = refresh;

    for(int j = 0; j < K; j++){
        vector<int> set_vec;
        clusters.push_back(set_vec);
    }

    //associate data points with the mu values
    //look at every data point
    double error_sum = 0;
    for (int i = 0; i < data.size(); i++){
        //compare it to every mu, to find the cluster it belongs to
        int index = 0;
        double min_dist = DBL_MAX;
        for (int j = 0; j < K; j++){
            //find distance from mu
            double dist = DBL_MAX;
            double d_sum = 0;
            for(int k = 0; k < inputSize; k++){
                d_sum += (mus[j][k] - data[j][k]) * (mus[j][k] - data[j][k]);
            }
            dist = sqrt(d_sum);

            //check if this mu is the best so far
            //update index and minimum distance accordingly
            if (dist < min_dist){
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

void RBFNN::findMus(vector<vector<double>> data){
    //clear old Mus
    vector<vector<double>> refresh;
    mus = refresh;

    for(int j = 0; j < K; j++){
        vector<double> set_vec;
        mus.push_back(set_vec);
    }

    //calculate new cluster mus
    //there are K clusters, index j
    for(int j = 0; j < K; j++){
        //vector of dimensions for mu
        vector<double> temp_vec;

        //calculate the average of each dimension, i-th dimension over cluster x's
        for(int i = 0; i < inputSize; i++){
            //dimension total
            double xi_sum = 0;
            //grab each xi from data point in clusters
            for(int k = 0; k < clusters[j].size(); k++){
                //cluster j, index k in cluster, for dimension xi
                xi_sum = data[clusters[j][k]][i];
            }

            //push back the average of this dimension
            temp_vec.push_back(xi_sum / (double)clusters[j].size());
        }
        mus.push_back(temp_vec);
    }
}

double RBFNN::functionApproximation(){
    
}

