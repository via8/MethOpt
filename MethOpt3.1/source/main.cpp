#include <iostream>	// cout
#include <fstream>	// ifstream
#include <iomanip>	// fixed, setprecision
#include <math.h>	// sqrt
#include <fstream>  // ifstream, ofstream
#include "GradientInput.hpp"
#include "GradientOutput.hpp"
#include "EigenHelper.hpp"
#include "Function.h"
#include "GradientMethod1stOrder.hpp"
#include "GradientMethod2ndOrder.hpp"

#define A_PARAM 4.0
#define B_PARAM 6.0
#define C_PARAM 3.0

/* GRADIENT METHOD INITIAL DATA */

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

/* END OF INITIAL DATA */

int main(int argc, char const* argv[]) {
	char const* EXCEPTION_CMD_LINE_ARGS = "Input data filename expected as a command line argument";
	char const* EXCEPTION_FILE_OPEN = "Unable to open input file";

	char const* helpMsg = "\n---HELP---\n\n"
		"\n---START OF EXAMPLE---\n\n"
		"\n---END OF EXAMPLE---\n";

	try {
		if (argc != 2)
			throw EXCEPTION_CMD_LINE_ARGS;

		// open input file
		std::ifstream input;
		input.open(argv[1], std::ifstream::in);
		if (!input.is_open())
			throw EXCEPTION_FILE_OPEN;
		
		// initialization
		Function<VectorDouble, double> function(f);
		Function<VectorDouble, VectorDouble> gradfunction(gradf);
		GradientInput gradientInput(input);
		GradientOutput gradientOutput;
		input.close();

		// 1st-order method call
		GradientMethod* method1 = new GradientMethod1stOrder();
		VectorDouble ans = method1->solve(function, gradfunction, gradientInput, gradientOutput);
		std::cout << "answer 1st-order method: " << ans.transpose() << '\n';
		std::cout << "calls  1st-order method: " << function.getCallCount() << '\n';

		// write sequence to output for further plotting
		if (gradientInput.getDim() == 2) {
			std::ofstream output;
			output.open("data.txt", std::ofstream::out);
			if (output.is_open()) {
				for (int i = 0; i < gradientOutput.getApproxSequence().size(); ++i)
					output << gradientOutput.getApproxSequence()[i].transpose() << '\n';
				output.close();

				// run python script to plot 3d scheme
				system("python plot/plot.py");
			}
			else {
				std::cout << "Unable open output file";
			}
		}
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