#pragma once
#ifndef PROBABILITY_H
#define PROBABILITY_H
class Probability
{
public:
	Probability();
	~Probability();

	double static GetOneGaussianByBoxMuller();
	double NormalDensity(double);
	double InverseCumulativeNormal(double u);
	double CumulativeNormal(double x);
	double GetOneGaussianBySummation();
};
#endif
