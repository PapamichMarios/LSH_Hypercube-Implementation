#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <random>
#include <cmath>
#include <climits>

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
	this->v = help_functions::normal_distribution_vector(dim);
}

Hyperplane::~Hyperplane()
{
	free(this->v);
	this->v = NULL;	
}
/*============================= Rest of the functions*/
double * Hyperplane::getVector()
{
	return this->v;
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
		cout << this->v[i] << " " ;

	cout << endl << "---------" << endl << endl;
}

int Hyperplane::computeH(vector<double> p)
{
	int sum=0;
	long int dot_product=0;

	for(unsigned int i=0; i<p.size(); i++)
		dot_product += v[i]*p[i];

	/*== handle overflow by restarting*/
	if( dot_product > INT_MAX || dot_product < INT_MIN)
	{
		cout << "Overflow detected(dot product), exiting program..." << endl;
		exit(EXIT_FAILURE);
	}
 
	return sum = floor( (double) ((int)dot_product + this->t)/this->w );
}
