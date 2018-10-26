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

#include "hash_table.h"

using namespace std;

int main(int argc, char **argv)
{
	int opt;
	int k, L;	
	int inputFileIndex, queryFileIndex, outputFileIndex;
	int tableSize;

	int i;
	HashTable<vector<double>> * hash_tableptr;


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
	string line, type, coord;
	int dimensions=0;


	/*== find out if we want euclidean or cosine and assign pointer to the proper hash_table*/
	type = help_functions::find_type(infile);

	/*== find out how many lines the file has(used for euclidean)*/
	tableSize = help_functions::calculate_tableSize(infile, type, k);

	/*== find out how many dimensions a point is*/
	dimensions = help_functions::calculate_dimensions(infile);

	/*== create hash table based on type and assign a pointer*/
	if( type == "COS" )
	{
		HashTable_COS<vector<double>> * hash_table = new HashTable_COS<vector<double>>(tableSize, k, dimensions);
		hash_tableptr = hash_table;
	}
	else
	{
		HashTable_EUC<vector<double>> * hash_table = new HashTable_EUC<vector<double>>(tableSize, k, dimensions);
		hash_tableptr = hash_table;
	}

	infile.clear();
	infile.seekg(0, ios::beg);
	

	/*== get each line from file - each line is a vector of size dim*/
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
		

		/*== hash the vector point to our hash table*/
		hash_tableptr->put(point, identifier);

		/*== clear vector for next iteration*/
		point.clear();
	}
	

	/*== get each vector from query file*/
	ifstream queryfile(argv[queryFileIndex]);
	ofstream outputfile(argv[outputFileIndex]);
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
		hash_tableptr->RS(point, outputfile, 1, 200);

		/*== Real Range Search*/

		/*== Approximate Nearest Neighbour*/
		outputfile << "LSH Neighbour: ";
		hash_tableptr->ANN(point, outputfile);

		/*== Nearest Neighbour*/
		outputfile << "Nearest neighbour: ";
		hash_tableptr->NN(point, outputfile);


		outputfile << endl << endl;

		/*== clear vector for next iteration*/
		point.clear();
	}

	exit(EXIT_SUCCESS);
}
