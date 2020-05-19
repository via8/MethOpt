import linear_problem
import linear_solver
from numpy import array, concatenate
from scipy.optimize import linprog


def test(linear_solution, lin_prob, len_eq, m, epsilon):
    n = lin_prob.n
    bounds = [[0.0] * 2] * n
    for i in range(0, n):
        bounds[i][0] = -50.0
        bounds[i][1] = 50.0
    temp = linprog(lin_prob.c,
                   A_ub=lin_prob.A[len_eq:m],
                   b_ub=lin_prob.b[len_eq:m],
                   A_eq=lin_prob.A[0:len_eq],
                   b_eq=lin_prob.b[0:len_eq],
                   bounds=bounds,
                   method='simplex').__getattr__('x')
    assert (len(linear_solution) == len(temp))
    for i in range(0, len(linear_solution)):
        assert(abs(linear_solution[i] - temp[i]) < epsilon)
    return temp


def set_alpha_k(_alpha0, _lambda, phi0, phi_list: [], x_k, s_k, eta_k):
    alpha_k = _alpha0
    temp = x_k + alpha_k * s_k
    fits = False
    while not fits:
        fits = True
        if phi0(*temp) > phi0(*x_k) + 1.0 / 2.0 * eta_k * alpha_k:
            fits = False
        for i in range(0, len(phi_list)):
            if phi_list[i](*temp) > 0.0:
                fits = False
        if not fits:
            alpha_k = alpha_k * _lambda
            temp = x_k + alpha_k * s_k
    return alpha_k


def run(x0: [], phi0, phi0_der, phi_list: [], phi_der_list: [], A: [[]], b: [], epsilon: float):
    _lambda = 0.8  # decrease ratio
    _alpha0 = 0.5  # default step
    _delta0 = 0.01

    n = len(A[0])
    x_k = x0
    eta_k = 1.0
    delta_k = _delta0

    while abs(eta_k) > epsilon:
        # STEP 1: solving auxiliary problem
        # calculate size of I_delta_k = {i | -delta_k <= phi_i(xk) <= 0}
        I_delta_k = []
        m_linear = 1 + len(A) + 2 * n
        for i in range(0, len(phi_list)):
            assert(phi_list[i](*x_k) <= 0.0)
            a = phi_list[i](*x_k)
            if phi_list[i](*x_k) >= -delta_k:
                I_delta_k.append(i)
                m_linear += 1

        # build linear problem
        n_linear = n + 1
        c_linear = concatenate(([0.0] * n, [1.0]), axis=None)
        A_linear = [[0.0] * n_linear] * m_linear
        b_linear = [0.0] * m_linear
        signs_linear = [' = '] * m_linear

        # equalities
        k = 0
        for i in range(0, len(A)):
            A_linear[k] = concatenate((A[i], [0.0]), axis=None)
            k += 1

        # objective function inequality
        A_linear[k] = concatenate((phi0_der(*x_k), [-1.0]), axis=None)
        signs_linear[k] = ' <= '
        k += 1

        # limitations inequalities
        for i in range(0, len(phi_der_list)):
            if i in I_delta_k:
                A_linear[k] = concatenate((phi_der_list[i](*x_k), [-1.0]), axis=None)
                signs_linear[k] = ' <= '
                k += 1

        # ||s|| <= 1
        for i in range(0, n):
            A_linear[k] = [0.0] * n_linear
            A_linear[k][i] = 1.0
            b_linear[k] = 1.0
            signs_linear[k] = ' <= '
            k += 1
            A_linear[k] = [0.0] * n_linear
            A_linear[k][i] = -1.0
            b_linear[k] = 1.0
            signs_linear[k] = ' <= '
            k += 1

        # get linear problem solution
        lin_prob = linear_problem.LinearProblem(n_linear, m_linear)
        lin_prob.c = c_linear
        lin_prob.A = A_linear
        lin_prob.b = b_linear
        lin_prob.signs = signs_linear

        # TODO: use scipy.optimize.linprog from test() method instead
        linear_solution = array(linear_solver.run(lin_prob, epsilon))

        # test result solution via inner method TODO: fix precision before test
        # linear_solution = test(linear_solution, lin_prob, len(A), m_linear, epsilon)

        # STEP 2: setting x_k+1, delta_k+1
        s_k = linear_solution[0:n]
        eta_k = linear_solution[n]
        alpha_k = set_alpha_k(_alpha0, _lambda, phi0, phi_list, x_k, s_k, eta_k)

        if eta_k < -delta_k:
            x_k = x_k + alpha_k * s_k
        else:
            delta_k = _lambda * delta_k

    return x_k
