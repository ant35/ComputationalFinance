#pragma once
#ifndef MYRAND_H
#define MYRAND_H
#include <cstdlib>
#include "Generator.h"
#include "Probability.h"

class myRand :
	public Generator
{
public:
	myRand();
	~myRand();
	void RestartSeed() { srand(1); };
	virtual double RandomNumber() {
		return rand();
	};
	virtual double GetOneUniformRandom() {
		return rand() % 2;
	};
	virtual double GetOneGaussianRandom();
	virtual double GetLargestM() {
		return static_cast<double>(RAND_MAX);
	};
	virtual void print() { std::cout << "Rand() random number generator" << std::endl; };


};

#endif // !MYRAND_H