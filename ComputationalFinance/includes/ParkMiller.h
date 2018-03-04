#pragma once
#ifndef PARKMILLER_H
#define PARKMILLER_H
#include "Generator.h"

class ParkMiller: public Generator
{
public:
	ParkMiller();
	ParkMiller(int s);
	~ParkMiller();
	double RandomNumber();
	void RestartSeed() { seed = origSeed; }
	double GetOneUniformRandom();
	double GetOneGaussianRandom();
	double inline GetLargestM() { return m; };
	virtual inline void print() { std::cout << "Park Miller random number generator" << std::endl; }
private:
	int seed;
	int origSeed;
	int m = 2147483647;
	int a = 16807;
	int q = m / a;
	int r = m - a*q;
	double invm = 1.0 / m;

};

#endif // !PARKMILLER_H