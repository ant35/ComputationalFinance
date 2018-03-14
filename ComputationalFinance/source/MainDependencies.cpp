#include "stdafx.h"
#include "MainDependencies.h"

void MainDependencies::confidenceInterval(double mean, double variance) {
	cout << "(" << mean - 1.96*sqrt(variance) / 100.0 << "," << mean + 1.96*sqrt(variance) / 100.0 << ")" << endl;
}