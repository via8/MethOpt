#ifndef GRADIENT_METHOD_1ST_ORDER_H
#define GRADIENT_METHOD_1ST_ORDER_H

#include "GradientInput.hpp"
#include "GradientOutput.hpp"
#include "EigenHelper.hpp"
#include "Function.h"

class GradientMethod1stOrder {
private:
	static char const* EXCEPTION_INAPPROPRIATE_PRESICION;

	VectorDouble currPoint;

public:
	GradientMethod1stOrder() {}
	~GradientMethod1stOrder() {}
	VectorDouble solve(
		GradientInput const& gradientInput,
		GradientOutput& gradientOutput,
		Function<VectorDouble, double>& f, 
		Function<VectorDouble, VectorDouble>& gradf);
};

#endif /* GRADIENT_METHOD_1ST_ORDER_H */
