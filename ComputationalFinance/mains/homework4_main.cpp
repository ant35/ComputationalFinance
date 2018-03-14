#include "stdafx.h"
#include "homework4_main.h"


homework4_main::homework4_main()
{
}


homework4_main::~homework4_main()
{
}


void homework4_main::run() {
	double volatility, strike_price, time_till_maturity_years;
	string type, contract;
	double interest, div_yield, starting_price;

	volatility = 0.3;
	strike_price = 110;
	time_till_maturity_years = 1.0;
	type = "call";
	interest = 0.05;
	div_yield = 0.0;
	starting_price = 100.0;

	PlainVanilla plainvanilla;
	Option * option1 = &plainvanilla;
	option1->setVolatility(volatility);
	option1->setStrike(strike_price);
	option1->setMaturity(time_till_maturity_years);
	option1->setOptionType(type);
	option1->setInterestRate(interest);
	option1->setDividend(div_yield);
	option1->setS0(starting_price);

	double mesh = 1.0 / 252, paths = 10000;
	OptionPricer pricer;
	pricer.setMesh(mesh);
	pricer.setPaths(paths);
	double truth = 0;
	double * results;

	ParkMiller pm; // get park miller generator
	unsigned d = 1, n = paths; //set dimensions to 12
	Sobol sobol(n, d); //get sobol sequence
	
	pricer.setGenerator(&pm);
	option1->print();
	cout << "Black Scholes: " << option1->ClosedForm() << endl;
	results = pricer.analytical_solution(option1);
	cout << "MC for plain vanilla using Park Miller : " << results[0] << endl;
	pm.RestartSeed();
	cout << "Antithetic MC for plain vanilla using Park Miller: " << pricer.antithetic_analytical_solution(option1) << endl;
	pricer.setGenerator(&sobol);
	cout << "QMC for plain vanilla using Sobol sequence: " <<pricer.analytical_solution(option1)[0]<< endl;


}

void homework4_main::testSobol() {
	unsigned n = 500, d = 2;
	string file1 = "C:\\Users\\Hansa\\Documents\\Alex's Stuff\\sobol_sequence.txt";
	string file2 = "C:\\Users\\Hansa\\Documents\\Alex's Stuff\\park_miller.txt";
	Sobol sobol(n, d);
	ofstream myfile;
	myfile.open(file1);

	for (unsigned i = 0; i < n; i++) {
		myfile << sobol.GetOneUniformRandom() << "," << sobol.GetOneUniformRandom() << endl;
	}
	myfile.close();
	myfile.open(file2);

	ParkMiller pm;
	for (unsigned i = 0; i < 2*n; i++) {
		myfile << pm.GetOneUniformRandom() << "," << pm.GetOneUniformRandom() << endl;
	}
}