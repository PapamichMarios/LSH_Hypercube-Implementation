#ifndef __HELP_FUNCTIONS_H__
#define __HELP_FUNCTIONS_H__

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <random>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>

namespace help_functions
{
	inline void print_NN(std::vector<std::vector<std::string>> hash_table_measurements,std::ofstream& outputfile)
	{
		std::vector<double> distance_list;

		for(unsigned int i=0; i<hash_table_measurements.size(); i++)
			distance_list.push_back(stod(hash_table_measurements.at(i).at(0)));
			
		auto min = std::min_element(distance_list.begin(), distance_list.end());
		int min_index = std::distance(distance_list.begin(), min);

		/*== print to file*/
		outputfile << hash_table_measurements.at(min_index).at(1) << std::endl;
		outputfile << "distanceTrue: " << *min << std::endl;
		outputfile << "tTrue: " << hash_table_measurements.at(min_index).at(2) << " secs" << std::endl;
	}

	inline void print_ANN(std::vector<std::vector<std::string>> hash_table_measurements,std::ofstream& outputfile)
	{
		std::vector<double> distance_list;

		for(unsigned int i=0; i<hash_table_measurements.size(); i++)
			distance_list.push_back(stod(hash_table_measurements.at(i).at(0)));
			
		auto min = std::min_element(distance_list.begin(), distance_list.end());
		int min_index = std::distance(distance_list.begin(), min);

		/*== print to file*/
		outputfile << hash_table_measurements.at(min_index).at(1) << std::endl;
		outputfile << "distanceLSH: " << *min << std::endl;
		outputfile << "tLSH: " << hash_table_measurements.at(min_index).at(2) << " secs" << std::endl;
	}

	inline double * normal_distribution_vector(int dim)
	{
	 	double* vector = (double*)std::calloc(dim+1, sizeof(double));

		std::random_device rd;
		std::default_random_engine generator(rd());
		std::normal_distribution<double> distribution(0.0, 1.0);

		for(int i=0; i<dim; i++)
			vector[i] = distribution(generator);

		return vector;
	}

	template <typename K>
	inline double euclidean_distance(const K &vector1, const K &vector2)
	{
		double distance = 0;

		for(unsigned int i=0; i<vector1.size(); i++)
			distance += pow(vector1[i] - vector2[i], 2);

		return sqrt(distance);
	}

	inline int calculate_dimensions(std::ifstream& infile)
	{
		int dimensions=0;
		std::string line, coord;

		getline(infile, line);
		getline(infile, line);
		std::istringstream iss_d(line);
		while(getline(iss_d, coord, ' '))
			dimensions++;

		dimensions -= 2;

		infile.clear();
		infile.seekg(0, std::ios::beg);

		return dimensions;
	}

	inline int calculate_tableSize(std::ifstream& infile, std::string type, int k)
	{
		std::string line;
		int table_size;

		if(type == "EUC")
		{
			int lines_counter = 0;

			while(getline(infile, line))	
				++lines_counter;

			
			table_size = lines_counter/2;

			infile.clear();
			infile.seekg(0, std::ios::beg);
		}
		else
			table_size = pow(2, k);

		return table_size;
	}

	inline std::string find_type(std::ifstream& infile)
	{
		std::string line, type;

		getline(infile, line);
		if( line.compare(0, 6, "cosine") == 0 )
			type = "COS";
		else
			type = "EUC";

		infile.clear();
		infile.seekg(0, std::ios::beg);

		return type;
	}

}
#endif
