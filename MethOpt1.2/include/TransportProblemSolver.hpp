#ifndef TRANSPORT_PROBLEM_SOLVER_HPP
#define TRANSPORT_PROBLEM_SOLVER_HPP

#include "EigenHelper.hpp"
#include "TransportProblemTable.hpp"

class TransportProblemSolver {
private:
	static char const* EXCEPTION_NO_SOLUTION;

	void northwestCornerMethod(TransportProblemTable& table) const;
	void  minimumElementMethod(TransportProblemTable& table) const;

public:
	// enumeration of methods of building initial approximation
	enum class InitApprox {
		NW_CORNER_METHOD = 0,
		MIN_ELEM_METHOD  = 1
	};

	 TransportProblemSolver();
	~TransportProblemSolver();

	void solve(TransportProblemTable& table, InitApprox method = InitApprox::NW_CORNER_METHOD) const;
};

#endif /* TRANSPORT_PROBLEM_SOLVER_HPP */