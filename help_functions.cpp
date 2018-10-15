#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <random>

namespace help_functions
{
	float * normal_distribution_vector(int dim)
	{
	 	float* vector = (float*)std::calloc(dim+1, sizeof(float));

		std::normal_distribution<float> distribution(0.0, 1.0);
		std::default_random_engine generator;

		for(int i=0; i<dim; i++)
		{
			vector[i] = distribution(generator);
		}

		return vector;
	}
}
