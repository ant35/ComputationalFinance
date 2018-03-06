#pragma once
#ifndef OPTION_H
#define OPTION_H

#include "Asset.h"

class Option :
	public Asset
{
public:
	
	void print() = 0;
	/*Defines conditions for which the option is knocked out (if any).*/
	virtual bool KnockedOut(double spot) = 0;
	
	double Payoff(double spot) {	
		if (option_type == "call") return max(spot - strike, 0.0);
		else if (option_type == "put") return max(strike - spot, 0.0);
		else cout << "UNDEFINED OPTION TYPE: PLAINVANILLA: " << option_type << endl;

		return -1.0;
	};

	virtual double ClosedForm() = 0;
	inline void setMaturity(double mat) { maturity = mat; };
	inline void setOptionType(string callOrPut) { option_type = callOrPut; };
	inline void setStrike(double Strike) { strike = Strike; };
	inline void setVolatility(double volatility) { sigma = volatility; };
	
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

protected:
	/*Volatility of the underlying.*/
	double sigma;
	/*Strike price for the option.*/
	double strike;
	/*Time to maturity for the option (years).*/
	double maturity;
	/*The type of option ("call" or "put").*/
	string option_type;

	friend class OptionPricer;
};
#endif
