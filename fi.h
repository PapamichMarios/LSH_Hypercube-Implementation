#ifndef __FI_H__
#define __FI_H__

#include "hyperplane.h"

class fi
{
	private:
	 	Hyperplane ** h_array;
		int * 		 r_array;

	public:
		fi();
		~fi();

		Hyperplane ** getH_array();
		int * getR_array();
};
#endif
