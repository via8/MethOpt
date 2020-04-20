#ifndef GRADIENT_METHOD_H
#define GRADIENT_METHOD_H

#include "GradientInput.hpp"
#include "GradientOutput.hpp"
#include "EigenHelper.hpp"
#include "Function.h"

class GradientMethod {
protected:
	static char const* EXCEPTION_INAPPROPRIATE_PRESICION;

public:
	GradientMethod() {}
	virtual ~GradientMethod() {}
	virtual VectorDouble solve(
		Function<VectorDouble, double>& f,
		Function<VectorDouble, VectorDouble>& gradf,
		GradientInput const& gradientInput,
		GradientOutput& gradientOutput) = 0;
};

#endif /* GRADIENT_METHOD_H */