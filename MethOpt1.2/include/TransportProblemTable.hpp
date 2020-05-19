#ifndef TRANSPORT_PROBLEM_TABLE_HPP
#define TRANSPORT_PROBLEM_TABLE_HPP

#include <fstream>	// ifstream
#include "EigenHelper.hpp"

class TransportProblemTable {
private:
	static char const* EXCEPTION_INVALID_INPUT_DATA;
	static char const* EXCEPTION_UNEXPECTED_EOF;

	Int m;	// number of warehouses
	Int n;	// number of delivery points

	VectorFloat a;	// vector of available cargo a_i, i = 0..m-1
	VectorFloat b;	// vector of required  cargo b_j, j = 0..n-1

	MatrixFloat c;	// matrix of tariffs   c_ij
	MatrixFloat x;	// matrix of variables x_ij

public:
	TransportProblemTable(std::ifstream& input);
	Int getm() const { return m; }
	Int getn() const { return n; }
	VectorFloat const& geta() const { return a; }
	VectorFloat const& getb() const { return b; }
	MatrixFloat const& getc() const { return c; }
	MatrixFloat& getx() { return x; }
};

#endif /* TRANSPORT_PROBLEM_TABLE_HPP */