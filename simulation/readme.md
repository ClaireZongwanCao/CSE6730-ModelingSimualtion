CSE 6730 first check point submission
=======================

Compiling
======

Just run the make file. It should produce two executables: generate_input and simulation. 

Executing
======

generate_input is used to generate a test file for the simulation. It takes as input a configuration file. A self explanatory configuration file is provided (config.txt). The file produced by this executable is fed to the executable "simulation". It will write the results to standart out in the following format: 

car_id	car_src	car_dest	entry_time	exit_time

A general note: the code crashes for some long test cases, left undebugged due to lack of time. 