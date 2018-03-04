#include "stdafx.h"
#include <cmath>
#include <string>
#include <iostream>
#include "Probability.h"

using std::string;

Probability::Probability()
{
}


Probability::~Probability()
{
	
}

///All of the code below is based off of Mark Joshi's code. Copyright has been included.
/*
* Copyright(c) 2002
* Mark Joshi
*/

/*Generates a random variable with standard normal distribution*/
double Probability::GetOneGaussianByBoxMuller(Generator * generator)
{
	
	double result;

	double x;
	double y;

	double sizeSquared;
	do
	{
			x = 2.0*generator->GetOneUniformRandom() / generator->GetLargestM() - 1;
			y = 2.0*generator->GetOneUniformRandom() / generator->GetLargestM() - 1;
		sizeSquared = x*x + y*y;
	} while
		(sizeSquared >= 1.0);

	result = x*sqrt(-2 * log(sizeSquared) / sizeSquared);

	return result;

}

double Probability::GetOneGaussianBySummation()
{
	double result = 0;

	for (unsigned long j = 0; j < 12; j++)
		result += rand() / static_cast<double>(RAND_MAX);

	result -= 6.0;

	return result;

}

const double OneOverRootTwoPi = 0.398942280401433;

/* probability density for a standard Gaussian distribution*/
double Probability::NormalDensity(double x)
{
	return OneOverRootTwoPi*exp(-x*x / 2);
}

/* the InverseCumulativeNormal function via the Beasley-Springer/Moro approximation */
double Probability::InverseCumulativeNormal(double u)
{


	static double a[4] = { 2.50662823884,
		-18.61500062529,
		41.39119773534,
		-25.44106049637 };

	static double b[4] = { -8.47351093090,
		23.08336743743,
		-21.06224101826,
		3.13082909833 };

	static double c[9] = { 0.3374754822726147,
		0.9761690190917186,
		0.1607979714918209,
		0.0276438810333863,
		0.0038405729373609,
		0.0003951896511919,
		0.0000321767881768,
		0.0000002888167364,
		0.0000003960315187 };


	double x = u - 0.5;
	double r;

	if (fabs(x)<0.42) // Beasley-Springer
	{
		double y = x*x;

		r = x*(((a[3] * y + a[2])*y + a[1])*y + a[0]) /
			((((b[3] * y + b[2])*y + b[1])*y + b[0])*y + 1.0);

	}
	else // Moro
	{

		r = u;

		if (x>0.0)
			r = 1.0 - u;

		r = log(-log(r));

		r = c[0] + r*(c[1] + r*(c[2] + r*(c[3] + r*(c[4] + r*(c[5] + r*(c[6] +
			r*(c[7] + r*c[8])))))));

		if (x<0.0)
			r = -r;

	}

	return r;
}


/* standard normal cumulative distribution function */
double Probability::CumulativeNormal(double x)
{
	static double a[5] = { 0.319381530,
		-0.356563782,
		1.781477937,
		-1.821255978,
		1.330274429 };

	double result;

	if (x<-7.0)
		result = NormalDensity(x) / sqrt(1. + x*x);

	else
	{
		if (x>7.0)
			result = 1.0 - CumulativeNormal(-x);
		else
		{
			double tmp = 1.0 / (1.0 + 0.2316419*fabs(x));

			result = 1 - NormalDensity(x)*
				(tmp*(a[0] + tmp*(a[1] + tmp*(a[2] + tmp*(a[3] + tmp*a[4])))));

			if (x <= 0.0)
				result = 1.0 - result;

		}
	}

	return result;
}
