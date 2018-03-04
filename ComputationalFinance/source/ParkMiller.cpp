#include "stdafx.h"
#include "ParkMiller.h"
#include "Probability.h"

ParkMiller::ParkMiller()
{
	seed = 1;
	origSeed = seed;
}

ParkMiller::ParkMiller(int s) : seed(s) {
	if (seed <= 0) seed = 1;
	origSeed = seed;
}

ParkMiller::~ParkMiller()
{
}

double ParkMiller::RandomNumber() {
	int k, p;
	/* Component 1 */
	k = seed / q;
	seed = a*(seed - k*q) - k*r;
	if (seed<0) seed = seed + m;
	return seed;
}

double ParkMiller::GetOneUniformRandom() {
	double u = RandomNumber();
	return u*invm;
}

double ParkMiller::GetOneGaussianRandom() {
	Probability prob;
	return prob.InverseCumulativeNormal(GetOneUniformRandom());
}