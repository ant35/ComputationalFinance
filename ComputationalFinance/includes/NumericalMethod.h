#pragma once
#ifndef NUMERICALMETHOD_H
#define NUMERICALMETHOD_H

/*Interface for numerical methods.*/
class NumericalMethod
{
public:

	NumericalMethod()
	{
	}

	~NumericalMethod()
	{
	}

	virtual double OneStepAhead(double) = 0;

};

#endif // !NUMERICALMETHOD_H