#ifndef __HYPERPLANE_H__
#define __HYPERPLANE_H__

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

		/*== getters*/
		float* getVector();
		int getW();
		float getT();

};
#endif
