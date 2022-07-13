#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include "read.hh"
#include "model/model.hh"


static const char source[] =
    "kernel void add(\n"
    "       ulong n,\n"
    "       float charge,"
    "       global const float *a,\n"
    "       global const float *b,\n"
    "       global float *c\n"
    "       )\n"
    "{\n"
    "    size_t i = get_global_id(0);\n"
    "    if (i < n) {\n"
    "       c[i] = tanh(charge * b[i]) + a[i];"
    "    }\n"
    "}\n";



std::vector<float> perform_cl_action(cl::Context &context, cl::CommandQueue &queue,
								  cl::Kernel &add, const float data,
								  std::vector<float> a, std::vector<float> b,
								  const size_t N){
	std::vector<float> c(N);

	// Allocate device buffers and transfer input data to device.
	cl::Buffer A(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		a.size() * sizeof(float), a.data());

	cl::Buffer B(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		b.size() * sizeof(float), b.data());

	cl::Buffer C(context, CL_MEM_READ_WRITE,
		c.size() * sizeof(float));

	// Set kernel parameters.
	add.setArg(0, static_cast<cl_ulong>(N));
    add.setArg(1, static_cast<cl_float>(data));
	add.setArg(2, A);
	add.setArg(3, B);
	add.setArg(4, C);
	
	// Launch kernel on the compute device.
	queue.enqueueNDRangeKernel(add, cl::NullRange, N, cl::NullRange);

	// Get result back to host.
	queue.enqueueReadBuffer(C, CL_TRUE, 0, c.size() * sizeof(float), c.data());

	return c;
}

void Brain::run(cl::Context &context, 
        cl::CommandQueue &queue,
		cl::Kernel &add,
		const size_t cur_index){
	std::vector<float> weights = {};
	std::vector<float> biases = {};

	for (auto conn : neuron_layers[pos][cur_index].connections){
		weights.push_back(conn.weight);
		biases.push_back(conn.bias);
	}

	size_t i = 0;
	for (auto new_charge : perform_cl_action(context, queue, add, neuron_layers[pos][cur_index].charge, biases, weights, neuron_layers[pos][cur_index].connections.size())){
		(neuron_layers[pos][cur_index]).charge += new_charge;
		i++;
	}
}

int main(int argc, char **argv){

    try {
	// Get list of OpenCL platforms.
	std::vector<cl::Platform> platform;
	cl::Platform::get(&platform);

	if (platform.empty()) {
	    std::cerr << "OpenCL platforms not found." << std::endl;
	    return 1;
	}

	cl::Context context;
	std::vector<cl::Device> device;
	for(auto p = platform.begin(); device.empty() && p != platform.end(); p++) {
	    std::vector<cl::Device> pldev;	

	    try {
		p->getDevices(CL_DEVICE_TYPE_GPU, &pldev);

		for(auto d = pldev.begin(); device.empty() && d != pldev.end(); d++) {
		    if (!d->getInfo<CL_DEVICE_AVAILABLE>()) continue;

		    std::string ext = d->getInfo<CL_DEVICE_EXTENSIONS>();

			/*
            //double precision
		    if (
			    ext.find("cl_khr_fp64") == std::string::npos &&
			    ext.find("cl_amd_fp64") == std::string::npos
		       ) continue;
			*/



		    device.push_back(*d);
		    context = cl::Context(device);
			continue;
		}
	    } catch(...) {
		device.clear();
	    }
	}

	if (device.empty()) {
	    std::cerr << "GPUs with double precision not found." << std::endl;
	    return 1;
	}

	// Create command queue.
	cl::CommandQueue queue(context, device[0]);

	// Compile OpenCL program for found device.
	cl::Program program(context, cl::Program::Sources(
		    1, std::make_pair(source, strlen(source))
		    ));

	try {
	    program.build(device);
	} catch (const cl::Error&) {
	    std::cerr
		<< "OpenCL compilation error" << std::endl
		<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device[0])
		<< std::endl;
	    return 1;
	}

	cl::Kernel add(program, "add");
	

	Brain my_brain = compile_brain(compile_file(argv[1]));

	auto out = my_brain.run_network(context, queue, add, {0.5,1});

	std::string output_write = "";

	for (auto item: out)
		output_write += std::to_string(std::exp(item)/(std::exp(item)+1)) + "\n";

	std::ofstream write_out(argv[2]);

	write_out.clear();

	write_out << output_write;

	write_out.clear();
	
    } catch (const cl::Error &err) {
	std::cerr
	    << "OpenCL error: "
	    << err.what() << "(" << err.err() << ")"
	    << std::endl;
	return 1;
    }
}