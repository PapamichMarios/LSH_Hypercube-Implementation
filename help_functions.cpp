#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <random>

namespace help_functions
{
	double * normal_distribution_vector(int dim)
	{
	 	double* vector = (double*)std::calloc(dim+1, sizeof(double));

		std::random_device rd;
		std::default_random_engine generator(rd());
		std::normal_distribution<double> distribution(0.0, 1.0);

		for(int i=0; i<dim; i++)
		{
			vector[i] = distribution(generator);
		}

		return vector;
	}
}
