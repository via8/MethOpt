#include "TransportProblemSolver.hpp"
#include <assert.h>	// assert
#include <iostream>

char const* TransportProblemSolver::EXCEPTION_NO_SOLUTION = "No solution can be found";
char const* TransportProblemSolver::EXCEPTION_NW_METHOD_NOT_APPLICABLE = "NW method of calculating initial solution is not applicable for specified problem";
char const* TransportProblemSolver::EXCEPTION_MATRIX_RANK = "Tariffs matrix rank is inappropriate";
char const* TransportProblemSolver::EXCEPTION_MEMORY_LACK = "Unable allocate memory for calculations";
char const* TransportProblemSolver::EXCEPTION_RECALCULATION_CYCLE = "Unknown error, unable to build recalculation cycle";

TransportProblemSolver::TransportProblemSolver() {}

TransportProblemSolver::~TransportProblemSolver() {}

void TransportProblemSolver::solve(TransportProblemTable& table, InitApprox method) const {
	// check matrix rank
	if (EigenHelper::matrixFloatRank(table.getc()) > table.getm() + table.getn() - 1)
		throw EXCEPTION_MATRIX_RANK;

	// find initial solution
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

	potentialsMethod(table);
}

void TransportProblemSolver::northwestCornerMethod(TransportProblemTable& table) const {
	Int m = table.getm();
	Int n = table.getn();
	VectorInt a = table.geta();
	VectorInt b = table.getb();

	for (Int i = 0, j = 0; i != m || j != n;) {

		if (table.getc()(i, j) == EigenHelper::InfinityFloat)
			throw EXCEPTION_NW_METHOD_NOT_APPLICABLE;

		Int min = a(i) < b(j) ? a(i) : b(j);
		table.getx()(i, j) = min;

		a(i) -= min;
		b(j) -= min;

		if (a(i) == 0)
			++i;

		if (b(j) == 0)
			++j;
	}
}

