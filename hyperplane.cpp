#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <random>

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
	random_device rd;
	default_random_engine generator(rd());
	uniform_real_distribution<double> distribution(0.0, double(w));

	this->t = distribution(generator);

	/*== vector: single precision coords N(0,1)*/
	this->vector = help_functions::normal_distribution_vector(dim);
}

Hyperplane::~Hyperplane()
{
	free(this->vector);
	this->vector = NULL;	
}
/*============================= Rest of the functions*/
double * Hyperplane::getVector()
{
	return this->vector;
}

int Hyperplane::getW()
{
	return this->w;
}

double Hyperplane::getT()
{
	return this->t;
}

void Hyperplane::printHyperplane(int dim)
{
	cout << "-t: " << this->t << endl;
	cout << "-w: " << this->w << endl;
	cout << "-vector: " << endl;

	for(int i=0; i<dim; i++)
		cout << this->vector[i] << " " ;

	cout << endl << "---------" << endl << endl;
}
