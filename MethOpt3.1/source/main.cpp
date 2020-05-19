#include <iostream>	// cout
#include <fstream>	// ifstream
#include <iomanip>	// fixed, setprecision
#include <stdlib.h>	// abs
#include <math.h>	// sqrt
#include <fstream>  // ifstream, ofstream
#include <assert.h>	// assert
#include <limits>	// numerical_limits
#include <cmath>	// isfinite
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

// define computational experiment procedure here
bool isApplicable(VectorDouble centerPoint, double delta, double& m, double& M) {
	int const meshParam = 10;
	double const step = delta / meshParam;
	VectorDouble startPoint(centerPoint.rows());
	VectorDouble x(centerPoint.rows());
	VectorDouble y(centerPoint.rows());
	double mCurr = std::numeric_limits<double>::infinity();
	double MCurr = 0;

	startPoint[0] = centerPoint[0] - delta / 2;
	startPoint[1] = centerPoint[1] - delta / 2;

	// outer loop for X point
	for (int i = 0; i < meshParam; ++i) {
		for (int j = 0; j < meshParam; ++j) {
			x[0] = startPoint[0] + step * i;
			x[1] = startPoint[1] + step * j;

			// inner loop for Y point
			for (int ii = 0; ii < meshParam; ++ii) {
				for (int jj = 0; jj < meshParam; ++jj) {
					if (ii == i && jj == j) continue;
					y[0] = startPoint[0] + step * ii;
					y[1] = startPoint[1] + step * jj;

					// here we have check of the next inaquality:
					// m||y||^2 <= yT*H(x)*y <= M||y||^2 using l2 norm
					MatrixDouble hessian = hessianf(x);
					double center = y.transpose() * hessian * y;	// yT*H(x)*y
					double sqNorm = y.norm() * y.norm();			// ||y||^2
					double temp = center / sqNorm;					// yT*H(x)*y / ||y||^2
					if (mCurr > temp)
						mCurr = temp;
					if (MCurr < temp)
						MCurr = temp;
				}
			}
		}
	}

	if (isfinite(mCurr) && MCurr > 0 && mCurr < MCurr) {
		m = mCurr;
		M = MCurr;
		return true;
	}

	return false;
}

/* END OF INITIAL DATA */

double checkOrthty(GradientOutput const& gradientOutput) {
	double max = 0.0;
	if (gradientOutput.getApproxSequence().size() <= 1)
		return max;
	VectorDouble curr = gradientOutput.getApproxSequence()[0] - gradientOutput.getApproxSequence()[1];
	VectorDouble next;
	for (int i = 1, size = gradientOutput.getApproxSequence().size(); i < size - 1; ++i, curr = next) {
		next = gradientOutput.getApproxSequence()[i] - gradientOutput.getApproxSequence()[i + 1];
		curr = curr / curr.norm();
		next = next / next.norm();
		double scalarProduct = curr.transpose() * next;
		if (max < abs(scalarProduct))
			max = abs(scalarProduct);
		//std::cout << "scalar products:\n";
		//std::cout << scalarProduct << '\n';
	}

	return max;
}

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
		std::cout << "1ST-ORDER GRADIENT METHOD\n";
		std::cout << "answer: " << ans1st.transpose() << '\n';
		std::cout << "calls : " << function.getCallCount() << '\n';
		std::cout << "maximum scalar product: " << checkOrthty(gradientOutput) << '\n' << '\n';
		if (gradientInput.getDim() == 2) {
			plot(gradientOutput);
		}
		gradientOutput.clearApproxSequence();

		// define computational experiment here
		// that determines whether 2-nd order method is applicable
		{
			VectorDouble center(gradientInput.getDim());
			center[0] = -2.0 / (3.0 * sqrt(13.0));
			center[1] = -1.0 / (3.0 * sqrt(13.0));
			double m = 0.0;
			double M = 0.0;
			assert(isApplicable(center, 1.0, m, M));
			assert(m > 0);
			assert(M > 0);
			assert(m < M);
			std::cout << "COMPUTATIONAL EXPERIMENT\n";
			std::cout << "m = " << m << '\n';
			std::cout << "M = " << M << '\n';
			std::cout << "convergence rate (the denominator of geometric progression): q = " << (M - m) / (M + m) << '\n' << '\n';
		}

		// 2nd-order method call
		GradientMethod2ndOrder method2nd;
		VectorDouble ans2nd = method2nd.solve(gradientInput, gradientOutput, function, gradfunction, hessianfunction);
		std::cout << "2ND-ORDER GRADIENT METHOD\n";
		std::cout << "answer: " << ans2nd.transpose() << '\n';
		std::cout << "calls : " << function.getCallCount() << '\n';
		std::cout << "maximum scalar product: " << checkOrthty(gradientOutput) << '\n' << '\n';
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