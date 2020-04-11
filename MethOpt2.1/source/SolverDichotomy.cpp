#include "SolverDichotomy.h"

double SolverDichotomy::solve(Function& function, InputData const& data) const {
	double a_k = data.geta();
	double b_k = data.getb();
	double eps = data.getEpsilon();
	while (b_k - a_k > eps) {
		double middle = (a_k + b_k) / 2;
		double delta = (b_k - a_k) / 1000;		// delta := 0.1% of (bk - ak)
		double left  = middle - delta;
		double right = middle + delta;
		if (function(left) < function(right))
			b_k = right;
		else
			a_k = left;
	}

	return (a_k + b_k) / 2;
}