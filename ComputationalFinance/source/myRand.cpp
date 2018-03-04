#include "stdafx.h"
#include "myRand.h"


myRand::myRand()
{
}


myRand::~myRand()
{
}

double myRand::GetOneGaussianRandom() {
	Probability p;
	return p.InverseCumulativeNormal(GetOneUniformRandom());
};