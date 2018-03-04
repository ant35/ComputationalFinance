#pragma once
#ifndef PROBABILITY_H
#define PROBABILITY_H
#include <iostream>
#include "Generator.h"
#include "myRand.h"

using std::string;
class Probability
{
public:
	Probability();
	~Probability();

	/*Uses Marsaglia Box Muller to generate random normal
	*@param rng choose the random number generator to generate the normal (parkmiller, ecuyer, default = rand)
	*/
	double static GetOneGaussianByBoxMuller(Generator *);
	double NormalDensity(double);
	double InverseCumulativeNormal(double u);
	double CumulativeNormal(double x);
	double GetOneGaussianBySummation();
};
#endif