void TransportProblemSolver::minimumElementMethod(TransportProblemTable& table) const {
	Int m = table.getm();
	Int n = table.getn();
	VectorInt a = table.geta();
	VectorInt b = table.getb();

	// simple ~(mn)(m + n) implementation
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

// TODO: optimize
void TransportProblemSolver::potentialsMethod(TransportProblemTable& table) const {
	Int m = table.getm();
	Int n = table.getn();
	VectorFloat u = VectorFloat(m);				// u_i, i from 0..m-1 potentials
	VectorFloat v = VectorFloat(n);				// v_j, j from 0..n-1 potentials
	std::vector<std::pair<Int, Int>> filled;	// routes (cells - pairs of i, j) assigned with cargo
	
	bool optimal = false;
	while (!optimal) {
		u.setConstant(EigenHelper::NanFloat);
		v.setConstant(EigenHelper::NanFloat);
		filled.clear();

		// build vector of currently filled cells
		for (Int i = 0; i < m; ++i) {
			for (Int j = 0; j < n; ++j) {
				if (table.getx()(i, j) != 0)
					filled.push_back(std::pair<Int, Int>(i, j));
			}
		}
		assert(filled.size() <= (size_t)(m + n));
		assert(filled.size() >= (size_t) n);

		// find all potentials by solving linear problem
		calculatePotentials(table.getc(), filled, u, v);

		// check optimality condition
		Int row;
		Int col;
		if (!(optimal = isOptimal(table.getx(), table.getc(), u, v, row, col))) {
			// if not optimal then build and run recalculation cycle
			std::cout << "\nbefore\n" << table.getx() << "\n\n";
			runRecalculationCycle(table.getx(), row, col);
			std::cout << "\nafter\n" << table.getx() << "\n\n";
		}
	}
}

// TODO: optimize
void TransportProblemSolver::calculatePotentials(MatrixFloat const& c, std::vector<std::pair<Int, Int>> const& filled, VectorFloat& u, VectorFloat& v) const {
	Int m = c.rows();
	Int n = c.cols();
	Int nullable = m + n - filled.size();

	Int nullified = 0;
	Int resolved = 0;
	while (resolved != m + n) {
		assert(resolved < m + n);
		for (std::pair<Int, Int> cell : filled) {
			if (EigenHelper::isNan(u(cell.first))) {
				if (EigenHelper::isNan(v(cell.second))) {
					if (nullified < nullable) {
						u(cell.first) = 0.0f;
						v(cell.second) = u(cell.first) + c(cell.first, cell.second);
						resolved += 2;
						++nullified;
					}
					else
						continue;
				}
				else {
					u(cell.first) = v(cell.second) - c(cell.first, cell.second);
					++resolved;
				}
			}
			else if (EigenHelper::isNan(v(cell.second))) {
				if (EigenHelper::isNan(u(cell.first))) {
					if (nullified < nullable) {
						u(cell.first) = 0.0f;
						v(cell.second) = u(cell.first) + c(cell.first, cell.second);
						resolved += 2;
						++nullified;
					}
					else
						continue;
				}
				else {
					v(cell.second) = u(cell.first) + c(cell.first, cell.second);
					++resolved;
				}
			}
		}
	}
}

bool TransportProblemSolver::isOptimal(MatrixInt const& x, MatrixFloat const& c, VectorFloat const& u, VectorFloat const& v, Int& row, Int& col) const {
	for (row = 0; row < x.rows(); ++row) {
		for (col = 0; col < x.cols(); ++col) {
			// if cell is empty (filled does not contain it) and optimality condition is violated
			if (x(row, col) == 0 && v(col) - u(row) > c(row, col)) {
				return false;
			}
		}
	}
	
	return true;
}

void TransportProblemSolver::runRecalculationCycle(MatrixInt& x, Int row, Int col) const {
	std::pair<Int, Int> initial = std::pair<Int, Int>(row, col);

	// build recalculation cycle
	Pathfinder pathfinder(x, initial);
	if (!pathfinder.recursivePathSearch(initial, Pathfinder::VERTICAL, Pathfinder::PLUS))
		throw EXCEPTION_RECALCULATION_CYCLE;

	// find minimum value among all cells in path with MINUS sign 
	std::vector<std::pair<Int, Int>> path = pathfinder.getPath();
	Int min = EigenHelper::InfinityInt;
	// even => PLUS, odd => MINUS
	for (Int i = 1; i < path.size(); i += 2) {
		if (min > x(path[i].first, path[i].second))
			min = x(path[i].first, path[i].second);
	}
	
	// recalculate values in path's cells (PLUS => + min, MINUS => - min)
	for (Int i = 0; i < path.size(); ++i) {
		if (i % 2)
			x(path[i].first, path[i].second) -= min;
		else
			x(path[i].first, path[i].second) += min;
	}
}

TransportProblemSolver::Pathfinder::Pathfinder(MatrixInt const& x, std::pair<Int, Int> initial)
	: x(x), m(x.rows()), n(x.cols()), initial(initial) {
	hPathsPaved = std::vector<bool>(x.rows(), false);
	vPathsPaved = std::vector<bool>(x.cols(), false);
}

TransportProblemSolver::Pathfinder::~Pathfinder() {}

// TODO: remove recursion & optimize
bool TransportProblemSolver::Pathfinder::recursivePathSearch(std::pair<Int, Int> cursor, Int prevDir, Int prevSign) {
	assert(path.size() < (size_t)(m + n));
	path.push_back(cursor);
	//std::cout << cursor.first << ' ' << cursor.second << '\n';

	if (prevDir == HORIZONTAL) {
		hPathsPaved[cursor.first] = true;
		for (Int i = 0; i < m; ++i) {
			if (cursor != initial && i == initial.first && cursor.second == initial.second)
				// TODO: ONLY IF CYCLE COST < 0 => RETURN TRUE, ELSE RETURN FALSE
				return true;
			if (hPathsPaved[i] || (prevSign == PLUS && x(i, cursor.second) == 0))
				continue;
			//if ((i == initial.first && cursor.second == initial.second) || //std::pair<Int, Int>(i, cursor.second) == initial ||
			if	(recursivePathSearch(std::pair<Int, Int>(i, cursor.second), VERTICAL, prevSign == PLUS ? MINUS : PLUS))
				return true;
		}
		hPathsPaved[cursor.first] = false;
	}
	else {
		vPathsPaved[cursor.second] = true;
		for (Int j = 0; j < n; ++j) {
			// THERE'S NO WAY WE CAN RETURN HORIZONTALLY
			//if (cursor != initial && j == initial.second && cursor.first == initial.first)
			//	return true;

			// we start moving horizontally => we have to return in initial cell vertically
			// => vPathsPaved[initial.second] may be 'true'
			if ((vPathsPaved[j] && j != initial.second) || (prevSign == PLUS && x(cursor.first, j) == 0))
				continue;
			//if ((j == initial.second && cursor.first == initial.first) || //std::pair<Int, Int>(cursor.first, j) == initial ||
			if	(recursivePathSearch(std::pair<Int, Int>(cursor.first, j), HORIZONTAL, prevSign == PLUS ? MINUS : PLUS))
				return true;
		}
		vPathsPaved[cursor.second] = false;
	}

	path.pop_back();
	return false;
}