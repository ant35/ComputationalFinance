#pragma once
#ifndef HOMEWORK4_MAIN_H
#define HOMEWORK4_MAIN_H

#include "MainDependencies.h"
class homework4_main :
	public MainDependencies
{
public:
	homework4_main();
	~homework4_main();
	static void run();
	static void testSobol();
	
};

#endif // !HOMEWORK4_MAIN_H