#include "stdafx.h"
#include "UpAndOut.h"
#include "Probability.h"

UpAndOut::UpAndOut()
{
}


UpAndOut::~UpAndOut()
{
}

double UpAndOut::ClosedForm() {
	Probability p;
	struct maths {
		static double delta(double tau, double s, double r, double sigma, int pm) {
			if (pm >= 0)	return (1.0 / (sigma*sqrt(tau)))*(log(s) + (r + .5*sigma*sigma)*tau);
			else return (1.0 / (sigma*sqrt(tau)))*(log(s) + (r - .5*sigma*sigma)*tau);
		}
	};

	double years = maturity;
	double cfsol, divScale = exp(-dividend_yield*years);
	double ert = exp(-interest_rate*maturity), B = up;
	double x = s0;
	double tau = maturity , r = interest_rate - dividend_yield,	K = strike;
	double A = (-2.0 * r) / (sigma*sigma);
	double i1 = p.CumulativeNormal(maths::delta(tau, x / K, r, sigma, 1)) -
		p.CumulativeNormal(maths::delta(tau, x / B, r, sigma, 1));
	double i2 = p.CumulativeNormal(maths::delta(tau, x / K, r, sigma, -1)) -
		p.CumulativeNormal(maths::delta(tau, x / B, r, sigma, -1));
	double i3 = p.CumulativeNormal(maths::delta(tau, B*B / (K*x), r, sigma, 1)) -
		p.CumulativeNormal(maths::delta(tau, B / x, r, sigma, 1));
	double i4 = p.CumulativeNormal(maths::delta(tau, B*B / (K*x), r, sigma, -1)) -
		p.CumulativeNormal(maths::delta(tau, B / x, r, sigma, -1));

	cfsol = x*divScale*i1 - ert*K*i2 - B*divScale*pow(x / B, A)*i3
		+ ert*K*pow(x / B, A + 1)*i4;

	return cfsol;
}

bool UpAndOut::KnockedOut(double spot) {
	if (spot >= up) return true;
	return false;
}
