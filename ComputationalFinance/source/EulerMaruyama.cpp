#include "stdafx.h"
#include "EulerMaruyama.h"


EulerMaruyama::EulerMaruyama(double a, double b, double dw, double Mesh)
{
	setA(a); setB(b); setdW(dw); setMesh(Mesh);
}


EulerMaruyama::~EulerMaruyama()
{
}

double EulerMaruyama::OneStepAhead(double previous) {
	return previous*(1 + A*mesh + B*dW); // apply Euler's method
}