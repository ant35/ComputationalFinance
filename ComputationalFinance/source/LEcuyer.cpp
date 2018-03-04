#include "stdafx.h"
#include "LEcuyer.h"
#include "Probability.h"

LEcuyer::LEcuyer() {
	int seed[] =  { 1,2,3,5,7,11 };
	origSeed = seed;
	x10 = 1;
	x11 = 2;
	x12 = 3;
	x20 = 5;
	x21 = 7;
	x22 = 11;
}
LEcuyer::LEcuyer(int* seed)
{
	origSeed = seed;
	x10 = *seed;
	x11 = *(seed + 1);
	x12 = *(seed + 2);
	x20 = *(seed + 3);
	x21 = *(seed + 4);
	x22 = *(seed + 5);

}


LEcuyer::~LEcuyer()
{
}

void LEcuyer::RestartSeed() {
	x10 = *origSeed;
	x11 = *(origSeed + 1);
	x12 = *(origSeed + 2);
	x20 = *(origSeed + 3);
	x21 = *(origSeed + 4);
	x22 = *(origSeed + 5);
}
double LEcuyer::RandomNumber() {
	int h, p12, p13, p21, p23;
	//Compute a12*x11 mod m1
	h = x11 / q12;
	p12 = a12*(x11 - h*q12) - h*r12;
	if (p12 < 0)p12 = p12 + m1;

	//Compute a13*x10 mod m1
	h = x10 / q13;
	p13 = a13*(x10 - h*q13) - h*r13;
	if (p13 < 0)p13 = p13 + m1;

	//Compute a21*x22 mod m2
	h = x22 / q21;
	p21 = a21*(x22 - h*x22) - h*r21;
	if (p21 < 0)p21 = p21 + m2;
	
	//Compute a23*x20 mod m2
	h = x20 / q23;
	p23 = a23*(x20 - h*q23) - h*r23;
	if (p23 < 0) p23 = p23 + m2;

	//Update x11 and x10
	x10 = x11;
	x11 = x12;

	//Compute (p12+p13) mod m1 and update x12
	p12 = p12 - m1;
	x12 = p12 + p13;
	if (x12 < 0) x12 = x12 + m1;

	//Update x21 and x20
	x20 = x21;
	x21 = x22;
	
	//Compute (p21+p23) mod m2 and update x22
	p21 = p21 - m2;
	x22 = p21 + p23;
	if (x22 < 0) x22 = x22 + m2;
	
	//Compute x1 mod m1
	if (x12 < x22) return x12 - x22 + m1;
	else return x12 - x22;
}

double LEcuyer::GetOneUniformRandom() {
	double U;
	U = -RandomNumber(); 
	if (U == 0) U = m1; 
	return U*Invmp1;
}

double LEcuyer::GetOneGaussianRandom() {
	Probability prob;
	return prob.InverseCumulativeNormal(GetOneUniformRandom());
}