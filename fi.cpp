#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "fi.h"
#include "hyperplane.h"

using namespace std;

/*================================ Constructor && Destructor*/
fi::fi(int k, int dim)
{
	int i;
	vector<double> t_list;

	/*constructing the hs*/
	this->h_array = new Hyperplane*[k];
	for(i=0; i<k; i++)
	{
		this->h_array[i] = new Hyperplane(dim);
		
		/*== checking that hs differ*/
		if( std::find(t_list.begin(), t_list.end(), h_array[i]->getT()) != t_list.end() )
		{
			cout << "potential conflict in hs, getting a new one" << endl;
			i-=1;
			free(this->h_array[i]);
			
			continue;
		}
		else
			t_list.push_back(this->h_array[i]->getT());
	}

	/*constructing the rs*/
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int> distribution(0.0, 100.0);
	
	this->r_array = new int[k];
	for(i=0; i<k; i++)
		this->r_array[i] = distribution(generator);
}

fi::~fi()
{
	
}
/*================================ Rest of the functions*/
Hyperplane ** fi::getH_array()
{
	return this->h_array;
}

int * fi::getR_array()
{
	return this->r_array;
}
