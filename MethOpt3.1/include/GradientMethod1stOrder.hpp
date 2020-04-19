#ifndef GRADIENT_METHOD_1ST_ORDER_H
#define GRADIENT_METHOD_1ST_ORDER_H

#include "GradientMethod.hpp"

class GradientMethod1stOrder : public GradientMethod {
private:
	VectorDouble currPoint;

public:
	GradientMethod1stOrder() {}
	~GradientMethod1stOrder() {}
	VectorDouble solve(
		Function<VectorDouble, double>& f, 
		Function<VectorDouble, VectorDouble>& gradf, 
		VectorDouble initialPoint, double epsilon);
};

#endif /* GRADIENT_METHOD_1ST_ORDER_H */
