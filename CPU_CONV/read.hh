#pragma once
#include <string>
#include <vector>
#include <stdlib.h>
#include <tuple>

struct neuron_compile_struct{
    std::vector<float> decimal_constants;
    std::vector<size_t> synapse_index;
};

//read a file 
std::string 
read_file(const std::string c_filename);

std::vector<std::string> split (std::string s, std::string delimiter);

//compile a file
std::vector<std::vector<neuron_compile_struct>>
compile_file(const std::string c_filename);