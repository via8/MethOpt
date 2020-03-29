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

	VectorInt a;	// vector of available cargo a_i, i = 0..m-1
	VectorInt b;	// vector of required  cargo b_j, j = 0..n-1

	MatrixFloat c;	// matrix of tariffs   c_ij
	MatrixInt x;	// matrix of variables x_ij

public:
	TransportProblemTable(std::ifstream& input);
	Int getm() const { return m; }
	Int getn() const { return n; }
	VectorInt const& geta() const { return a; }
	VectorInt const& getb() const { return b; }
	MatrixFloat const& getc() const { return c; }
	MatrixInt& getx() { return x; }
};

#endif /* TRANSPORT_PROBLEM_TABLE_HPP */