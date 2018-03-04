#include "stdafx.h"
#include "Fishman.h"
#include "ParkMiller.h"
#include <cmath>

Fishman::~Fishman()
{
}

double Fishman::acceptancetest(double x) {
	return exp(-0.5*(x - 1)*(x - 1));
}
double Fishman::GetOneUniformRandom() {
	return generator->GetOneUniformRandom();
}
double Fishman::RandomNumber() {
	double u1, u2, x;
	
		while (true) {
			u1 = GetOneUniformRandom();
			u2 = GetOneUniformRandom();
			u3 = GetOneUniformRandom();
			x = -log(u1);
			if (u2 <= acceptancetest(x)) return x;
		}
	
	
}
double Fishman::GetOneGaussianRandom() {
	double x = RandomNumber();
	if ( u3 <= 0.5) return -1.0*x;
	else return x;
}