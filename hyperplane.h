#ifndef __HYPERPLANE_H__
#define __HYPERPLANE_H__

#define DIM 128

#include <string>

class Hyperplane
{
	private:
		float * vector;
		int w;
		float t;
	public:
		Hyperplane(int dim);
		~Hyperplane();


};
#endif
