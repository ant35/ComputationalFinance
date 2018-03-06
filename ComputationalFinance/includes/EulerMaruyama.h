#pragma once
#ifndef EULERMARUYAMA_H
#define EULERMARUYAMA_H
#include "StochasticNumericalMethod.h"

class EulerMaruyama: public StochasticNumericalMethod
{
public:
	EulerMaruyama(double,double,double,double);
	~EulerMaruyama();

	double OneStepAhead(double previous);
	void inline setA(double a) { A = a; };
	void inline setB(double b) { B = b; };
	void inline setBprime(double bprime) { Bprime = bprime; };
	void inline setdW(double dw) { dW = dw; };
	void inline setMesh(double Mesh) { mesh = Mesh; };

};

#endif // !EULERMARUYAMA_H