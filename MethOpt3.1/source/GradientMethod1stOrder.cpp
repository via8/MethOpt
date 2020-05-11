#include "GradientMethod1stOrder.hpp"
#include "Function.cpp"
#include "SolverFibonacci.cpp"
#include "InputData.cpp"
#include <iostream>

char const* GradientMethod1stOrder::EXCEPTION_INAPPROPRIATE_PRESICION = "Precision must be in range (0, 1)";

VectorDouble GradientMethod1stOrder::solve(
	GradientInput const& gradientInput,
	GradientOutput& gradientOutput,
	Function<VectorDouble, double>& f, 
	Function<VectorDouble, VectorDouble>& gradf) {

	f.resetCallCount();
	gradf.resetCallCount();

	double epsilon = gradientInput.getEpsilon();
	if (epsilon <= 0.0 || epsilon >= 1)
		throw EXCEPTION_INAPPROPRIATE_PRESICION;

	// initialize x0 and one-dimensional minimization function
	currPoint = gradientInput.getInitialPoint();
	std::function<double(double)> temp = [&](double alpha) {
		return f(this->currPoint - alpha * gradf(currPoint));
	};
	Function<double, double> minf(temp);

	// initialize one-dimensional Fibonacci method
	// best approach using it - when Fn > (b - a) / eps
	double minfEps = epsilon / 1000;
	double a = minfEps;
	double b = 1.0;
	double minFibonacciNumber = (b - a) / minfEps;
	int n = 2;
	for (int prev = 1, curr = 1; (double)curr <= minFibonacciNumber; ++n) {
		int temp = curr;
		curr = prev + curr;
		prev = temp;
	}
	if (n < 4)
		n = 4;
	Solver* solverFibonacci = nullptr;
	InputData* inputData	= nullptr;
	try {
		solverFibonacci = new SolverFibonacci(n);
		inputData = new InputData(a, b, minfEps);
	}
	catch (char const* exception) {
		throw EXCEPTION_INAPPROPRIATE_PRESICION;
	}
	double l2norm = gradf(currPoint).norm();

	// square of l2 norm of gradf(xk) < eps - exit condition
	std::cout << "ALPHA PARAMETER VALUES\n";
	int i = 0;
	while (l2norm * l2norm >= epsilon) {
		double alpha = solverFibonacci->solve(minf, *inputData);
		gradientOutput.putPoint(currPoint);
		currPoint = currPoint - alpha * gradf(currPoint);
		l2norm = gradf(currPoint).norm();
		std::cout << "step " << ++i << " alpha: " << alpha << '\n';
	}
	gradientOutput.putPoint(currPoint);
	std::cout << '\n';

	delete solverFibonacci;
	delete inputData;

	return currPoint;
}