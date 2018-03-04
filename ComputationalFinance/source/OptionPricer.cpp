#include "stdafx.h"
#include "OptionPricer.h"
#include <cmath>
#include <iostream>
using std::cout;

#include "Probability.h"
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

double OptionPricer::analytical_solution(Option opt) {
	Probability p;
	double scale = 1.0 / paths;
	double A = opt.s_naught*exp(opt.interest_rate - opt.dividend_yield -0.5*opt.sigma*opt.sigma);
	double B = opt.sigma*sqrt(opt.maturity);
	double C = exp(-opt.interest_rate*opt.maturity);
	double sum = 0;
	double S_jT = 0;

	
	for (unsigned long j = 0; j < paths; ++j) {
		S_jT = A*exp(B*rng->GetOneGaussianRandom());
		sum+=(opt)(S_jT);
	}

	return C*scale * sum;
}

double* OptionPricer::antithetic_numerical_Method(Option opt, double(*method)(double previous, double mesh, double A, double increment, double sigma),double truth) {
	Probability probability; //allows probability functions
	double S_i = opt.s_naught, S_i_t = opt.s_naught; // S_i is asset price, S_i_t is the antithetic path
	double B = exp(-opt.interest_rate*opt.maturity); //e^(-rT)
	double bound = opt.maturity / mesh; //T/h=N
	double sum = 0, scale = 0.5/ paths, err_sum = 0.0; //running sums and scaling constants
	double A = opt.interest_rate - opt.dividend_yield, Z =0.0; //r-d, Z~Normal(0,1)
	double v1=0, v2=0, vary1 =0,vary2=0, covy1y2 = 0;
	bool stop1 = false, stop2 = false;
	int count = 0; //for the while loop
	while (count < paths) {
		/*PLAIN VANILLA (will update antithetic version another time)*/
		if (opt.contract_type == "plain vanilla") {
			for (unsigned long i = 0; i < bound; ++i) {
				Z = probability.GetOneGaussianByBoxMuller(rng);

				//If one of the options still hasn't expired...
				S_i = (*method)(S_i, mesh, A, root_mesh*Z, opt.sigma); //apply numerical method
				S_i_t = (*method)(S_i_t, mesh, A, -root_mesh*Z, opt.sigma);

			}
			
		}
		/*UP AND OUT*/
		else if (opt.contract_type == "up and out") {
			for (unsigned long i = 0; i < bound; ++i) {
				Z = probability.GetOneGaussianByBoxMuller(rng);

				//If one of the options still hasn't expired...
				if (!stop1) S_i = (*method)(S_i, mesh, A, root_mesh*Z, opt.sigma); //apply numerical method
				if (!stop2) S_i_t = (*method)(S_i_t, mesh, A, -root_mesh*Z, opt.sigma); 
				
				if (S_i >= opt.up) { stop1 = true; S_i = opt.strike; };  //underlying price crossed barrier, option expires
				if (S_i_t >= opt.up) { stop2 = true; S_i_t = opt.strike; };
				if (stop1&&stop2) break; //break loop
					
			}
			//calculates payoff
			
			
			stop1 = false;
			stop2 = false;
		}
		/*UNDEFINED CONTRACT*/
		else {
			cout << "Contract type " << opt.contract_type << "is not defined. Review Payoff.h file." << endl;
		}
		v1 = (opt)(S_i);
		v2 = (opt)(S_i_t);
		vary1 += (v1 - truth)*(v1 - truth);
		vary2 += (v2 - truth)*(v2 - truth);
		covy1y2 += (v1 - truth)*(v2 - truth);
		sum += v1 + v2;
		v1 = 0;
		v2 = 0;
		S_i = opt.s_naught;
		S_i_t = opt.s_naught;
		count++;
	}
	double results[2];
	results[0] = B*scale*sum;
	results[1] = (1.0 / (paths - 1))*0.25*(vary1 + vary2 + 2*covy1y2);
	return results;
};

/*
Returns the closed form solution to price a European plain vanilla call or put option using
the Black-Scholes-(Merton) model.

@param opt Option that is being worked on.
*/

