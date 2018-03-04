#pragma once
#ifndef BOXMULLERMARSAGLIA_H
#define BOXMULLERMARSAGLIA_H

#include "Generator.h"
class BoxMullerMarsaglia :
	public Generator
{
public:
	BoxMullerMarsaglia(Generator * gen);
	~BoxMullerMarsaglia();
	virtual double RandomNumber();
	virtual double GetOneUniformRandom();
	virtual double GetOneGaussianRandom();
	virtual inline double GetLargestM();
	virtual void RestartSeed();
	virtual void print() {
		cout << "Box Muller Marsaglia normal random generator with ";
		generator->print();
		cout << endl;
	};

private: 
	Generator * generator;
};

#endif // !BOXMULLERMARSAGLIA_H