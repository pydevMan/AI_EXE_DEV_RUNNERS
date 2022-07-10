#pragma once
#include <string>
#include <vector>
#include <stdlib.h>
#include "model/model.hh"

struct neuron_compile_struct{
    std::vector<float> decimal_constants;
    std::vector<size_t> synapse_index;
};

//read a file 
std::string 
read_file(const std::string c_filename);

//compile a file
std::vector<std::vector<neuron_compile_struct>>
compile_file(const std::string c_filename);


Brain compile_brain(std::vector<std::vector<neuron_compile_struct>> content);