#pragma once
#ifndef GENERATOR_H
#define GENERATOR_H
#include <iostream>

using namespace std;
/*This class is an interface for generators Park Miller, and l'Ecuyer.*/
class Generator
{
public:

	Generator()
	{
	}

	~Generator()
	{
	}
	virtual double RandomNumber()=0;
	virtual double GetOneUniformRandom()=0;
	virtual double GetOneGaussianRandom()=0;
	virtual inline double GetLargestM()=0;
	virtual void RestartSeed()=0;
	virtual void print()=0;
	
};

#endif // !GENERATOR_H