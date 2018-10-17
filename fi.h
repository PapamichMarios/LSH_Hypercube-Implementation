#ifndef __FI_H__
#define __FI_H__

#include "hyperplane.h"

class fi
{
	private:
	 	Hyperplane ** h_array;
		int * 		 r_array;
		int k;
		int dim;

	public:
		fi(int k, int dim);
		~fi();

		Hyperplane ** getH_array();
		int * getR_array();
		int hashValue(std::vector<double> p, int table_size);
};
#endif
