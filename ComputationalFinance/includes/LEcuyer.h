#pragma once
#ifndef LECUYER_H
#define LECUYER_H
#include "Generator.h"
class LEcuyer: public Generator
{
public:
	LEcuyer();
	LEcuyer(int* seed);
	~LEcuyer();

	double RandomNumber();
	void RestartSeed();
	double GetOneUniformRandom();
	double GetOneGaussianRandom();
	double inline GetLargestM() { return m1; };
	virtual inline void print() { std::cout << "L'Ecuyer random number generator" << std::endl; }

private:
	int * origSeed;
	int x10;
	int x11;
	int x12;
	int x20;
	int x21;
	int x22;
	int m1 = 2147483647;
	int m2 = 2145483479;
	int a11 = 0;
	int a12 = 63308;
	int a13 = -183326;
	int a21 = 86098;
	int a22 = 0;
	int a23 = -539608;
	int q12 = m1 / a12;
	int q13 = m1 / a13;
	int q21 = m2 / a21;
	int q23 = m2 / a23;
	int r12 = m1 - a12*q12;
	int r13 = m1 - a13*q13;
	int r21 = m2 - a21*q21;
	int r23 = m2 - a23*q23;
	double Invmp1 = 1.0 / (m1 + 1);
};
#endif // !LECUYER_H
