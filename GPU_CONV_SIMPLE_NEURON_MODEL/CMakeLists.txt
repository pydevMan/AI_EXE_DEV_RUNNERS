cmake_minimum_required(VERSION 3.7)

project(GPU_CONV VERSION 0.0.1 LANGUAGES CXX)

add_executable(AI_EXE_CMAKE_CONV_RUNNER_CL main read model/model)
target_compile_features(AI_EXE_CMAKE_CONV_RUNNER_CL PRIVATE cxx_auto_type)

find_package(OpenCL REQUIRED)
target_link_libraries(AI_EXE_CMAKE_CONV_RUNNER_CL OpenCL::OpenCL)

install(TARGETS AI_EXE_CMAKE_CONV_RUNNER_CL DESTINATION bin
        #DESTINATION ${CMAKE_INSTALL_DOCDIR}/AI_EXE_CMAKE_CONV_RUNNER_CL
)