#include "GradientMethod2ndOrder.hpp"

char const* GradientMethod2ndOrder::EXCEPTION_INAPPROPRIATE_PRESICION = "Precision must be in range (0, 1)";

VectorDouble GradientMethod2ndOrder::solve(
	GradientInput const& gradientInput,
	GradientOutput& gradientOutput,
	Function<VectorDouble, double>& f,
	Function<VectorDouble, VectorDouble>& gradf,
	Function<VectorDouble, MatrixDouble>& hessianf) {

	f.resetCallCount();
	gradf.resetCallCount();
	hessianf.resetCallCount();

	double epsilon = gradientInput.getEpsilon();
	if (epsilon <= 0.0 || epsilon >= 1)
		throw EXCEPTION_INAPPROPRIATE_PRESICION;

	// initialize
	currPoint				   = gradientInput.getInitialPoint();
	VectorDouble currGrad	   = gradf(currPoint);
	MatrixDouble currHessian   = hessianf(currPoint);
	VectorDouble currDirection = EigenHelper::solveLinearDense(currHessian, -currGrad);
	double const EPS_PARAM	= 0.5;
	double const lambda		= 0.8;
	double fCurr = f(currPoint);
	double l2norm = gradf(currPoint).norm();

	// square of l2 norm of gradf(xk) < eps - exit condition
	while (l2norm * l2norm >= epsilon) {
		double alpha = 1.0;
		VectorDouble nextPoint = currPoint + alpha * currDirection;
		double fNext = f(nextPoint);
		while (fNext - fCurr > EPS_PARAM * alpha * currGrad.transpose() * currDirection) {
			alpha *= lambda;
			nextPoint = currPoint + alpha * currDirection;
			fNext = f(nextPoint);
		}
		gradientOutput.putPoint(currPoint);
		currPoint		= nextPoint;
		currGrad		= gradf(currPoint);
		currHessian		= hessianf(currPoint);
		currDirection	= EigenHelper::solveLinearDense(currHessian, -currGrad);
		fCurr			= fNext;
		l2norm = currGrad.norm();
	}
	gradientOutput.putPoint(currPoint);

	return currPoint;
}
