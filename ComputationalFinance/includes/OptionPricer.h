#pragma once
#ifndef OPTIONPRICER_H

#define OPTIONPRICER_H
#include "Option.h"
#include "Generator.h"
#include "StochasticNumericalMethod.h"
class OptionPricer
{
public:
	OptionPricer(); //Default constructor.
	OptionPricer(long, double);
	~OptionPricer();

	/*
	Returns the analytical solution, S(T), to dS(t) = S(t)((r-d)dt + dW(t)) and Monte Carlo
	simulation to compute the prices for a European-style call and put. Does NOT work for look back options.

	@param opt Option to be analyzed

	*/
	double analytical_solution(Option * opt);
	/*Provides antithetic MC adaptation to OptionPricer::numerical_Method. Will possibly deprecate numerical_Method in future,
	so I made it into its own function.*/
	double* antithetic_numerical_Method(Option * opt, StochasticNumericalMethod * snm);
	double* control_variate(Option * opt, StochasticNumericalMethod * snm, double truth = -1);

	/*
		This function generates a Brownian motion W and feeds it into an iterative numerical method for approximating an SDE.

		@param Option object to refer to option being worked on
		@param snm Pointer to a StochasticNumericalMethod object that implements a numerical method

	*/
	double* numerical_Method(Option * opt, StochasticNumericalMethod * snm);
	
	/*Set number of paths to be generated in MC simulation.*/
	inline void setPaths(long p) { paths = p; };
	inline void setMesh(double m) { mesh = m; setRootMesh(); };
	inline void setGenerator(Generator * generator) { rng = generator; };

private:
	inline void setRootMesh() { root_mesh = sqrt(mesh); }
	/*Number of paths to be generated in MC simulation.*/
	long paths;
	/*Mesh size to partition in numerical approximation.*/
	double mesh;
	/*Square root of the mesh*/
	double root_mesh;
	/*Random number generator for approximations*/
	Generator * rng;
};
#endif // !P1B_H