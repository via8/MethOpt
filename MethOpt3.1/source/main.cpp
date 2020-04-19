#include <iostream>	// cout
#include <fstream>	// ifstream
#include <iomanip>	// fixed, setprecision

// define unimodal function here
double f(double x) {
	return 0.0;
}

int main(int argc, char const* argv[]) {
	char const* EXCEPTION_CMD_LINE_ARGS = "Input data filename expected as a command line argument";
	char const* EXCEPTION_FILE_OPEN = "Unable to open input file";

	char const* helpMsg = "\n---HELP---\n\n"
		"\n---START OF EXAMPLE---\n\n"
		"\n---END OF EXAMPLE---\n";

	try {
		if (argc != 2)
			throw EXCEPTION_CMD_LINE_ARGS;

		std::ifstream input;
		input.open(argv[1], std::ifstream::in);
		if (!input.is_open())
			throw EXCEPTION_FILE_OPEN;

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