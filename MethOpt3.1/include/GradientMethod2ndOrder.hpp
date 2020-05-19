#ifndef GRADIENT_METHOD_2ND_ORDER_H
#define GRADIENT_METHOD_2ND_ORDER_H

#include "GradientInput.hpp"
#include "GradientOutput.hpp"
#include "EigenHelper.hpp"
#include "Function.h"

class GradientMethod2ndOrder {
private:
	static char const* EXCEPTION_INAPPROPRIATE_PRESICION;

	VectorDouble currPoint;

public:
	GradientMethod2ndOrder() {}
	~GradientMethod2ndOrder() {}
	VectorDouble solve(
		GradientInput const& gradientInput,
		GradientOutput& gradientOutput,
		Function<VectorDouble, double>& f,
		Function<VectorDouble, VectorDouble>& gradf,
		Function<VectorDouble, MatrixDouble>& hessianf);
};

#endif /* GRADIENT_METHOD_2ND_ORDER_H */
