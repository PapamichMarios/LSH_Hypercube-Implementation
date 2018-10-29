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
	vector<double> point(dimensions);
	int j=0;

	string identifier;
	if( type == "COS" )
		getline(infile, line);

	while(getline(infile, line))
	{
		istringstream iss(line);
		j=0;
		/*== split the line into 128 coords which we save into a vector of size 128*/
		getline(iss, identifier, ' ');
		while(getline(iss, coord, ' '))
		{
			if(j<dimensions)
				point[j] = stod(coord);

			j++;
		}
		
		/*== hash the vector point to our hypercube*/
		hyper_cubeptr->put(point, identifier);
	}

	/*== start searching query points*/
	ifstream queryfile(argv[queryFileIndex]);
	ofstream outputfile(argv[outputFileIndex]);

	double distance_NN, distance_ANN, time_ANN;
	double approaching_factor=0;
	double average_time_ANN=0;

	/*== save how many queries we have to search*/
	int querySize = help_functions::count_lines_query(queryfile, type);

	/*== first line of the query file contains the Radius*/
	getline(queryfile, line);
	
	double R;
	std::istringstream Rstream(line);
	Rstream >> line >> R;

	while(getline(queryfile, line))
	{
		istringstream iss(line);
		j=0;
		/*== split the line into 128 coords which we save into a vector of size 128*/
		getline(iss, identifier, ' ');
		while(getline(iss, coord, ' '))
		{
			if(j<dimensions)
				point[j] = stod(coord);

			j++;
		}
		
		/*== print id to file*/
		outputfile << "Query: " << identifier << endl;

		/*== Range Search*/
		outputfile << "R-near neighbours:" << endl;
		hyper_cubeptr->RS(point, outputfile, C, R, probes, M);

		/*== Approximate Nearest Neighbour*/
		outputfile << "LSH Neighbour: ";
		hyper_cubeptr->ANN(point, outputfile, probes, M, distance_ANN, time_ANN);

		/*== Nearest Neighbour*/
		outputfile << "Nearest neighbour: ";
		hyper_cubeptr->NN(point, outputfile, distance_NN);

		outputfile << endl;

		if( distance_ANN/distance_NN > approaching_factor )
			approaching_factor = distance_ANN/distance_NN;

		average_time_ANN += time_ANN;

		/*== reset for next loop*/
		time_ANN = 0;
		distance_ANN = 0;
		distance_NN = 0;
	}

	/*== print average time && approaching factor*/
	average_time_ANN /= querySize;

	outputfile << "Approaching factor: " << approaching_factor << endl;
	outputfile << "Average time ANN: " << average_time_ANN << endl;

	/*== free hypercube*/
	delete hyper_cubeptr;
	hyper_cubeptr = NULL;

	/*== close files*/
	infile.close();
	queryfile.close();
	outputfile.close();
	
	exit(EXIT_SUCCESS);
}
