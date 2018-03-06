#include "stdafx.h"
#include "EulerMaruyama.h"

EulerMaruyama::EulerMaruyama() {
	//default
}



EulerMaruyama::~EulerMaruyama()
{
}

double EulerMaruyama::OneStepAhead(double previous) {
	return previous + aFunc(previous)*mesh + bFunc(previous)*dW; // apply Euler's method
}