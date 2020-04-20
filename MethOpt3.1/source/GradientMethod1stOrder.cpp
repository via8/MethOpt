#include "GradientMethod1stOrder.hpp"
#include "Function.cpp"
#include "SolverFibonacci.cpp"
#include "InputData.cpp"
#include <iostream>
#include <assert.h>

VectorDouble GradientMethod1stOrder::solve(
	Function<VectorDouble, double>& function, 
	Function<VectorDouble, VectorDouble>& gradf, 
	GradientInput const& gradientInput,
	GradientOutput& gradientOutput) {

	double epsilon = gradientInput.getEpsilon();
	if (epsilon <= 0.0 || epsilon >= 1)
		throw GradientMethod::EXCEPTION_INAPPROPRIATE_PRESICION;

	// initialize x0 and one-dimensional minimization function
	currPoint = gradientInput.getInitialPoint();
	std::function<double(double)> temp = [&](double alpha) {
		return function(this->currPoint - alpha * gradf(currPoint));
	};
	Function<double, double> minf(temp);

	// initialize one-dimensional Fibonacci method
	// best approach using it - when Fn > (b - a) / eps
	double a = epsilon;
	double b = 1.0;
	double minFibonacciNumber = (b - a) / epsilon;
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
		solverFibonacci = new SolverFibonacci(n + 1);
		inputData = new InputData(a, b, epsilon);
	}
	catch (char const* exception) {
		throw GradientMethod::EXCEPTION_INAPPROPRIATE_PRESICION;
	}

	// square of l2 norm of gradf(xk) < eps - exit condition
	double l2norm = gradf(currPoint).norm();
	while (l2norm * l2norm >= epsilon) {
		double alpha = solverFibonacci->solve(minf, *inputData);
		gradientOutput.putPoint(currPoint);
		currPoint = currPoint - alpha * gradf(currPoint);
		l2norm = gradf(currPoint).norm();
		// TODO: check (*prev, *curr) ~ 0
	}
	gradientOutput.putPoint(currPoint);

	delete solverFibonacci;
	delete inputData;

	return currPoint;
}