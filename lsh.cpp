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

	int i,j;
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
	int lines_counter=0;
	int dimensions=0;


	/*== find out if we want euclidean or cosine and assign pointer to the proper hash_table*/
	getline(infile, line);
	if( line.compare(0, 6, "cosine") == 0 )
		type = "COS";
	else
		type = "EUC";

	infile.clear();
	infile.seekg(0, ios::beg);

	/*== find out how many lines the file has(used for euclidean)*/
	if( type == "EUC" )
	{
		while(getline(infile, line))
			++lines_counter;

		tableSize = lines_counter/2;
		
		infile.clear();
		infile.seekg(0, ios::beg);
	}
	else
		tableSize = pow(2, k);


	/*== find out how many dimensions a point is*/
	getline(infile, line);
	getline(infile, line);
	istringstream iss_d(line);
	while(getline(iss_d, coord, ' '))
		dimensions++;

	dimensions -= 1;

	infile.clear();
	infile.seekg(0, ios::beg);


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
	if( type == "COS" )
		getline(infile, line);

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
		hash_tableptr->put(point);

		/*== clear vector for next iteration*/
		point.clear();
	}
	

	/*== get each vector from query file*/

	exit(EXIT_SUCCESS);
}