double OptionPricer::black_Scholes(Option opt) {
	Probability probability;
	double a = (opt.sigma*sqrt(opt.maturity));
	double scale = 1.0 / a;
	double A = log(opt.s_naught / opt.strike);
	double B = opt.interest_rate + opt.sigma*opt.sigma * 0.5;

	double d1 = scale*(A + B*opt.maturity);
	double d2 = d1 - a;

	double N_d1 = probability.CumulativeNormal(d1);
	double N_d2 = probability.CumulativeNormal(d2);

	if (opt.option_type == "call")
		return N_d1*exp(-opt.dividend_yield*opt.maturity)*opt.s_naught - N_d2 * opt.strike * exp(-1 * opt.interest_rate*opt.maturity);
	else
		return probability.CumulativeNormal(-1 * d2) * opt.strike * exp(-1 * opt.interest_rate*opt.maturity)
		- probability.CumulativeNormal(-1 * d1)*exp(-opt.dividend_yield*opt.maturity)*opt.s_naught;
}

double* OptionPricer::control_variate(Option opt, double(*method)(double previous, double mesh, double A, double increment, double sigma),double truth) {
	Probability probability;
	double S_i = opt.s_naught; // S is asset price
	double B = exp(-opt.interest_rate*opt.maturity);	
	double bound = 1.0*opt.maturity / mesh;
	double sum = 0, scale = 1.0 / paths, increment = 0;
	double A = opt.interest_rate - opt.dividend_yield;
	double v = 0;

	double *y = new double[paths];
	double *x = new double[paths]; //control variate, E[random walk]= 0
	int count = 0;
	while (count < 1000) {
		/*PLAIN VANILLA*/
		if (opt.contract_type == "plain vanilla") {
			for (unsigned long i = 0; i < bound; ++i) {
				S_i = (*method)(S_i, mesh, A, root_mesh*probability.GetOneGaussianByBoxMuller(rng), opt.sigma); //apply numerical method
			}
			sum += (opt)(S_i); //calculates payoff
		}
		/*UP AND OUT*/
		else if (opt.contract_type == "up and out") {
			bool stop = false;
			for (unsigned long i = 0; i < bound; ++i) {
				S_i = (*method)(S_i, mesh, A, root_mesh*probability.GetOneGaussianByBoxMuller(rng), opt.sigma); //apply numerical method
				if (S_i >= opt.up) { 
					stop = true; 
					S_i = opt.strike;
					break;
				}; //underlying price crossed barrier, option expires
			}

			//initialize x and y arrays
			x[count] = probability.GetOneGaussianByBoxMuller(rng); //control variate sum
			v = (opt)(S_i);//calculates payoff
			sum += v;

			y[count] = v;

			v = 0;
			stop = false;
		}
		/*UNDEFINED CONTRACT*/
		else {
			cout << "Contract type " << opt.contract_type << "is not defined. Review Payoff.h file." << endl;
		}

		S_i = opt.s_naught;
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
		/*PLAIN VANILLA (will update antithetic version another time)*/
		if (opt.contract_type == "plain vanilla") {
			cout << "Plain vanilla numerical method accidentally deleted. Will re-implement later." << endl;
		}
		/*UP AND OUT*/
		else if (opt.contract_type == "up and out") {

			for (unsigned long i = 0; i < bound; ++i) {
				Z = probability.GetOneGaussianByBoxMuller(rng);
				//If the options hasn't expired...
				S_i = (*method)(S_i, mesh, A, root_mesh*Z, opt.sigma); //apply numerical method
				if (S_i >= opt.up) {
					stop1 = true; 
					break; 
				}  //underlying price crossed barrier, option expires
				
			}
			//calculates payoff
			Z = probability.GetOneGaussianByBoxMuller(rng);
			if (!stop1) v1 = (opt)(S_i);
			sum += v1;
			randomwalk += Z;
			e = (v1 - truth);
			err_sum += e*e;
			v1 = 0;
			stop1 = false;

		}
		/*UNDEFINED CONTRACT*/
		else {
			cout << "Contract type " << opt.contract_type << "is not defined. Review Payoff.h file." << endl;
		}

		S_i = opt.s_naught;
		count++;
	}
	double results[2];
	results[0] = B*(scale*sum-scale*b*randomwalk);
	results[1] = (1.0 / (paths - 1))*(1.0/varscale)*err_sum;
	
	return results;
};


