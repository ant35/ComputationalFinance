#pragma once
#ifndef PLAINVANILLA_H
#define PLAINVANILLA_H
#include "Option.h"

class PlainVanilla :
	public Option
{
public:
	PlainVanilla();
	~PlainVanilla();
	PlainVanilla(double volatility, double strike_price, double time_till_maturity_years, string type,
		double interest, double div_yield, double starting_price);
	void print() {
		cout << "Option Properties ... " << endl 
			<< "Starting Price: " << s0 << endl 
			<< " Type: " << option_type << "/" << "Plain vanilla" << endl 
			<<"Volatility: " << sigma << endl 
			<< "Strike: " << strike << endl 
			<< "Maturity: " << maturity << " year(s) " << endl 
			<<"Interest Rate: " << interest_rate << endl 
			<< "Dividend Yield: " << dividend_yield << endl;

	}
	double ClosedForm();
	bool KnockedOut(double spot) { return false; } // Returns false because there are no knockout points for a plain vanilla option.



};

#endif // !PLAINVANILLA_H