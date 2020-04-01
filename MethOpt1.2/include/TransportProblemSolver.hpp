#ifndef TRANSPORT_PROBLEM_SOLVER_HPP
#define TRANSPORT_PROBLEM_SOLVER_HPP

#include "EigenHelper.hpp"
#include "TransportProblemTable.hpp"
#include <vector>	// vector
#include <utility>	// pair

class TransportProblemSolver {
private:
	static char const* EXCEPTION_NO_SOLUTION;
	static char const* EXCEPTION_NW_METHOD_NOT_APPLICABLE;
	static char const* EXCEPTION_MATRIX_RANK;				// TODO: this one is not an exception actually, resolvable
	static char const* EXCEPTION_MEMORY_LACK;
	static char const* EXCEPTION_RECALCULATION_CYCLE;
	static float const EPSILON;

	void northwestCornerMethod(TransportProblemTable& table) const;
	void  minimumElementMethod(TransportProblemTable& table) const;
	void potentialsMethod(TransportProblemTable& table) const;

	void calculatePotentials(MatrixFloat const& c, std::vector<std::pair<Int, Int>> const& filled, VectorFloat& u, VectorFloat& v) const;
	bool isOptimal(MatrixFloat const& x, MatrixFloat const& c, VectorFloat const& u, VectorFloat const& v, Int& row, Int& col) const;
	void runRecalculationCycle(TransportProblemTable& table, Int row, Int col) const;
	
	class Pathfinder {
	private:
		MatrixFloat const& x;
		MatrixFloat const& c;
		Int const m;
		Int const n;
		std::pair<Int, Int> initial;
		std::vector<bool> vPathsPaved;
		std::vector<bool> hPathsPaved;
		std::vector<std::pair<Int, Int>> path;

	public:
		enum {
			VERTICAL = 0,
			HORIZONTAL
		};

		Pathfinder(MatrixFloat const& x, MatrixFloat const& c, std::pair<Int, Int> initial);
		~Pathfinder();

		bool recursivePathSearch(std::pair<Int, Int> cursor, Int prevDir);
		std::vector<std::pair<Int, Int>> getPath() { return path; }
	};

public:
	// enumeration of methods of building initial approximation
	enum class InitApprox {
		NW_CORNER_METHOD = 0
		//MIN_ELEM_METHOD  = 1
	};

	 TransportProblemSolver();
	~TransportProblemSolver();

	void solve(TransportProblemTable& table, InitApprox method = InitApprox::NW_CORNER_METHOD) const;
	//void solve(TransportProblemTable& table, InitApprox method = InitApprox::MIN_ELEM_METHOD) const;
};

#endif /* TRANSPORT_PROBLEM_SOLVER_HPP */