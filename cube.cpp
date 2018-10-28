#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <list>
#include <algorithm>
#include <cstring>
#include <math.h>
#include <getopt.h>

#include "help_functions.h"
#include "hyper_cube.h"

#define C 1

using namespace std;

int main(int argc, char **argv)
{
	int opt;
	int k, M, probes;	
	int inputFileIndex, queryFileIndex, outputFileIndex;

	/*== struct for getopt long options*/
	static struct option long_options[] = 
	{
		{"d"	 , required_argument, NULL, 'd'},
		{"q"	 , required_argument, NULL, 'q'},
		{"k"	 , required_argument, NULL, 'k'},
		{"M"	 , required_argument, NULL, 'M'},
		{"o"	 , required_argument, NULL, 'o'},
		{"probes", required_argument, NULL, 'p'}
	};

	/*== get all input arguments through getopt()*/
	while ((opt = getopt_long_only(argc, argv, "", long_options, NULL)) != -1)
	{
        switch (opt)
		{
			case 'd':
				inputFileIndex = optind-1;
				break;
			case 'q':
				queryFileIndex = optind-1;
				break;
			/*== dimensions (d')*/
			case 'k':
				k = atoi(optarg);
				break;
			/*== max number of candidate points examined*/
			case 'M':
				M = atoi(optarg);
				break;
			case 'o':
				outputFileIndex = optind-1;
				break;
			/*== max number of buckets searched*/
			case 'p':
				probes = atoi(optarg);
				break;
			default: 
				fprintf(stderr, "Usage: $./cube –d <input file> –q <query file> –k <int> -M <int> -probes <int> -ο <output file>\n");
				exit(EXIT_FAILURE);
		}
    }

	/*== check if user entered all arguments*/
	if( argc != 13 )
	{
		fprintf(stderr, "Usage: $./cube –d <input file> –q <query file> –k <int> -M <int> -probes <int> -ο <output file>\n");
		exit(EXIT_FAILURE);
	}

	ifstream infile(argv[inputFileIndex]);
	string line, coord;

	/*== find out how many dimensions a point is*/
	int dimensions = help_functions::calculate_dimensions(infile);

	/*== find out if we want euclidean or cosine*/
	string type = help_functions::find_type(infile);

	/*== calculate tableSize*/
	int tableSize = pow(2, k);

	/*== constuct hypercube*/
	HyperCube<vector<double>> * hyper_cubeptr;
	if(type == "EUC")
		hyper_cubeptr = new HyperCube_EUC<vector<double>>(tableSize, k, dimensions);
	else
		hyper_cubeptr = new HyperCube_COS<vector<double>>(tableSize, k, dimensions);

	/*== get each line from input file - each line is a vector of size dim*/
	vector<double> point;
	string identifier;
	if( type == "COS" )
		getline(infile, line);

	while(getline(infile, line))
	{
		istringstream iss(line);
		/*== split the line into 128 coords which we save into a vector of size 128*/
		getline(iss, identifier, ' ');
		while(getline(iss, coord, ' '))
		{
			try {
				point.push_back(stod(coord));
			}
			catch(const std::invalid_argument& ia){
				continue;
			}
		}
		
		/*== hash the vector point to our hypercube*/
		hyper_cubeptr->put(point, identifier);

		/*== clear vector for next iteration*/
		point.clear();
	}

	/*== start searching query points*/
	ifstream queryfile(argv[queryFileIndex]);
	ofstream outputfile(argv[outputFileIndex]);

	/*== first line of the query file contains the Radius*/
	getline(queryfile, line);
	
	double R;
	std::istringstream Rstream(line);
	Rstream >> line >> R;

	while(getline(queryfile, line))
	{
		istringstream iss(line);
		/*== split the line into 128 coords which we save into a vector of size 128*/
		getline(iss, identifier, ' ');
		while(getline(iss, coord, ' '))
		{
			try {
				point.push_back(stod(coord));
			}
			catch(const std::invalid_argument& ia){
				continue;
			}
		}
		
		/*== print id to file*/
		outputfile << "Query: " << identifier << endl;

		/*== Range Search*/
		outputfile << "R-near neighbours:" << endl;
		hyper_cubeptr->RS(point, outputfile, C, R, probes, M);

		/*== Approximate Nearest Neighbour*/
		outputfile << "LSH Neighbour: ";
		hyper_cubeptr->ANN(point, outputfile, probes, M);

		/*== Nearest Neighbour*/
		outputfile << "Nearest neighbour: ";
		hyper_cubeptr->NN(point, outputfile);

		outputfile << endl;

		/*== clear vector for next iteration*/
		point.clear();
	}

	exit(EXIT_SUCCESS);
}
