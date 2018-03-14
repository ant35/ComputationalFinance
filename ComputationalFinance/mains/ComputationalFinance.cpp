// ComputationalFinance.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <new>

#include "homework4_main.h"

using std::string;
using std::cout;
using std::endl;
using std::string;
using std::setprecision;
using std::ofstream;
using namespace std::chrono;


int main()
{
	homework4_main::run();
	//for this method you actually need to switch the way you draw point from sobolArray (in Sobol.h)
	//to draw across rows and go down columns. For that reason it is commented out.
	//The results were used to generate the points for excel plots
	//homework4_main::testSobol(); 
	return 0;


}



