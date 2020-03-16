#include <iostream>	// cout
#include <fstream>	// ifstream
#include "TransportProblemTable.hpp"

int main(int argc, char const* argv[]) {
	char const* EXCEPTION_CMD_LINE_ARGS = "Input data filename expected as a command line argument";
	char const* EXCEPTION_FILE_OPEN = "Unable to open input file";

	char const* helpMsg = "\n---HELP---\n\n"
		"Transport problem input file must be following format:\n"
		"-two integers: number of warehouses (m) and number of delivery points (n)\n"
		"-vector of available cargo (a_i) size of m\n"
		"-vector of required  cargo (b_j) size of n\n"
		"-matrix of tariffs (c_ij) size of m x n\n"
		"-last specify delivery bans - pairs of integers A and B:\n"
		" from where (0...m-1) to where (0...n-1) delivery is not allowed\n"
		"-m, n     >  0\n"
		"-a_i, b_j >  0\n"
		"-c_ij     >= 0\n"
		"\n---START OF EXAMPLE---\n\n"
		"4 5\n"
		"\n"
		"8 3 11 5\n"
		"4 5 4 9 5\n"
		"\n"
		"8 2 5 3 14\n"
		"10 4 5 7 15\n"
		"5 1 2 1 10\n"
		"6 3 2 4 15\n"
		"\n"
		"0 1\n"
		"2 3\n"
		"\n---END OF EXAMPLE---\n";

	try {
		if (argc != 2)
			throw EXCEPTION_CMD_LINE_ARGS;

		std::ifstream input;
		input.open(argv[1], std::ifstream::in);
		if (!input.is_open())
			throw EXCEPTION_FILE_OPEN;

		TransportProblemTable table(input);
	}
	catch (char const* errMsg) {
		std::cout << "ERROR: " << errMsg << '\n';

		if (errMsg != EXCEPTION_CMD_LINE_ARGS &&
			errMsg != EXCEPTION_FILE_OPEN) {
			std::cout << helpMsg << '\n';
		}

		return -1;
	}

	return 0;
}