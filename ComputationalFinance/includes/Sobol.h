#pragma once
#ifndef SOBOL_H
#define SOBOL_H
#include "Generator.h"

class Sobol :
	public Generator
{
public:
	Sobol(int,int);
	~Sobol();
	virtual double RandomNumber();
	virtual double GetOneUniformRandom();
	virtual double GetOneGaussianRandom();
	virtual inline double GetLargestM();
	virtual void RestartSeed();
	virtual void print();


private:
	double ** sobolArray;
	unsigned N, D;
	unsigned long row = 0, column = 0;

};

#endif // !SOBOL_H