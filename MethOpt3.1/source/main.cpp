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

// define objective function here
double f(VectorDouble x) {
	return A_PARAM * x[0] + x[1] + 4.0 * sqrt(1.0 + B_PARAM * x[0] * x[0] + C_PARAM * x[1] * x[1]);
}

// define objective function gradient here
VectorDouble gradf(VectorDouble x) {
	VectorDouble res = VectorDouble(x.rows());
	res[0] = A_PARAM + 4.0 * B_PARAM * x[0] / sqrt(1.0 + B_PARAM * x[0] * x[0] + C_PARAM * x[1] * x[1]);	// gradf(x)[0]
	res[1] =	 1.0 + 4.0 * C_PARAM * x[1] / sqrt(1.0 + B_PARAM * x[0] * x[0] + C_PARAM * x[1] * x[1]);	// gradf(x)[1]
	return res;
}

// define objective function Hessian here
MatrixDouble hessianf(VectorDouble x) {
	MatrixDouble res = MatrixDouble(x.rows(), x.rows());
	double temp = 1.0 + B_PARAM * x[0] * x[0] + C_PARAM * x[1] * x[1];
	double denominator = sqrt(temp * temp * temp);
	res(0, 0) = 4.0 * B_PARAM * (1.0 + C_PARAM * x[1] * x[1]) / denominator;
	res(1, 1) = 4.0 * C_PARAM * (1.0 + B_PARAM * x[0] * x[0]) / denominator;
	res(0, 1) = res(1, 0) = -4.0 * B_PARAM * C_PARAM * x[0] * x[1] / denominator;
	return res;
}

/* END OF INITIAL DATA */

void plot(GradientOutput const& gradientOutput) {
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

int main(int argc, char const* argv[]) {
	char const* EXCEPTION_CMD_LINE_ARGS = "Input data filename expected as a command line argument";
	char const* EXCEPTION_FILE_OPEN = "Unable to open input file";

	char const* helpMsg = "\n---HELP---\n\n"
		"For optimization problem, solved by gradient methods of 1st and 2nd orders\n"
		"input data is expected to be the following:\n"
		"-integer number n >= 1 which indicates dimension of objective function\n"
		"-n real numbers that specify initial point coordinates\n"
		"-real number that specify needed precision of calculations\n"
		"\n---START OF EXAMPLE---\n\n"
		"2\n"
		"5.0 5.0\n"
		"1e-3\n"
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
		Function<VectorDouble, MatrixDouble> hessianfunction(hessianf);
		GradientInput gradientInput(input);
		GradientOutput gradientOutput;
		input.close();

		// 1st-order method call
		GradientMethod1stOrder method1st;
		VectorDouble ans1st = method1st.solve(gradientInput, gradientOutput, function, gradfunction);
		std::cout << "answer 1st-order method: " << ans1st.transpose() << '\n';
		std::cout << "calls  1st-order method: " << function.getCallCount() << '\n';
		if (gradientInput.getDim() == 2) {
			plot(gradientOutput);
		}
		gradientOutput.clearApproxSequence();

		// 2nd-order method call
		GradientMethod2ndOrder method2nd;
		VectorDouble ans2nd = method2nd.solve(gradientInput, gradientOutput, function, gradfunction, hessianfunction);
		std::cout << "answer 2nd-order method: " << ans2nd.transpose() << '\n';
		std::cout << "calls  2nd-order method: " << function.getCallCount() << '\n';
		if (gradientInput.getDim() == 2) {
			plot(gradientOutput);
		}
		gradientOutput.clearApproxSequence();
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