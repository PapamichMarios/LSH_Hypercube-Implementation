#include <iostream>

#include "fi.h"

/*================================ Constructor && Destructor*/
fi::fi()
{
	
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
