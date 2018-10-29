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
	int k = 4;
	int L = 5;	
	int inputFileIndex , inputFileFlag  = 0;
	int queryFileIndex , queryFileFlag  = 0;
	int outputFileIndex, outputFileFlag = 0;

	int i;
	HashTable<vector<double>> ** hash_tableptr;


	/*== get all input arguments through getopt()*/
	while ((opt = getopt(argc, argv, "d:q:k:L:o:")) != -1)
	{
        switch (opt)
		{
			case 'd':
				inputFileIndex = optind-1;
				inputFileFlag = 1; 
				break;
			case 'q':
				queryFileIndex = optind-1;
				queryFileFlag = 1;
				break;
			case 'k':
				k = atoi(optarg);
				break;
			case 'L':
				L = atoi(optarg);
				break;
			case 'o':
				outputFileIndex = optind-1;
				outputFileFlag = 1;
				break;
			case '?':
				cout << "Invalid option" << endl;
				exit(EXIT_FAILURE);
		}
    }


	/*== open input file*/
	ifstream infile;
	if( inputFileFlag )
	{
		infile.open(argv[inputFileIndex]);
		if(!infile.is_open())
		{
			cout << "Wrong input file given." << endl;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		string input;

		while(!infile.is_open())
		{
			cout << "Please enter a valid input file: ";
			cin >> input;

			infile.open(input);
		}
	}
		
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
	vector<double> point(dimensions);
	int j=0;

	string identifier;
	if( type == "COS" )
		getline(infile, line);

	while(getline(infile, line))
	{
		j=0;
		istringstream iss(line);
		/*== split the line into 128 coords which we save into a vector of size 128*/
		getline(iss, identifier, ' ');
		while(getline(iss, coord, ' '))
		{
			if( j < dimensions )
				point[j] = stod(coord);
			
			j++;
		}
		
		/*== hash the vector point to our hash table*/
		for(i=0; i<L; i++)
			hash_tableptr[i]->put(point, identifier);
	}
	
	/*== open query file*/
	ifstream queryfile;
	if( queryFileFlag )
	{
		queryfile.open(argv[queryFileIndex]);
		if(!queryfile.is_open())
		{
			cout << "Wrong query file given." << endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		string input;

		while(!queryfile.is_open())
		{
			cout << "Please enter a valid query file: ";
			cin >> input;

			queryfile.open(input);
		}

	}

	/*== open output file*/
	ofstream outputfile;
	if(outputFileFlag)
	{
		outputfile.open(argv[outputFileIndex]);
		if(!outputfile.is_open())
		{
			cout << "Wrong outputfile given." << endl;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		string output;
		
		while(!outputfile.is_open())
		{
			cout << "Please enter an output file: ";
			cin >> output;

			outputfile.open(output);
		}
	}


	/*== get each vector from query file*/
	vector<string> measurements(3);
	vector<vector<string>> hash_table_measurements(L);
	map<string, double> dist_map;

	double distance_NN = INT_MAX, distance_ANN = INT_MAX, time_ANN;
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
		j=0;
		istringstream iss(line);
		/*== split the line into 128 coords which we save into a vector of size 128*/
		getline(iss, identifier, ' ');
		while(getline(iss, coord, ' '))
		{
			if( j < dimensions )
				point[j] = stod(coord);

			j++;
		}
		
		/*== print id to file*/
		outputfile << "Query: " << identifier << endl;

		/*== Range Search*/
		outputfile << "R-near neighbours:" << endl;
		for(i=0; i<L; i++)
			hash_tableptr[i]->RS(point, C, R, dist_map);
		help_functions::print_RS(dist_map, outputfile);

		/*== Approximate Nearest Neighbour*/
		outputfile << "LSH Neighbour: ";
		for(i=0; i<L; i++)
		{
			measurements = hash_tableptr[i]->ANN(point, distance_ANN, time_ANN);
			hash_table_measurements[i] = measurements;
		}
		help_functions::print_ANN(hash_table_measurements, outputfile);

		/*== Nearest Neighbour*/
		outputfile << "Nearest neighbour: ";
		hash_tableptr[0]->NN(point, outputfile, distance_NN);

		outputfile << endl;

		if( distance_ANN/distance_NN > approaching_factor )
			approaching_factor = distance_ANN/distance_NN;

		average_time_ANN += time_ANN;

		/*== clear map for next iteration*/
		dist_map.clear();

		/*== reset distances for next loop*/
		distance_ANN = INT_MAX;
		distance_NN  = INT_MAX;
	}

	/*== print average time && approaching factor*/
	average_time_ANN /= querySize;

	outputfile << "Approaching factor: " << approaching_factor << endl;
	outputfile << "Average time ANN: " << average_time_ANN << endl;
	outputfile << "Memory used: " << L * hash_tableptr[0]->memory_used(dimensions) << endl;

	/*== free hash_tables*/
	for(i=0; i<L; i++)
	{
		delete hash_tableptr[i];
		hash_tableptr[i] = NULL;
	}
	delete[] hash_tableptr;

	/*== close files*/
	infile.close();
	queryfile.close();
	outputfile.close();

	exit(EXIT_SUCCESS);
}
