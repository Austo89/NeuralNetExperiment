cmake_minimum_required(VERSION 3.3)
project(NeuralNetExperiment)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Algorithm.cpp
    Algorithm.h
    DataGenerator.cpp
    DataGenerator.h
    Experiment.cpp
    Experiment.h
    main.cpp
    MultilayerNN.cpp
    MultilayerNN.h
    RBFNN.cpp
    RBFNN.h)

add_executable(NeuralNetExperiment ${SOURCE_FILES})