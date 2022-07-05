#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "read.hh"





int main(int argc, char **argv){
    //starting off with reading our files
    if (argc != 3) exit(1);
    const auto compiled = compile_file(argv[1]);

    
}