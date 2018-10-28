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
#include <string>
#include <math.h>

#include "hash_table.h"

/*== c used for Range Search*/
#define C 1

using namespace std;

int main(int argc, char **argv)
{
	int opt;
	int k, L;	
	int inputFileIndex, queryFileIndex, outputFileIndex;

	int i;
	HashTable<vector<double>> ** hash_tableptr;


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

	/*== find out if we want euclidean or cosine*/
	string type = help_functions::find_type(infile);

	/*== find out how many lines the file has(used for euclidean)*/
	int tableSize = help_functions::calculate_tableSize(infile, type, k);

	/*== find out how many dimensions a point is*/
	int dimensions = help_functions::calculate_dimensions(infile);

	/*== create hash table based on type and assign a pointer*/
	if(type == "COS")
	{
		hash_tableptr = new HashTable<vector<double>>*[L];

		for(int i=0; i<L; i++)
			hash_tableptr[i] = new HashTable_COS<vector<double>>(tableSize, k, dimensions);
	}
	else
	{
		hash_tableptr = new HashTable<vector<double>>*[L];

		for(int i=0; i<L; i++)
			hash_tableptr[i] = new HashTable_EUC<vector <double>>(tableSize, k, dimensions);
	}

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
		for(i=0; i<L; i++)
			hash_tableptr[i]->put(point, identifier);
		/*== clear vector for next iteration*/
		point.clear();
	}
	

	/*== get each vector from query file*/
	ifstream queryfile(argv[queryFileIndex]);
	ofstream outputfile(argv[outputFileIndex]);

	vector<string> measurements;
	vector<vector<string>> hash_table_measurements;
	map<string, double> dist_map;

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
		for(i=0; i<L; i++)
			hash_tableptr[i]->RS(point, outputfile, C, R, dist_map);
		help_functions::print_RS(dist_map, outputfile);

		/*== Approximate Nearest Neighbour*/
		outputfile << "LSH Neighbour: ";
		for(i=0; i<L; i++)
		{
			measurements = hash_tableptr[i]->ANN(point);
			hash_table_measurements.push_back(measurements);
		}
		help_functions::print_ANN(hash_table_measurements, outputfile);

		/*== Nearest Neighbour*/
		hash_table_measurements.clear();

		outputfile << "Nearest neighbour: ";
		for(i=0; i<L; i++)
		{
			measurements = hash_tableptr[i]->NN(point);
			hash_table_measurements.push_back(measurements);
		}
		help_functions::print_NN(hash_table_measurements, outputfile);
		outputfile << endl;

		/*== clear vector for next iteration*/
		point.clear();
		hash_table_measurements.clear();
		dist_map.clear();
	}

	exit(EXIT_SUCCESS);
}
