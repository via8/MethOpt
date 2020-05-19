#ifndef SOLVER_H
#define SOLVER_H

#include "Function.h"
#include "InputData.h"

class Solver {
public:
	Solver() {}
	virtual ~Solver() {}
	virtual double solve(Function<double, double>& function, InputData const& data) const = 0;
};

#endif /* SOLVER_H */