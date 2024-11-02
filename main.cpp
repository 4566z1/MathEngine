#include "MathEngine.hpp"
using ME::MathEngine;

int main()
{
	MathEngine math_engine(L"Math Engine");

	math_engine.loop();
	return 0;
}