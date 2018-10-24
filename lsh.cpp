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

#include "hash_table.h"

using namespace std;

int main(int argc, char **argv)
{
	int opt;
	int k, L;	
	int inputFileIndex, queryFileIndex, outputFileIndex;
	int tableSize;

	int i,j;
	HashTable<vector<double>, string> * hash_table;

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

	/*== construct hash table*/
	hash_table = new HashTable<vector<double>, string>(tableSize, k, dimensions);
	
	/*== get each line from file - each line is a vector of size dim*/
	vector<double> point;
	while(getline(infile, line))
	{
		istringstream iss(line);
		/*== split the line into 128 coords which we save into a vector of size 128*/
		while(getline(iss, coord, ' '))
		{
			try {
				point.push_back(stod(coord));
			}
			catch(const std::invalid_argument& ia){
				continue;
			}
		}
		
		/*== hash the vector point to our hash table*/
		hash_table->put(point);

		/*== clear vector for next iteration*/
		point.clear();
	}
	
	exit(EXIT_SUCCESS);
}
