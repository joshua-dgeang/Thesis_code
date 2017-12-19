#ifndef GAUSSRAND_HPP
#define GAUSSRAND_HPP
class GaussRand{
	public:
		GaussRand(double m = 0, double s = 1.0,int seed = 31399199);
		~GaussRand();
		double GetRN ();
	private:
		const double mean;
		const double std;
		const int randSeed;
};
#endif
