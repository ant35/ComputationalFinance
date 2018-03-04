#pragma once
#ifndef FISHMAN_H
#define FISHMAN_H
#include <iostream>

#include "Generator.h"
using std::string;

class Fishman: public Generator
{
public:
	Fishman(Generator * gen) :generator(gen) {};
	~Fishman();
	virtual double RandomNumber();
	void RestartSeed() { generator->RestartSeed(); };
	virtual double GetOneUniformRandom();
	virtual double GetOneGaussianRandom();
	virtual inline double GetLargestM() { return generator->GetLargestM(); };
	virtual void print() { cout << "Fishman random number generator" << endl; };
	double acceptancetest(double x);
	/*Generates a standard normal random variable
	*@param rng either parkmiller or ecuyer
	*/
private:
	Generator * generator;
	double u3;
	double pi = 3.14159265359;
	double sqrtPIover2 = sqrt(pi / 2);
};

#endif FISHMAN_H