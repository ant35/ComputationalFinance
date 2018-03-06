#pragma once
#include "NumericalMethod.h"
class StochasticNumericalMethod :
	public NumericalMethod
{
public:

	StochasticNumericalMethod()
	{
	}

	~StochasticNumericalMethod()
	{
	}
	virtual double OneStepAhead(double) = 0;
	virtual void inline setA(double) = 0;
	virtual void inline setB(double) = 0;
	virtual void inline setBprime(double) = 0;
	virtual void inline setdW(double) = 0;
	virtual void inline setMesh(double) = 0;

protected:
	double A;
	double B;
	double Bprime;
	double dW;
	double mesh;
};

