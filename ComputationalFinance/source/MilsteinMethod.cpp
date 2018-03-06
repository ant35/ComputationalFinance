#include "stdafx.h"
#include "MilsteinMethod.h"


MilsteinMethod::MilsteinMethod(double a, double b, double bprime, double dw, double Mesh)
{
	setA(a); setB(b); setBprime(bprime); setdW(dw); setMesh(Mesh);
}


MilsteinMethod::~MilsteinMethod()
{
}

double MilsteinMethod::OneStepAhead(double previous) {
	return previous*(1 + A*mesh + B*dW+ 0.5*B*Bprime*(dW*dW - mesh));

}