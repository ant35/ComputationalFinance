#pragma once
#ifndef OPTION_H
#define OPTION_H

#include "Asset.h"
#include <string>
using std::cout;
using std::endl;

class Option :
	public Asset
{
public:
	Option();
	Option(double volatility ,double strike_price, double time_till_maturity_years, string type, string contract);
	Option(double volatility, double strike_price, double time_till_maturity_years, string type, string contract,
		double interest, double div_yield, double starting_price);
	~Option();
	void print() {
		cout << "Option Properties ... " << endl << "Starting Price: " << s_naught << endl << " Type: " << option_type <<"/"<<contract_type<< endl << 
			"Volatility: " << sigma << endl << "Strike: " << strike << endl << "Maturity: " << maturity << " year(s) " << endl << 
			"Interest Rate: " << interest_rate << endl << "Dividend Yield: " << dividend_yield << std::endl;

	}

	/*This overload implements the payoff function of the option based on its contract.*/
	double operator()(double final_price);

	/*Payoff for plain vanilla option*/
	double plain_vanilla(double final_price);

	/*Set lower barrier on barrier option.*/
	inline void setLowerBarrier(double l) { low = l; }

	/*Set upper barrier on barrier option.*/
	inline void setUpperBarrier(double u) { up = u; }

protected:
	/*Volatility of the asset.*/
	double sigma;

	/*Strike price for the asset.*/
	double strike;

	/*Lower barrier for barrier options*/
	double low;
	
	/*Time to maturity for the asset (years).*/
	double maturity;

	/*The type of option ("call" or "put").*/
	string option_type;

	/*Will define the payoff structure for the contract.*/
	string contract_type;
	
	/*Upper barrier for barrier options*/
	double up;
	
	friend class OptionPricer;
};
#endif
