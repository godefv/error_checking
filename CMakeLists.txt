cmake_minimum_required (VERSION 3.0)
project(godefv_error_checking)

include(godefv.cmake)

godefv_add_module(error_checking 
	exit.cpp
	system.cpp
	return_status.cpp       
)
target_compile_features(godefv_error_checking INTERFACE cxx_std_14)

enable_testing()
godefv_add_test(error_checking return_status)


