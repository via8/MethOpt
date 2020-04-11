#include "SolverFibonacci.h"
#include <new>	// nothrow

char const* SolverFibonacci::EXCEPTION_N   = "Inappropriate Fibonacci sequence length";
char const* SolverFibonacci::EXCEPTION_MEM = "Not enough memory to allocate fibonacci sequence";

SolverFibonacci::SolverFibonacci(int n) :
	m_n(n) {

	if (n < 4)
		throw EXCEPTION_N;

	m_fib = new(std::nothrow) int[m_n];
	if (m_fib == NULL)
		throw EXCEPTION_MEM;

	// generate sequence
	m_fib[0] = 1;
	m_fib[1] = 1;
	for (int i = 2; i < m_n; ++i)
		m_fib[i] = m_fib[i - 1] + m_fib[i - 2];
}

SolverFibonacci::~SolverFibonacci() {
	delete[] m_fib;
}

double SolverFibonacci::solve(Function& function, InputData const& data) const {
	double a_k = data.geta();
	double b_k = data.getb();
	double eps = data.getEpsilon();

	while (b_k - a_k > eps) {
		double lambda_k = a_k + ((double)m_fib[m_n - 3] / m_fib[m_n - 1]) * (b_k - a_k);
		double mu_k		= a_k + ((double)m_fib[m_n - 2] / m_fib[m_n - 1]) * (b_k - a_k);
		double function_lambda_k = function(lambda_k);
		double function_mu_k	  = function(mu_k);
		for (int k = 1; k < m_n - 2; ++k) {
			// proceed with left interval [a_k, mu_k]
			if (function_lambda_k < function_mu_k) {
				b_k = mu_k;
				mu_k = lambda_k;
				lambda_k = a_k + ((double)m_fib[m_n - 3 - k] / m_fib[m_n - 1 - k]) * (b_k - a_k);
				function_mu_k = function_lambda_k;
				function_lambda_k = function(lambda_k);
			}
			// proceed with right interval [lambda_k, b_k]
			else {
				a_k = lambda_k;
				lambda_k = mu_k;
				mu_k = a_k + ((double)m_fib[m_n - 2 - k] / m_fib[m_n - 1 - k]) * (b_k - a_k);
				function_lambda_k = function_mu_k;
				function_mu_k = function(mu_k);
			}
		}
	}

	return (a_k + b_k) / 2;
}