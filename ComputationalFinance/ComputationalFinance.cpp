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
#include "OptionPricer.h"
#include "LEcuyer.h"
#include "ParkMiller.h"
#include "Fishman.h"
#include "BoxMullerMarsaglia.h"


inline double Euler_Method(double previous, double mesh, double A, double increment, double sigma);
inline double log_Euler_Method(double previous, double mesh, double A, double increment, double sigma);
inline double Milstein_Method(double previous, double mesh, double A, double increment, double sigma);
void record2bData(unsigned long iterations);
void testMethod(int*, OptionPricer, Option, string, double(*func)(double, double, double, double, double), double = -1);

int main()
{
	record2bData(1000);
	
	double volatility, strike_price, time_till_maturity_years;
	string type, contract;
	double interest, div_yield, starting_price;

	volatility = 0.3;
	strike_price = 110;
	time_till_maturity_years = 1.0;
	type = "call";
	//contract = "up and out";
	contract = "plain vanilla";
	interest = 0.05;
	div_yield = 0.0;
	starting_price = 100.0;

	Option option1(volatility, strike_price, time_till_maturity_years, type, contract, interest, div_yield, starting_price);
	option1.setUpperBarrier(120);
	Option options[] = { option1 };

	int size = 1; //of option array

	OptionPricer pricer;
	pricer.setMesh(1.0 / 365);
	pricer.setPaths(10000);
	double truth = 0;
	
	Generator *ecuyer = new LEcuyer;
	Generator *parkmiller = new ParkMiller;
	Generator *e2 = new LEcuyer;
	Generator *p2 = new ParkMiller;
	Generator *fishmanEcuyer = new Fishman(e2);
	Generator *fishmanParkmiller = new Fishman(p2);
	Generator *p3 = new ParkMiller;
	Generator *box = new BoxMullerMarsaglia(p3);
	pricer.setPaths(100000);
	/*1a*/
	cout << "100000 paths: " << endl;
	cout << "BS = " << pricer.black_Scholes(option1) << endl;
	pricer.setGenerator(ecuyer);
	cout << "MC with L'Ecuyer = " << pricer.analytical_solution(option1) << endl;
	pricer.setGenerator(parkmiller);
	cout << "MC with Park Miller = " << pricer.analytical_solution(option1) << endl;
	

	/*1b*/
	pricer.setPaths(100000);
	parkmiller->RestartSeed();
	cout << "\n100000 paths: " << endl;
	pricer.setGenerator(parkmiller);
	cout << "MC with inverse normal (park miller) = " << pricer.analytical_solution(option1) << endl;
	pricer.setGenerator(box);
	cout << "MC with Box Muller = " << pricer.analytical_solution(option1) << endl;
	pricer.setGenerator(fishmanParkmiller);
	cout << "MC with Fisherman normal generator = " << pricer.analytical_solution(option1) << endl;

	
	
	return 0;

	/*2*/

}

/*
Generates next iteration in a euler method approximation to the SDE dS(t) = S(t)((r-d)dt + sigma*dW(t)).
@param previous Last iteration value
@param mesh Step size
@param A := a(X(t))= interest rate - dividend yield
@param increment sqrt(t_{i+1} - t_i) * Z
*/
double Euler_Method(double previous, double mesh, double A, double increment, double sigma) {
	return previous*(1 + A*mesh + sigma*increment); // apply Euler's method
}

/*
Generates next iteration in a euler method approximation to a stochastic differential equation.
@param previous Last iteration value
@param mesh Step size
@param A := a(X(t))= interest rate - dividend yield
@param increment sqrt(t_{i+1} - t_i) * Z
*/
double log_Euler_Method(double previous, double mesh, double A, double increment, double sigma) {
	return previous + (A - 0.5*sigma*sigma)*mesh + sigma*increment; // apply Euler's method	
}

/*
Generates next iteration in a Milstein method approximation to a stochastic differential equation.
@param previous Last iteration value
@param mesh Step size
@param A := a(X(t)) = interest rate - dividend yield
@param increment sqrt(t_{i+1} - t_i) * Z
*/
double Milstein_Method(double previous, double mesh, double A, double increment, double sigma) {
	return previous*(1 + A*mesh + sigma*increment + 0.5*sigma*sigma*(increment*increment - mesh));
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
void testMethod(int* paths, OptionPricer pricer, Option OPTION, string method, double(*func)(double, double, double, double, double), double truth) {
	double *results, curr, var, UB, LB, runningSum = 0, avg_err;
	for (unsigned i = 0; i < 4; ++i) {
		cout << "Paths: " << paths[i] << "->";
		pricer.setPaths(paths[i]);

		//Start function
		auto start = std::chrono::system_clock::now();

		if (method == "Antithetic Euler Method") results = pricer.antithetic_numerical_Method(OPTION, func, truth);
		else if (method == "Control Variate Method") results = pricer.control_variate(OPTION, func, truth);
		else results = pricer.numerical_Method(OPTION, func, truth);
		curr = results[0]; //mean
		var = results[1]; //variance

						  //Function End
		auto end = std::chrono::system_clock::now();

		//Calculate runtime
		std::chrono::duration<double> duration = end - start;
		UB = curr + 1.96*sqrt(var / paths[i]);
		LB = curr - 1.96*sqrt(var / paths[i]);
		cout << "Option price using " << method << "=" << curr << "; CI=[" << LB << "," << UB << "]" <<
			endl << "variance = " << setprecision(10) << var << ", runtime = " << duration.count() << " seconds" << endl;
		runningSum += var;
	}
	avg_err = (1 / 3.0)*runningSum;
	cout << "Average Variance = " << avg_err << endl;
	runningSum = 0;
	cout << endl;
}