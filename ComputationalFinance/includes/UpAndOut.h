#pragma once
#ifndef UPANDOUT_H
#define UPANDOUT_H
#include "Option.h"
class UpAndOut :
	public Option
{
public:
	UpAndOut();
	~UpAndOut();
	double ClosedForm();
	bool KnockedOut(double spot);
	void print() {
		cout << "Option Properties ... " << endl
			<< "Starting Price: " << s0 << endl
			<< " Type: " << option_type << "/" << "Up and Out" << endl
			<< "Volatility: " << sigma << endl
			<< "Strike: " << strike << endl
			<< "Maturity: " << maturity << " year(s) " << endl
			<< "Interest Rate: " << interest_rate << endl
			<< "Dividend Yield: " << dividend_yield << endl;
	}
	inline void setLowerBarrier(double l) { low = l; }
	inline void setUpperBarrier(double u) { up = u; }
	

protected:
	/*Lower barrier for barrier options*/
	double low;

	/*Upper barrier for barrier options*/
	double up;
};

#endif // !UPANDOUT_H