double* OptionPricer::numerical_Method(Option opt, double(*method)(double previous, double mesh, double A, double increment, double sigma),double truth) {
	Probability probability; LEcuyer ecuyer;
	int count = 0; //for the while loop
	double sum = 0, increment = 0, err_sum = 0.0; //running sums and scaling constants
	double S_i = opt.s_naught,bound = opt.maturity/mesh,A=opt.interest_rate-opt.dividend_yield,Z = 0.0; //r-d, Z~Normal(0,1)
	double B=exp(-opt.interest_rate*opt.maturity),scale = 1.0/paths, v1 = 0, e = 0;
	bool stop1 = false;
	while (count < paths) {
		/*PLAIN VANILLA*/
		if (opt.contract_type == "plain vanilla") {
			for (unsigned long i = 0; i < bound; ++i) {
				Z = probability.GetOneGaussianByBoxMuller(rng);
				S_i = (*method)(S_i, mesh, A, root_mesh*Z, opt.sigma); //get S(T) estimate
			}
			v1 = opt(S_i);
			sum += v1;
			
			e = v1 - truth;
			err_sum = e*e;
			
		}
		/*UP AND OUT*/
		else if (opt.contract_type == "up and out") {

			for (unsigned long i = 0; i < bound; ++i) {
				Z = probability.GetOneGaussianByBoxMuller(rng);
				//If the options hasn't expired...
				S_i = (*method)(S_i, mesh, A, root_mesh*Z, opt.sigma); //apply numerical method
				if (S_i >= opt.up) { 
					stop1 = true;
					S_i = opt.strike;
					break;
				}  //underlying price crossed barrier, option expires
			}
			//calculates payoff
			v1 = (opt)(S_i);
			sum += v1;
			e = v1 - truth;
			err_sum += e*e;
			v1 = 0;
			stop1 = false;

		}
		/*UNDEFINED CONTRACT*/
		else {
			cout << "Contract type " << opt.contract_type << "is not defined. Review Payoff.h file." << endl;
		}

		S_i = opt.s_naught;
		count++;
	}
	double results[2];
	results[0] = B*scale*sum;
	results[1] = (1.0 / (paths - 1))*err_sum;

	return results;
}


double OptionPricer::up_and_out_closed_form(Option opt, double close, double t) {
	Probability p;
	struct maths {
		static double delta(double tau,double s, double r,double sigma, int pm) {
			if (pm >= 0)	return (1.0 / (sigma*sqrt(tau)))*(log(s)+(r+.5*sigma*sigma)*tau);
			else return (1.0 / (sigma*sqrt(tau)))*(log(s) + (r - .5*sigma*sigma)*tau);
		}
	};
	
	double years = opt.maturity;
	double cfsol, divScale = exp(-opt.dividend_yield*years);
	double ert = exp(-opt.interest_rate*opt.maturity), B = opt.up, sigma = opt.sigma;
	double x = (close > 0) ? x = close : x = opt.s_naught;
	double tau = t > 0 ? tau = opt.maturity - t : tau = years , r = opt.interest_rate-opt.dividend_yield,
		K = opt.strike;
	double A = (-2.0 * r )/(sigma*sigma);
	double i1 = p.CumulativeNormal(maths::delta(tau, x / K, r, sigma,1)) -
		p.CumulativeNormal(maths::delta(tau, x / B,r,sigma,1));
	double i2 = p.CumulativeNormal(maths::delta(tau, x / K, r, sigma, -1)) -
		p.CumulativeNormal(maths::delta(tau, x / B, r, sigma, -1));
	double i3 = p.CumulativeNormal(maths::delta(tau, B*B / (K*x), r, sigma, 1)) -
		p.CumulativeNormal(maths::delta(tau, B / x, r, sigma, 1));
	double i4 = p.CumulativeNormal(maths::delta(tau, B*B / (K*x), r, sigma, -1)) -
		p.CumulativeNormal(maths::delta(tau, B / x, r, sigma, -1));
	
	cfsol = x*divScale*i1 - ert*K*i2 - B*divScale*pow(x / B, A)*i3 
		+ ert*K*pow(x / B, A + 1)*i4;

	return cfsol;

}


