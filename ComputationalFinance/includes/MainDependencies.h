#pragma once
#ifndef MAINDEPENDENCIES_H
#define MAINDEPENDENCIES_H
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
#include "Sobol.h"




class MainDependencies
{
public:

	MainDependencies()
	{
	}

	~MainDependencies()
	{
	}

	void confidenceInterval(double mean, double variance);
};

#endif // !MAINDEPENDENCIES_H