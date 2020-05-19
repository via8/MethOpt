#ifndef SOLVER_DICHOTOMY_H
#define SOLVER_DICHOTOMY_H

#include "Solver.h"

class SolverDichotomy : public Solver {
private:
public:
	SolverDichotomy() {}
	~SolverDichotomy() {}
	double solve(Function<double, double>& function, InputData const& data) const;
};

#endif /* SOLVER_DICHOTOMY_H */