#pragma once
#ifndef MILSTEINMETHOD_H
#define MILSTEINMETHOD_H
#include "StochasticNumericalMethod.h"

class MilsteinMethod: public StochasticNumericalMethod
{
public:
	MilsteinMethod(double, double, double, double, double);
	~MilsteinMethod();

	double OneStepAhead(double previous);
	void inline setA(double a) { A = a; };
	void inline setB(double b) { B = b; };
	void inline setBprime(double bprime) { Bprime = bprime; };
	void inline setdW(double dw) { dW = dw; };
	void inline setMesh(double Mesh) { mesh = Mesh; };
};

#endif // !MILSTEINMETHOD_H