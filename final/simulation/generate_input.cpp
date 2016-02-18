#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <stdint.h>

#include "util.h"

void PrintUsage() {
	std::cerr << "Usage: generate_input <config_file> <data_file>" << std::endl;
}

// num of params in the struct Param, to follow
unsigned int num_params = 8;

typedef struct {
	unsigned int sync_flag;
	unsigned int num_cars;
	unsigned int num_time_units;
	unsigned int green_time;
	unsigned int left_green_time;
//	unsigned int red_time;
	unsigned int intersect_delay;
	unsigned int diff21;
	unsigned int diff32;
}Params;

void getParams(std::ifstream&, Params *);

int main(int argc, char **argv) {
	
	if (argc!=3) {
		PrintUsage();
		exit(0);
	}
	
	//open config file
	std::ifstream infile;
	infile.open(argv[1]);
	if (!infile.is_open()) {
		std::cerr << "Unable to open config file" << std::endl;
		exit(0);
	}	
	
	// open output file and check for errors
	std::ofstream outfile;
	outfile.open(argv[2]);
	if (!outfile.is_open()) {
		std::cerr << "Unable to open data file" << std::endl;
		exit(0);
	}
	
	Params *P = new Params;
	getParams(infile, P);
	
	srand(time(NULL));

	// prepare the data file
	outfile << P->sync_flag << std::endl;
	outfile << P->num_cars << std::endl;
	outfile << P->num_time_units << std::endl;
	
	// changing signal diffs if unsync
	if (P->sync_flag==0) {
		P->diff21 = rand_number(0, (P->green_time + P->left_green_time)*2);
		P->diff32 = rand_number(0, (P->green_time + P->left_green_time)*2);
	}
	
	// starting times of all the cars
	std::vector<unsigned int> times;
	for (unsigned int i=0; i < P->num_cars; i++) {
		times.push_back(rand_number(0,P->num_time_units));
	}
	
	std::sort(times.begin(), times.end());
	
	unsigned int s, d;
	
	// randomly select source and destination for each car, and put it in data file
	// you have to make sure that s!=d
	for (unsigned int i=0; i < P->num_cars; i++) {
		s = rand_number(0,8);
		if (s==1 or s==2) {
			s+=2;
		}
		
		// generate a d, which is not equal to s
		do {
			d = rand_number(0,8);
			if (d==1 or d==2) {
				d+=2;
			}
		}while(s==d);
		
		outfile << i << "\t" << times[i] << "\t" << s << "\t" << d << std::endl;
		
	}
	
	outfile << P->green_time << std::endl;
	outfile << P->left_green_time << std::endl;
//	outfile << P->red_time << std::endl;
	outfile << P->intersect_delay << std::endl;
	outfile << P->diff21 << std::endl;
	outfile << P->diff32 << std::endl;

	return 0;
}

void getParams(std::ifstream& is, Params* P)
{
	// start reading input file;
	std::string line;
	std::vector<unsigned int> p;
	while(is.good()) {
		std::getline(is, line);
		if (line[0]=='#') {
			continue;
		}
		p.push_back(atoi(line.c_str()));
	}
	
	if (p.size() < num_params) {
		std::cerr << "Not enough values in config file" << std::endl;
		exit(0);
	}
	
	unsigned int i=0;
	P->sync_flag = p[i++];
	P->num_cars = p[i++];
	P->num_time_units = p[i++];
	P->green_time = p[i++];
	P->left_green_time = p[i++];
//	P->red_time = p[4];
	P->intersect_delay = p[i++];
	P->diff21 = p[i++];
	P->diff32 = p[i++];
}
