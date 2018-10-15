#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "hyperplane.h"

using namespace std;

int main(int argc, char **argv)
{
	int opt;
	int k, L;	
	int inputFileIndex, queryFileIndex, outputFileIndex;

	Hyperplane ** h_array;

	/*== get all input arguments through getopt()*/
	while ((opt = getopt(argc, argv, "d:q:k:L:o:")) != -1)
	{
        switch (opt)
		{
			case 'd':
				inputFileIndex = optind-1;
				break;
			case 'q':
				queryFileIndex = optind-1;
				break;
			case 'k':
				k = atoi(optarg);
				break;
			case 'L':
				L = atoi(optarg);
				break;
			case 'o':
				outputFileIndex = optind-1;
				break;
			default: 
				fprintf(stderr, "Usage: ./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file>\n");
				exit(EXIT_FAILURE);
		}
    }

	/*== check if user entered all arguments*/
	if( argc != 11)
	{
		fprintf(stderr, "Usage: ./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file>\n");
		exit(EXIT_FAILURE);
	}
	
	/*== making test hyperplane*/
	//Hyperplane(128);	

	/*== generating an array of hyperplanes to construct fi*/

	/*== start reading input file*/
	ifstream infile(argv[inputFileIndex]);
	string line, coord;
	
	vector<float> point;

	/*== get each line from file - each line is a vector of size 128*/
	while(getline(infile, line))
	{
		istringstream iss(line);
		/*== split the line into 128 coords which we save into a vector of size 128*/
		while(getline(iss, coord, ' '))
		{
			try {
				point.push_back(stof(coord));
			}
			catch(const std::invalid_argument& ia){
				continue;
			}
		}

		//iterating through vector
		//for(vector<float>::const_iterator it=point.begin(); it!=point.end(); ++it)
		//	cout << *it << endl;

		
	}
	
	exit(EXIT_SUCCESS);
}
