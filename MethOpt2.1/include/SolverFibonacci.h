#ifndef SOLVER_FIBONACCI_H
#define SOLVER_FIBONACCI_H

#include "Solver.h"

class SolverFibonacci : public Solver {
private:
	static const char* EXCEPTION_N;
	static const char* EXCEPTION_MEM;

	int const m_n;			// Fibonacci sequence length
	int*	  m_fib;		// Fibonacci sequence

public:
	SolverFibonacci(int n);
	~SolverFibonacci();
	double solve(Function<double, double>& function, InputData const& data) const;
};

#endif /* SOLVER_FIBONACCI_H */