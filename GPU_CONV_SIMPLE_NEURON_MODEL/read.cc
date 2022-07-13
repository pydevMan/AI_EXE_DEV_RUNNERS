#include <fstream>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <regex>
#include "read.hh"

std::string read_file(const std::string c_filename){
    std::string cur_line, content = "";

    std::ifstream ifs;

    ifs.open(c_filename);

    if (!ifs.is_open()) exit(1);

    while (ifs){
        content += cur_line + "\n";
        std::getline(ifs, cur_line);
    }

    ifs.close();

    return content;
}


Brain compile_brain(std::vector<std::vector<neuron_compile_struct>> content){

    Brain out(content[0].size(), content[content.size()-1].size());
    

    std::vector<Neuron> to_append;

    size_t i = 0;
    for (auto layer : content){
        std::for_each(layer.begin(), layer.end(),
        [&to_append, &i](neuron_compile_struct cur){
            Neuron pending;
            Synape<Neuron> cur_synapse;
            pending.charge = 0.0;
            pending.order_in_row = i;
            size_t curr = 0;
            for (size_t j = 0; j < cur.synapse_index.size(); j++){
                //error here
                cur_synapse.bias = cur.decimal_constants[curr];
                curr++;
                cur_synapse.weight = cur.decimal_constants[curr];
                cur_synapse.connnection_index_a = pending.order_in_row;
                cur_synapse.connnection_index_b = cur.synapse_index[j];
                curr++;
                pending.connections.push_back(cur_synapse);
                pending.connections[pending.connections.size() -1].to = 
                    std::make_shared<Neuron>(pending);
            }

            to_append.push_back(pending);
            pending.connections.clear();
            i++;
        });

        i = 0;
        out.add_layer(to_append);
        to_append.clear();
    }

    return out;
}


std::vector<std::vector<neuron_compile_struct>>
compile_file(const std::string c_filename){
    std::vector<std::vector<neuron_compile_struct>> out = {};
    std::string content = read_file(c_filename);
    std::vector<std::string> preprossessed = {};
    std::regex ex("\\d+\\.\\d+|\\d+|NL|NN");
    std::smatch my_match;

    auto is_num = [](std::string x){
        switch(x[0]){
            case '0': return true; case '1': return true; case '2': return true; case '3': return true; case '4': return true; 
            case '5': return true; case '6': return true; case '7': return true; case '8': return true; case '9': return true;
            default: return false;
        }
    };

    std::vector<neuron_compile_struct> to_append;

    neuron_compile_struct pending;
    pending.decimal_constants = {};
    pending.synapse_index = {};

    while (std::regex_search(content, my_match, ex)){
        preprossessed.push_back(my_match[0]);
        content = my_match.suffix();
    }

    for(auto i: preprossessed){
        if (is_num(i)){
            if (i.find(".") != std::string::npos)
                pending.decimal_constants.push_back(std::stof(i));
            else
                pending.synapse_index.push_back(std::stoi(i));
        }else if (i == "NN"){
            to_append.push_back(pending);
            pending.decimal_constants = {};
            pending.synapse_index = {};
        }else{
            out.push_back(to_append);
            to_append = {};
        }   
    }

    return out;
}