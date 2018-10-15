#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include "hyperplane.h"
#include "help_functions.h"

#define W 4

using namespace std;

/*============================= Constructor && Destructor*/
Hyperplane::Hyperplane(int dim)
{
	/*== w: 4 (can change from definition)*/
	this->w = W;

	/*== t: uniformly random in [0,w)*/
	this->t = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/W));

	/*== vector: single precision coords N(0,1)*/
	this->vector = help_functions::normal_distribution_vector(dim);
}

Hyperplane::~Hyperplane()
{
	
}

/*============================= Rest of the functions*/

