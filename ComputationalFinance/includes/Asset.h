#pragma once
#ifndef ASSET_H
#define ASSET_H

#include <iostream>
using std::string;

class Asset {
public:

	Asset()	{}

	~Asset() {}

	virtual void print() = 0;

protected:
		/*Risk free rate of the market.*/
	double interest_rate;

	/*Dividend yield of the asset.*/
	double dividend_yield;

	/*The starting value of the asset at time 0.*/
	double s_naught;

};

#endif 