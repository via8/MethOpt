#include "TransportProblemTable.hpp"

char const* TransportProblemTable::EXCEPTION_INVALID_INPUT_DATA = "Unable to read input data, check validity";
char const* TransportProblemTable::EXCEPTION_UNEXPECTED_EOF		= "Unexpected end of file reached";

TransportProblemTable::TransportProblemTable(std::ifstream& input) {
	// prepare stringstream for building object
	std::stringstream stream;
	stream << input.rdbuf();

	// read number of warehouses and number of delivery points
	if (!(stream >> m) || m <= 0)
		throw EXCEPTION_INVALID_INPUT_DATA;
	if (!(stream >> n) || n <= 0)
		throw EXCEPTION_INVALID_INPUT_DATA;

	// read vector of available cargo and vector of required cargo
	a = VectorFloat::Zero(m);
	b = VectorFloat::Zero(n);
	for (Int i = 0; i < m; ++i) {
		if (!(stream >> a(i)))
			throw EXCEPTION_UNEXPECTED_EOF;
		if (a(i) <= 0)
			throw EXCEPTION_INVALID_INPUT_DATA;
	}
	for (Int j = 0; j < n; ++j) {
		if (!(stream >> b(j)))
			throw EXCEPTION_UNEXPECTED_EOF;
		if (b(j) <= 0)
			throw EXCEPTION_INVALID_INPUT_DATA;
	}

	// build matrix of variables and read matrix of tariffs
	c = MatrixFloat::Zero(m, n);
	x = MatrixFloat::Zero(m, n);
	for (Int i = 0; i < m; ++i) {
		for (Int j = 0; j < n; ++j) {
			if (!(stream >> c(i, j)))
				throw EXCEPTION_UNEXPECTED_EOF;
			if (c(i, j) < 0)
				throw EXCEPTION_INVALID_INPUT_DATA;
		}
	}

	// fill container of banned routes
	while (!stream.eof()) {
		Int from;
		Int to;
		if (stream >> from) {
			if (!(stream >> to))
				throw EXCEPTION_UNEXPECTED_EOF;
			if (from < 0 || from >= m ||
				to   < 0 || to   >= n)
				throw EXCEPTION_INVALID_INPUT_DATA;
			c(from, to) = EigenHelper::InfinityFloat;
		}
	}

	// TODO: check correctness of input data: sum(b_j) <= sum(a_i)
	// TODO: implement auto-closing
}