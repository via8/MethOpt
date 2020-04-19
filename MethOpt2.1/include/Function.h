#ifndef FUNCTION_H
#define FUNCTION_H

#include <functional>

template <typename arg_t, typename return_t>
class Function {
private:
	std::function<return_t(arg_t)> m_function;
	int m_callCount;

public:
	Function(std::function<return_t(arg_t)> function) : m_function(function), m_callCount(0) {};
	~Function() {};

	// function call
	return_t operator()(arg_t x) {
		++m_callCount;
		return m_function(x);
	}
	int getCallCount() { return m_callCount; }
	void resetCallCount() { m_callCount = 0; }
};

#endif /* FUNCTION_H */