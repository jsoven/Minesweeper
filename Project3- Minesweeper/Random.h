#pragma once
#include <random>
using namespace std;
class Random
{
	//One instance of variable if static, all instances share ONE variable
	//static variables/functions exists for program lifetime
	//"this->" doesn't exists in static territory
	static std::mt19937 random;
public:

	//One and only one of this function
	static int Int(int min, int max);
	static float Float(float min, float max);
	
};

