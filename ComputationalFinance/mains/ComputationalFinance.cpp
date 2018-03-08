// ComputationalFinance.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <new>


using std::string;
using std::cout;
using std::endl;
using std::string;
using std::setprecision;
using std::ofstream;
using namespace std::chrono;

#include "Probability.h"
#include "PlainVanilla.h"
#include "UpAndOut.h"
#include "OptionPricer.h"
#include "LEcuyer.h"
#include "ParkMiller.h"
#include "Fishman.h"
#include "BoxMullerMarsaglia.h"
#include "EulerMaruyama.h"


void record2bData(unsigned long iterations);
void confidenceInterval(double mean, double variance);
double afunc(double prev) {
	return prev*(0.05-0.02);
}
double bfunc(double prev) {
	return prev*0.3;
}
int main()
{
	//record2bData(1000);
	
	double volatility, strike_price, time_till_maturity_years;
	string type, contract;
	double interest, div_yield, starting_price;
	
	volatility = 0.3;
	strike_price = 110;
	time_till_maturity_years = 1.0;
	type = "call";
	interest = 0.05;
	div_yield = 0.02;
	starting_price = 100.0;
	
	UpAndOut upandout;
	Option * option1 = &upandout;
	option1->setVolatility(volatility);
	option1->setStrike(strike_price);
	option1->setMaturity(time_till_maturity_years);
	option1->setOptionType(type);
	option1->setInterestRate(interest);
	option1->setDividend(div_yield);
	option1->setS0(starting_price);

	upandout.setUpperBarrier(120);
	double mesh = 1.0 / 365, paths = 100000;
	OptionPricer pricer;
	pricer.setMesh(mesh); 
	pricer.setPaths(paths);
	double truth = 0;
	
	Generator *ecuyer = new LEcuyer;
	StochasticNumericalMethod * snm = new EulerMaruyama;
	snm->setMesh(mesh);
	snm->setAFunc(afunc);
	snm->setBFunc(bfunc);

	truth = upandout.ClosedForm();
	cout << paths <<" paths: " << endl;
	cout << "BS = " << truth << endl;
	pricer.setGenerator(ecuyer);
	double * results = pricer.numerical_Method(option1, snm);
	double m = results[0], v = results[1];
	cout << "MC with L'Ecuyer = " << m << endl;
	cout << "Variance = " << v << endl;
	confidenceInterval(m, v); //95 % normal distribution
	delete[] results;
	
	paths = paths/2.0;
	pricer.setPaths(paths);
	cout << endl;
	cout << paths << " paths: " << endl;
	ecuyer->RestartSeed();
    results = pricer.antithetic_numerical_Method(option1, snm);
	m = results[0], v = results[1];
	cout << "AV with L'Ecuyer = " << m << endl;
	cout << "Variance = " << v << endl;
	confidenceInterval(m, v); //95 % normal distribution
	delete[] results;
	results = NULL;
	
	return 0;


}







/*
*Records the output for Box Muller, inverse transform, and Fishman
*@param rng either ecuyer or parkmiller
*@param iterations number of data to generate
*/
void record2bData(unsigned long iterations) {
	ofstream file; 
	LEcuyer ecuyer; ParkMiller parkmiller;
	LEcuyer * e = new LEcuyer; ParkMiller * p = new ParkMiller; LEcuyer* e2 = new LEcuyer; ParkMiller* p2 = new ParkMiller;
	BoxMullerMarsaglia boxEcuyer(e);
	BoxMullerMarsaglia boxPark(p);
	Fishman fishEcuyer(e2);
	Fishman fishPark(p2);

	file.open("C:\\Users\\Hansa\\Documents\\cfhw3\\2bdata_parkmiller.txt");
	file << "Inverse Transform (PM) , Box Muller (PM) , Fishman (PM)" << endl;
	for (unsigned long i = 0; i < iterations; ++i) {
		file << parkmiller.GetOneGaussianRandom() << "," << 
			boxPark.GetOneGaussianRandom() << "," << 
			fishPark.GetOneGaussianRandom() << "," <<  endl;
	}
	file.close();

	file.open("C:\\Users\\Hansa\\Documents\\cfhw3\\2bdata_ecuyer.txt");
	file << "Inverse Transform (EC) , Box Muller (EC) , Fishman (EC)" << endl;
	for (unsigned long i = 0; i < iterations; ++i) {
		file << ecuyer.GetOneGaussianRandom() << "," << 
			boxEcuyer.GetOneGaussianRandom() << "," << 
			fishEcuyer.GetOneGaussianRandom() << endl;
	}
	file.close();
}

void confidenceInterval(double mean, double variance) {
	cout << "(" << mean - 1.96*sqrt(variance) / 100.0 << "," << mean + 1.96*sqrt(variance) / 100.0 << ")" << endl;
}
