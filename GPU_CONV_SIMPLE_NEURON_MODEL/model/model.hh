#pragma once
#include <memory>
#include <vector>
#include <stdlib.h>
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

template <typename T>
// a conection from and too a other object
struct Synape{
    std::shared_ptr<T> from, to;
    float weight, bias;
    size_t connnection_index_a, connnection_index_b;
};

//a neuron :: the basis of logic in our model
struct Neuron{
    float charge;
    std::vector<Synape<Neuron>> connections;
    size_t order_in_row;
};

//the brain that defines our logic
class Brain{
    public:
    std::vector<std::vector<Neuron>> neuron_layers;
    size_t inputs, outputs;
    size_t pos;


    Brain(const size_t c_inputs, const size_t c_outputs);

    //add a layer to the brain
    void add_layer(std::vector<Neuron> new_layer);

    //connect u[ the layers
    void fix_layers(const size_t c_layer_index);

    //get the outputs of the network
    std::vector<float> run_network(
        cl::Context &context, 
        cl::CommandQueue &queue,
		cl::Kernel &add,const std::vector<float> c_inputs);

    //run a single neuron in the network at layer positon pos
    void run(cl::Context &context, 
             cl::CommandQueue &queue,
			cl::Kernel &add,
            const size_t cur_index);
};





// make a new neuron connection
Synape<Neuron>
New_NeuronSynape(const Neuron * c_neuron_b,
                 const float c_weight, const float c_bias,
                 const size_t c_connection_index_b);

// make a new neuron
Neuron 
New_Neuron(const std::vector<Synape<Neuron>> c_connections,
           const float c_charge, const size_t order_in_row);