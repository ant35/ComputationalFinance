#pragma once
#ifndef ASSET_H
#define ASSET_H
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

class Asset {
public:

	Asset()	{}

	~Asset() {}

	virtual void print() = 0;
	inline void setInterestRate(double ir) { interest_rate = ir; };
	inline void setDividend(double div) { dividend_yield = div; };
	inline void setS0(double s) { s0 = s; };

protected:
	/*Risk free rate of the market.*/
	double interest_rate;

	/*Dividend yield of the asset.*/
	double dividend_yield;

	/*The starting value of the asset at time 0.*/
	double s0;

};

#endif 