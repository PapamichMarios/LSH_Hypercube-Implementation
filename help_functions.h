#ifndef __HELP_FUNCTIONS_H__
#define __HELP_FUNCTIONS_H__

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <random>

namespace help_functions
{
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
}
#endif
