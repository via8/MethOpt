#include <iostream>	// cout
#include <fstream>	// ifstream
#include <iomanip>	// fixed, setprecision
#include <math.h>	// sqrt
#include "EigenHelper.hpp"
#include "Function.h"
#include "GradientMethod1stOrder.hpp"
#include "GradientMethod2ndOrder.hpp"

#define A_PARAM 4.0
#define B_PARAM 6.0
#define C_PARAM 3.0

// define target function here
double f(VectorDouble x) {
	return A_PARAM * x[0] + x[1] + 4.0 * sqrt(1.0 + B_PARAM * x[0] * x[0] + C_PARAM * x[1] * x[1]);
}

// define target function gradient here
VectorDouble gradf(VectorDouble x) {
	VectorDouble res = VectorDouble(2);
	res[0] = A_PARAM + 4.0 * B_PARAM * x[0] / sqrt(1.0 + B_PARAM * x[0] * x[0] + C_PARAM * x[1] * x[1]);	// gradf(x)[0]
	res[1] =	 1.0 + 4.0 * C_PARAM * x[1] / sqrt(1.0 + B_PARAM * x[0] * x[0] + C_PARAM * x[1] * x[1]);	// gradf(x)[1]
	return res;
}

int main(int argc, char const* argv[]) {
	//char const* EXCEPTION_CMD_LINE_ARGS = "Input data filename expected as a command line argument";
	//char const* EXCEPTION_FILE_OPEN = "Unable to open input file";

	//char const* helpMsg = "\n---HELP---\n\n"
	//	"\n---START OF EXAMPLE---\n\n"
	//	"\n---END OF EXAMPLE---\n";

	try {
		//if (argc != 2)
		//	throw EXCEPTION_CMD_LINE_ARGS;

		//std::ifstream input;
		//input.open(argv[1], std::ifstream::in);
		//if (!input.is_open())
		//	throw EXCEPTION_FILE_OPEN;

		Function<VectorDouble, double> function(f);
		Function<VectorDouble, VectorDouble> gradfunction(gradf);
		GradientMethod* method1 = new GradientMethod1stOrder();
		VectorDouble initialPoint(2);
		initialPoint[0] = 3.0;
		initialPoint[1] = 3.0;
		VectorDouble ans = method1->solve(function, gradfunction, initialPoint, 1e-3);
		std::cout << ans.transpose() << '\n';
		std::cout << function.getCallCount() << '\n';

		system("python ../plot/plot.py");
	}
	catch (char const* errMsg) {
		std::cout << "ERROR: " << errMsg << '\n';

		//if (errMsg != EXCEPTION_CMD_LINE_ARGS &&
		//	errMsg != EXCEPTION_FILE_OPEN) {
		//	std::cout << helpMsg << '\n';
		//}

		return -1;
	}

	return 0;
}