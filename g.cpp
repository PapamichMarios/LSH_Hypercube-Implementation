#include <vector>

#include "g.h"
#include "hyperplane.h"

using namespace std;

/*================================ Constructor && Destructor*/
G::G(int k, int dim)
{
	int i;
	
	this->k   = k;
	this->dim = dim;

	/*== constructing the hs*/
	this->h_array = new Hyperplane_COS*[k];
	for(i=0; i<k; i++)
	{
		this->h_array[i] = new Hyperplane_COS(dim);
		
		/*== checking that hs differ*/
	}
}

G::~G(){}

/*=============================== Rest of the funcs*/
Hyperplane_COS ** G::getH_array()
{
	return this->h_array;
}

int G::hashValue(vector<double> p)
{
	return 1;
}
