#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <list>
#include <algorithm>

#include "hyperplane.h"
#include "fi.h"

using namespace std;

int main(int argc, char **argv)
{
	int opt;
	int k, L;	
	int inputFileIndex, queryFileIndex, outputFileIndex;
	int tableSize;

	Hyperplane ** h_array;
	int i,j;

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
	
	ifstream infile(argv[inputFileIndex]);
	string line, coord;
	int lines_counter=0;
	int dimensions=0;

	/*== find out how many lines the file has*/
	while(getline(infile, line))
		++lines_counter;

	tableSize = lines_counter/2;
	
	infile.clear();
	infile.seekg(0, ios::beg);

	/*== find out how many dimensions a point is*/
	getline(infile, line);
	istringstream iss_d(line);
	while(getline(iss_d, coord, ' '))
		dimensions++;

	dimensions -= 1;

	infile.clear();
	infile.seekg(0, ios::beg);

	/*== construct fi*/
	fi * fi_ = new fi(k, dimensions);

	/*== construct hash table*/
	
	
	vector<float> point;
	/*== get each line from file - each line is a vector of size dim*/
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
