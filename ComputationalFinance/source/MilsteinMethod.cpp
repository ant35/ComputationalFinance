#include "stdafx.h"
#include "MilsteinMethod.h"


MilsteinMethod::MilsteinMethod()
{
	
}


MilsteinMethod::~MilsteinMethod()
{
}

double MilsteinMethod::OneStepAhead(double previous) {
	return previous + aFunc(previous)*mesh + bFunc(previous)*dW +
		0.5*bFunc(previous)*bPrimeFunc(previous)*(dW*dW - mesh);

}