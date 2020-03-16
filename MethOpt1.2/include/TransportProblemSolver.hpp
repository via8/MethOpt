#ifndef TRANSPORT_PROBLEM_SOLVER_HPP
#define TRANSPORT_PROBLEM_SOLVER_HPP

#include "EigenHelper.hpp"
#include "TransportProblemTable.hpp"

class TransportProblemSolver {
private:
	void northwestCornerMethod(TransportProblemTable& table) const;
	void  minimumElementMethod(TransportProblemTable& table) const;

public:
	// enumeration of methods of building initial approximation
	enum tag_init_approx {
		NW_CORNER_METHOD = 0,
		MIN_ELEM_METHOD  = 1
	};

	 TransportProblemSolver();
	~TransportProblemSolver();

	VectorFloat solve(TransportProblemTable& table, tag_init_approx method = NW_CORNER_METHOD) const;
};

#endif /* TRANSPORT_PROBLEM_SOLVER_HPP */