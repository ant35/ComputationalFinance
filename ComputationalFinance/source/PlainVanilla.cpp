#include "stdafx.h"
#include "PlainVanilla.h"
#include "Probability.h"

PlainVanilla::PlainVanilla()
{
}




PlainVanilla::PlainVanilla(double volatility, double strike_price, double time_till_maturity_years, string type,
	double interest, double div_yield, double starting_price) {
	setVolatility(volatility);
	setStrike(strike_price);
	setMaturity(time_till_maturity_years);
	setOptionType(type);
	setInterestRate(interest);
	setDividend(div_yield);
	setS0(starting_price);
};

PlainVanilla::~PlainVanilla()
{
}

double PlainVanilla::ClosedForm() {
	Probability probability;
	double a = sigma*sqrt(maturity);
	double scale = 1.0 / a;
	double A = log(s0 / strike);
	double B = interest_rate + sigma*sigma * 0.5;

	double d1 = scale*(A + B*maturity);
	double d2 = d1 - a;

	double N_d1 = probability.CumulativeNormal(d1);
	double N_d2 = probability.CumulativeNormal(d2);

	if (option_type == "call")
		return N_d1*exp(-dividend_yield*maturity)*s0 - N_d2 * strike * exp(-1 * interest_rate*maturity);
	else
		return probability.CumulativeNormal(-1 * d2) * strike * exp(-1 * interest_rate*maturity)
		- probability.CumulativeNormal(-1 * d1)*exp(-dividend_yield*maturity)*s0;
	
}
