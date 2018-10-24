#ifndef __HYPERPLANE_H__
#define __HYPERPLANE_H__

#include <string>

class Hyperplane
{
	protected:
		double * v;

	public:
		Hyperplane(int dim);
		~Hyperplane();

		/*== getters*/
		double* getVector();

		virtual int computeH(std::vector<double> p) = 0;

};

class Hyperplane_EUC : public Hyperplane
{
	private:
		int w;
		double t;

	public:
		Hyperplane_EUC(int dim);
		~Hyperplane_EUC();

		int getW();
		double getT();

		void printHyperplane(int dim);
		int computeH(std::vector<double> p);
		
};

class Hyperplane_COS : public Hyperplane
{
	public:
		Hyperplane_COS(int dim);
		~Hyperplane_COS();

		int computeH(std::vector<double> p);
};
		
		
#endif
