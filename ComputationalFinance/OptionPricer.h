#pragma once
#ifndef OPTIONPRICER_H

#define OPTIONPRICER_H
#include "Option.h"

class OptionPricer
{
public:
	OptionPricer(); //Default constructor.
	OptionPricer(long, double);
	~OptionPricer();

	/*
	Returns the analytical solution, S(T), to dS(t) = S(t)((r-d)dt + dW(t)) and Monte Carlo
	simulation to compute the prices for a European-style call and put. Does NOT work for look back options.

	@param iterations Number of paths to generate for MC.
	@param opt Option to be analyzed

	*/
	double analytical_solution(Option opt);
	/*Provides antithetic MC adaptation to OptionPricer::numerical_Method. Will possibly deprecate numerical_Method in future,
	so I made it into its own function.*/
	double* antithetic_numerical_Method(Option opt, double(*method)(double previous, double mesh, double A, double increment, double sigma),double truth = -1.0);

	/*
	Returns the closed form solution to price a European call or put option using
	the Black-Scholes-(Merton) model.

	@param Option option to be analyzed

	*/
	double black_Scholes(Option);

	double* control_variate(Option opt, double(*method)(double previous, double mesh, double A, double increment, double sigma),double truth = -1);

	/*
		This function generates a Brownian motion W and feeds it into an iterative numerical method for approximating an SDE.

		@param Option object to refer to option being worked on
		@param method Pointer to a function that implements a numerical method

		To use this function it is necessary to implement a function handle for the numerical method your using in the .cpp
		file.

	*/
	double* numerical_Method(Option opt, double (*method)(double previous, double mesh, double A, double increment, double sigma),double truth = -1);
	/*Set number of paths to be generated in MC simulation.*/
	inline void setPaths(long p) { paths = p; };
	/*Set mesh size to partition in numerical approximation.*/
	inline void setMesh(double m) { mesh = m; };
	
	/*
	Calculates closed form solution of up and out barrier option, assuming it doesn't cross the barrier.
	The formula for the closed form solution is taken from Shreve II (7.3.19).
	@param Option object to represent the derivative*/
	double up_and_out_closed_form(Option, double = -1, double = -1);
private:
	inline void setRootMesh(double rm) { root_mesh = rm; }
	/*Number of paths to be generated in MC simulation.*/
	long paths;
	/*Mesh size to partition in numerical approximation.*/
	double mesh;
	/*Square root of the mesh*/
	double root_mesh;
};
#endif // !P1B_H