// ComputationalFinance.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <chrono>
using std::string;
using std::cout;
using std::endl;
using std::string;
using std::setprecision;
using namespace std::chrono;

#include "OptionPricer.h"

inline double Euler_Method(double previous, double mesh, double A, double increment, double sigma);
inline double log_Euler_Method(double previous, double mesh, double A, double increment, double sigma);
inline double Milstein_Method(double previous, double mesh, double A, double increment, double sigma);
void testMethod(int*, OptionPricer, Option, string, double(*func)(double, double, double, double, double), double = -1);

int main()
{
	double volatility, strike_price, time_till_maturity_years;
	string type, contract;
	double interest, div_yield, starting_price;

	volatility=0.3;
	strike_price=110;
	time_till_maturity_years=1.45;
	type="call";
	contract="up and out";
	//contract = "plain vanilla";
	interest=0.05;
	div_yield= 0.02;
	starting_price= 100.0;
	
	Option option1(volatility,strike_price,time_till_maturity_years,type,contract,interest,div_yield,starting_price);
	option1.setUpperBarrier(120);
	Option options[] = { option1 };
	
	int size = 1; //of option array

	OptionPricer pricer;
	pricer.setMesh(1.0 / 365);
	double truth = 0;

	for (int i = 0; i < size; ++i) {
		int paths[4] = { 2500 , 5000, 7500, 10000 };
		double * results, var;
		Option OPTION = options[i];
		OPTION.print();
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*CLOSED FORM*/
		//Start function
		auto start = std::chrono::system_clock::now();

		truth = pricer.up_and_out_closed_form(OPTION);

		//Function End
		auto end = std::chrono::system_clock::now();

		//Calculate runtime
		std::chrono::duration<double> duration = end - start;

		cout << "Option price using closed-form formula = " << truth << ", runtime = " << duration.count() << "seconds" << endl;
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*NUMERICAL APPROXIMATION (EULER)*/
		testMethod(paths, pricer, OPTION, "Euler Method", Euler_Method, truth);
		/////////////////////////////////////////////////////////////////////////////////////////////////////


		/*ANTITHETIC NUMERICAL APPROXIMATION (EULER)*/
		testMethod(paths, pricer, OPTION, "Antithetic Euler Method", Euler_Method, truth);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*CONTROL VARIATE TECHNIQUE (EULER with Random walk as control)*/
		testMethod(paths, pricer, OPTION, "Control Variate Method", Euler_Method, truth);
	}

	return 0;

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
	return previous + (A-0.5*sigma*sigma)*mesh + sigma*increment; // apply Euler's method	
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
void testMethod(int* paths,OptionPricer pricer,Option OPTION, string method, double (*func)(double,double,double,double,double),double truth) {
	double *results, curr, var,UB,LB,runningSum=0,avg_err;
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
		cout << "Option price using "<< method << "=" << curr << "; CI=[" << LB << "," << UB << "]" <<
			endl << "variance = " << setprecision(10) << var << ", runtime = " << duration.count() << " seconds" << endl;
		runningSum += var;
	}
	avg_err = (1 / 3.0)*runningSum;
	cout << "Average Variance = " << avg_err << endl;
	runningSum = 0;
	cout << endl;
}