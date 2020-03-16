#include "TransportProblemSolver.hpp"

TransportProblemSolver::TransportProblemSolver() {}

TransportProblemSolver::~TransportProblemSolver() {}

VectorFloat TransportProblemSolver::solve(TransportProblemTable& table, tag_init_approx method) const {
	switch (method) {
	case NW_CORNER_METHOD:
		northwestCornerMethod(table);
		break;
	case MIN_ELEM_METHOD:
		minimumElementMethod(table);
		break;
	default:
		break;
	}
}

void TransportProblemSolver::northwestCornerMethod(TransportProblemTable& table) const {
	Int m = table.getm();
	Int n = table.getn();

	for (Int i = 0; i < m; ++i) {
		for (Int j = 0; j < n; ++j) {

		}
	}
}

void TransportProblemSolver::minimumElementMethod(TransportProblemTable& table) const {

}