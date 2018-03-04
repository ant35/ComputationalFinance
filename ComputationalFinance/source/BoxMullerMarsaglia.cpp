#include "stdafx.h"
#include "BoxMullerMarsaglia.h"


BoxMullerMarsaglia::BoxMullerMarsaglia(Generator * gen)
{
	generator = gen;
}


BoxMullerMarsaglia::~BoxMullerMarsaglia()
{
}
void BoxMullerMarsaglia::RestartSeed() {
	generator->RestartSeed();
}
double BoxMullerMarsaglia::RandomNumber() {
	return generator->RandomNumber();
}
double BoxMullerMarsaglia::GetOneUniformRandom() {
	return generator->GetOneUniformRandom();
}
double BoxMullerMarsaglia::GetLargestM() {
	return generator->GetLargestM();
}
double BoxMullerMarsaglia::GetOneGaussianRandom() {
	double result;

	double x;
	double y;

	double sizeSquared;
	do
	{
		x = 2.0*RandomNumber() / GetLargestM() - 1;
		y = 2.0*RandomNumber() / GetLargestM() - 1;
		sizeSquared = x*x + y*y;
	} while
		(sizeSquared >= 1.0);

	result = x*sqrt(-2 * log(sizeSquared) / sizeSquared);

	return result;
}
