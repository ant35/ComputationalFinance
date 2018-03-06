#include "stdafx.h"
#include "OptionPricer.h"
#include <cmath>
#include <iostream>
using std::cout;

#include "Option.h"
#include "LEcuyer.h"
#include "ParkMiller.h"
#include "Fishman.h"


/*Default path # and mesh size set to 10000 and 1/252 respectively.*/
OptionPricer::OptionPricer() {
	setPaths(10000);
	setMesh(1.0 / 252);
	setRootMesh(sqrt(1.0 / 252));

};
/*
 @param number_of_paths Number of sample paths to be generated in the monte carlo experiment
 @param mesh_size Size of mesh to partition the beginning to maturity of the option life.
*/
OptionPricer::OptionPricer(long number_of_paths, double mesh_size) {
	setPaths(number_of_paths);
	setMesh(mesh_size);
	setRootMesh(sqrt(mesh_size));
}

OptionPricer::~OptionPricer()
{
}

double OptionPricer::analytical_solution(Option * opt) {
	double scale = 1.0 / paths;
	double A = opt->s0*exp(opt->interest_rate - opt->dividend_yield -0.5*opt->sigma*opt->sigma);
	double B = opt->sigma*sqrt(opt->maturity);
	double C = exp(-opt->interest_rate*opt->maturity);
	double sum = 0;
	double S_jT = 0;

	
	for (unsigned long j = 0; j < paths; ++j) {
		S_jT = A*exp(B*rng->GetOneGaussianRandom());
		sum += opt->Payoff(S_jT);
	}

	return C*scale * sum;
}

double* OptionPricer::antithetic_numerical_Method(Option * opt,StochasticNumericalMethod * snm, double truth) {
	double S_i = opt->s0, S_i_t = opt->s0; // S_i is asset price, S_i_t is the antithetic path
	double B = exp(-opt->interest_rate*opt->maturity); //e^(-rT)
	double bound = opt->maturity / mesh; //T/h=N
	double v1=0, v2=0, vary1 =0,vary2=0, covy1y2 = 0, Z =0.0;
	bool stop1 = false, stop2 = false;

	int count = 0; //for the while loop
	double sum = 0, scale = 0.5 / paths;

	while (count < paths) {
		for (unsigned long i = 0; i < bound; ++i) {
			Z = rng->GetOneGaussianRandom();
				
			snm->setdW(sqrt(mesh)*Z);
			if (!stop1) S_i = snm->OneStepAhead(S_i); //apply numerical method
			snm->setdW(-sqrt(mesh)*Z); //set for opposite path
			if (!stop2) S_i_t = snm->OneStepAhead(S_i_t); 
				
			if (opt->KnockedOut(S_i)) { stop1 = true; S_i = opt->strike; };  //underlying price crossed barrier, option expires
			if (opt->KnockedOut(S_i_t)) { stop2 = true; S_i_t = opt->strike; };
			if (stop1&&stop2) break; //break loop	
		}
		//calculates payoff
			
			
		stop1 = false;
		stop2 = false;
	
		v1 = opt->Payoff(S_i);
		v2 = opt->Payoff(S_i_t);
		vary1 += (v1 - truth)*(v1 - truth);
		vary2 += (v2 - truth)*(v2 - truth);
		covy1y2 += (v1 - truth)*(v2 - truth);
		sum += v1 + v2;
		v1 = 0;
		v2 = 0;
		S_i = opt->s0;
		S_i_t = opt->s0;
		count++;
	}
	double results[2];
	results[0] = B*scale*sum;
	results[1] = (1.0 / (paths - 1))*0.25*(vary1 + vary2 + 2*covy1y2);
	return results;
};



double* OptionPricer::control_variate(Option * opt, StochasticNumericalMethod * snm,double truth) {
	double S_i = opt->s0; // S is asset price
	double B = exp(-opt->interest_rate*opt->maturity);
	double bound = 1.0*opt->maturity / mesh;
	double sum = 0, scale = 1.0 / paths, increment = 0;
	double A = opt->interest_rate - opt->dividend_yield;
	double v = 0;

	double *y = new double[paths];
	double *x = new double[paths]; //control variate, E[random walk]= 0
	int count = 0;
	while (count < 1000) {		
		for (unsigned long i = 0; i < bound; ++i) {
			snm->setdW(sqrt(mesh)*rng->GetOneGaussianRandom());
			S_i = snm->OneStepAhead(S_i); //apply numerical method
			if (opt->KnockedOut(S_i)) break; //check if option is knocked out at current spot
		}
		sum += opt->Payoff(S_i); //calculates payoff

		//initialize x and y arrays
		x[count] = rng->GetOneGaussianRandom(); //control variate sum
		v = opt->Payoff(S_i);//calculates payoff
		sum += v;

		y[count] = v;
		v = 0;

		S_i = opt->s0;
		count++;
	}

	/*Calculate optimal b*/
	double Ey = B*(1.0/1000)*sum;
	double yerr = 0.0,yerr2=0.0;
	double xerr = 0.0, xerr2 = 0.0;
	double Exy = 0.0;

	for (long i = 0; i < paths; ++i) {
		yerr += (y[i] - Ey);
		yerr2 += (y[i] - Ey)*(y[i] - Ey);
		xerr += x[i];
		xerr2 += x[i] * x[i];
		Exy = (y[i] - Ey)*(x[i]);
	}
	//Calculate b and rho
	double b = (1.0*Exy) / xerr2;
	double rho = Exy / (sqrt(xerr2*yerr2)), rho2 = rho*rho, varscale = 1 - rho2;

	//Calculate estimate
	count = 0;
	bool stop1 = false;
	double err_sum = 0.0, Z = 0, v1 = 0, e = 0, t = 0, randomwalk = 0;
	while (count < paths) {
		for (unsigned long i = 0; i < bound; ++i) {
			Z = rng->GetOneGaussianRandom();
			snm->setdW(sqrt(mesh)*Z);
			S_i = snm->OneStepAhead(S_i); //apply numerical method
			if (opt->KnockedOut(S_i)) break; //check if option is knocked out at current spot
				
		}
		//calculates payoff
		Z = rng->GetOneGaussianRandom();
		randomwalk += Z;
			
		v1 = opt->Payoff(S_i);
		sum += v1;
			
		e = (v1 - truth);
		err_sum += e*e;
		v1 = 0;
			
		S_i = opt->s0;
		count++;
	}
	double results[2];
	results[0] = B*(scale*sum-scale*b*randomwalk);
	results[1] = (1.0 / (paths - 1))*(1.0/varscale)*err_sum;
	
	return results;
};


double* OptionPricer::numerical_Method(Option * opt, StochasticNumericalMethod * snm, double truth) {
	int count = 0; //for the while loop
	double sum = 0, err_sum = 0.0; //running sums
	double S_i = opt->s0, bound = opt->maturity / mesh, v1 = 0, e = 0;
	double B=exp(-opt->interest_rate*opt->maturity),scale = 1.0/paths;

	while (count < paths) {
		for (unsigned long i = 0; i < bound; ++i) {
			snm->setdW(sqrt(mesh)*rng->GetOneGaussianRandom());
			S_i = snm->OneStepAhead(S_i); //get S(T) estimate
			if (opt->KnockedOut(S_i)) { S_i = opt->strike; break; }; //check if knocked out
		}
		v1 = opt->Payoff(S_i);
		sum += v1;
			
		e = v1 - truth;
		err_sum = e*e;
				
		S_i = opt->s0;
		count++;
	}
	double results[2];
	results[0] = B*scale*sum;
	results[1] = (1.0 / (paths - 1))*err_sum;

	return results;
}

