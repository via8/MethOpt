#include <iostream>	// cout
#include <fstream>	// ifstream
#include <iomanip>	// fixed, setprecision
#include <math.h>	// exp
#include "InputData.h"
#include "Function.h"
#include "Solver.h"
#include "SolverDichotomy.h"
#include "SolverFibonacci.h"

// define unimodal function here
double f(double x) {
	return exp(x / 2.0) * (x * x + 1.0);
}

int main(int argc, char const* argv[]) {
	char const* EXCEPTION_CMD_LINE_ARGS = "Input data filename expected as a command line argument";
	char const* EXCEPTION_FILE_OPEN		= "Unable to open input file";

	char const* helpMsg = "\n---HELP---\n\n"
		"Input file for one-dimensional optimization problem must include:\n"
		"-two float values: left and right borders of interval where optimal point is expected to be found\n"
		"-float value which will be interpreted as maximum allowed bk - ak interval length\n"
		"(in other words returned optimal point will belong to an interval of such length,\n"
		"from where it's (point) precision\n"
		"\n---START OF EXAMPLE---\n\n"
		"0.0 1.0\n"
		"1e-3\n"
		"\n---END OF EXAMPLE---\n";

	try {
		if (argc != 2)
			throw EXCEPTION_CMD_LINE_ARGS;

		std::ifstream input;
		input.open(argv[1], std::ifstream::in);
		if (!input.is_open())
			throw EXCEPTION_FILE_OPEN;

		// Dichotomy method
		InputData data(input);
		Function function(f);
		Solver* solver = new SolverDichotomy();
		double ansDichotomy = solver->solve(function, data);
		std::cout << std::fixed << std::setprecision(16);
		std::cout << "answer Dichotomy: " << ansDichotomy << '\n';
		std::cout << "calls  Dichotomy: " << function.getCallCount() << '\n';

		// Fibonacci method
		function.resetCallCount();
		delete solver;
		solver = new SolverFibonacci(20);
		double ansFibonacci = solver->solve(function, data);
		std::cout << "answer Fibonacci: " << ansFibonacci << '\n';
		std::cout << "calls  Fibonacci: " << function.getCallCount() << '\n';
	}
	catch (char const* errMsg) {
		std::cout << "ERROR: " << errMsg << '\n';

		if (errMsg != EXCEPTION_CMD_LINE_ARGS &&
			errMsg != EXCEPTION_FILE_OPEN) {
			std::cout << helpMsg << '\n';
		}

		return -1;
	}

	return 0;
}