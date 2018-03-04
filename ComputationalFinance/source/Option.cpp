#include "stdafx.h"
#include "Option.h"


template <class T>
double max(T v1, T v2) {
	if (v1 >= v2) return v1;
	else return v2;
}
template <class T>
double min(T v1, T v2) {
	if (v1 <= v2) return v1;
	else return v2;
}

Option::Option()
{
}

Option::Option(double volatility, double strike_price, double time_till_maturity_years, string type, string contract) {
	sigma = volatility; strike = strike_price; maturity = time_till_maturity_years; option_type = type; contract_type = contract;
	
};

Option::Option(double volatility, double strike_price, double time_till_maturity_years, string type, string contract,
	double interest, double div_yield,double starting_price) {
	sigma = volatility; strike = strike_price; maturity = time_till_maturity_years; option_type = type; contract_type = contract;
	interest_rate = interest; dividend_yield = div_yield; s_naught = starting_price;
};

Option::~Option()
{
	
}

/*Calculates payoff based on the type of contract.*/
double Option::operator()(double final_price) {
	if (contract_type == "plain vanilla") return plain_vanilla(final_price);
	else if (contract_type == "up and out") return plain_vanilla(final_price);
	else {
		std::cout << "Contract type " << contract_type << " not defined. " << std::endl;
		return 0;
	}
}

double Option::plain_vanilla(double final_price) {
	if (option_type == "call") return max(final_price - strike, 0.0);
	else return max(strike - final_price, 0.0);
}
