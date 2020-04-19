#ifndef GRADIENT_METHOD_2ST_ORDER_H
#define GRADIENT_METHOD_2ST_ORDER_H

#include "GradientMethod.hpp"

class GradientMethod2stOrder : public GradientMethod {
private:
public:
	GradientMethod2stOrder() {}
	~GradientMethod2stOrder() {}
	VectorDouble solve(
		Function<VectorDouble, double>& f,
		Function<VectorDouble, VectorDouble>& gradf,
		VectorDouble initialPoint, double epsilon);
};

#endif /* GRADIENT_METHOD_2ST_ORDER_H */
