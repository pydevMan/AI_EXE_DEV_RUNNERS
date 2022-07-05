#include <algorithm>
#include "model.hh"

// make a new neuron connection
Synape<Neuron>
New_NeuronSynape(const Neuron c_neuron_a, const Neuron c_neuron_b,
                 const float c_weight, const float c_bias,
                 const size_t c_connection_index_a, 
                 const size_t c_connection_index_b)
{
    Synape<Neuron> out;

    out.connnection_index_a = c_connection_index_a;
    out.connnection_index_b = c_connection_index_b;

    out.weight = c_weight;
    out.bias = c_bias;

    out.from = std::make_shared<Neuron>(c_neuron_a);
    out.to = std::make_shared<Neuron>(c_neuron_b);

    return out;
}

// make a new neuron
Neuron 
New_Neuron(const std::vector<Synape<Neuron>> c_connections,
           const float c_charge, const size_t c_order_in_row)
{
    Neuron out;

    out.connections = c_connections;

    out.order_in_row = c_order_in_row;
    out.charge = c_charge;

    return out;
}


Brain::Brain(const size_t c_inputs, const size_t c_outputs){
    pos = 0;
    inputs = c_inputs;
    outputs = c_outputs;
}

void Brain::add_layer(std::vector<Neuron> new_layer){
    neuron_layers.push_back(new_layer);
    if (neuron_layers.size() > 1) this->fix_layers(neuron_layers.size()-2);
}

void Brain::fix_layers(const size_t c_layer_index){
    std::for_each(
    neuron_layers[c_layer_index].begin(),
    neuron_layers[c_layer_index].end(),
    [&, this, c_layer_index](Neuron &cur)
    {
        for (size_t i = 0; i < cur.connections.size(); i++){
            cur.connections[i].to = 
            std::make_shared<Neuron>(
                neuron_layers[c_layer_index-1]
                [cur.connections[i].connnection_index_a]);
        }
    });
}