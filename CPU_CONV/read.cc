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


std::vector<std::string> split (std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
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