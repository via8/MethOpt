#include "TransportProblemSolver.hpp"

char const* TransportProblemSolver::EXCEPTION_NO_SOLUTION = "No solution can be found";

TransportProblemSolver::TransportProblemSolver() {}

TransportProblemSolver::~TransportProblemSolver() {}

void TransportProblemSolver::solve(TransportProblemTable& table, InitApprox method) const {
	switch (method) {
	case InitApprox::NW_CORNER_METHOD:
		northwestCornerMethod(table);
		break;
	case InitApprox::MIN_ELEM_METHOD:
		minimumElementMethod(table);
		break;
	default:
		break;
	}
}

void TransportProblemSolver::northwestCornerMethod(TransportProblemTable& table) const {
	Int m = table.getm();
	Int n = table.getn();
	VectorInt a = table.geta();
	VectorInt b = table.getb();

	for (Int i = 0, j = 0; i != m || j != n;) {
		Int min = a(i) < b(j) ? a(i) : b(j);
		table.getx()(i, j) = min;

		a(i) -= min;
		b(j) -= min;

		if (a(i) == 0) {
			//for (Int k = j + 1; k < n; ++k)
			//	table.getx()(i, k) = 0;
			++i;
		}

		if (b(j) == 0) {
			//for (Int k = i + 1; k < m; ++k)
			//	table.getx()(k, j) = 0;
			++j;
		}
	}
}

void TransportProblemSolver::minimumElementMethod(TransportProblemTable& table) const {
	Int m = table.getm();
	Int n = table.getn();
	VectorInt a = table.geta();
	VectorInt b = table.getb();

	// simple ~(mn)^2 implementation
	// though we will not need to use
	// NW-method on cells with same tariffs
	// cause of minI and minJ already fit
	Int required = 0;
	for (Int j = 0; j < n; ++j)
		required += table.getb()(j);
	while (required > 0) {
		float minTariff = EigenHelper::InfinityFloat;
		Int minI;
		Int minJ;
		for (Int i = 0; i < m; ++i) {
			if (a(i) == 0)
				continue;
			for (Int j = 0; j < n; ++j) {
				if (b(j) == 0 || table.getx()(i, j) != 0)
					continue;
				if (minTariff > table.getc()(i, j)) {
					minTariff = table.getc()(i, j);
					minI = i;
					minJ = j;
				}	
			}
		}
		if (minTariff == EigenHelper::InfinityFloat)
			throw EXCEPTION_NO_SOLUTION;
		Int min = a(minI) < b(minJ) ? a(minI) : b(minJ);
		table.getx()(minI, minJ) = min;
		a(minI) -= min;
		b(minJ) -= min;
		required -= min;
	}
}