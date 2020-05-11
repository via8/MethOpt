# all the designations set as zoutendijk method description source
# (from schoolbook, no link)
import zoutendijk
from numpy import concatenate


def main():
    # objective function and it's gradient
    def phi0(x, y): return 3.0 * x * x + y * y - x * y - 2.0 * x - 3.0 * y
    def phi0_der(x, y): return [6.0 * x - y - 2.0, 2.0 * y - x - 3.0]

    # list of phi_i(x1, x2, ...) <= 0 and their gradients
    phi_list = []
    def phi1(x, y): return x - 5.0 * y - 5.0
    def phi2(x, y): return 2.0 * x * x - y + 1.0
    def phi3(x, y): return -x - 1.0
    def phi4(x, y): return -y
    phi_list.append(phi1)
    phi_list.append(phi2)
    phi_list.append(phi3)
    phi_list.append(phi4)
    phi_der_list = []
    def phi1_der(x, y): return [1.0, -5.0]
    def phi2_der(x, y): return [4.0 * x, -1.0]
    def phi3_der(x, y): return [-1.0, 0.0]
    def phi4_der(x, y): return [0.0, -1.0]
    phi_der_list.append(phi1_der)
    phi_der_list.append(phi2_der)
    phi_der_list.append(phi3_der)
    phi_der_list.append(phi4_der)

    # objective function for searching initial approximation (x0)
    def x0_phi0(x, y, eta): return eta
    def x0_phi0_der(x, y, eta): return [0.0, 0.0, 1.0]

    # list of phi_i(x1, x2, ..., eta) <= 0 and their gradients for x0
    x0_phi_list = []
    def x0_phi1(x, y, eta): return phi1(x, y) - eta
    def x0_phi2(x, y, eta): return phi2(x, y) - eta
    def x0_phi3(x, y, eta): return phi3(x, y) - eta
    def x0_phi4(x, y, eta): return phi4(x, y) - eta
    x0_phi_list.append(x0_phi1)
    x0_phi_list.append(x0_phi2)
    x0_phi_list.append(x0_phi3)
    x0_phi_list.append(x0_phi4)
    x0_phi_der_list = []
    def x0_phi1_der(x, y, eta): return concatenate((phi1_der(x, y), [-1.0]), axis=None)
    def x0_phi2_der(x, y, eta): return concatenate((phi2_der(x, y), [-1.0]), axis=None)
    def x0_phi3_der(x, y, eta): return concatenate((phi3_der(x, y), [-1.0]), axis=None)
    def x0_phi4_der(x, y, eta): return concatenate((phi4_der(x, y), [-1.0]), axis=None)
    x0_phi_der_list.append(x0_phi1_der)
    x0_phi_der_list.append(x0_phi2_der)
    x0_phi_der_list.append(x0_phi3_der)
    x0_phi_der_list.append(x0_phi4_der)

    # matrix A, vector b from limitations Ax = b
    A = [[4, -1]]
    b = [-2]

    # matrix A, vector b from limitations Ax = b for auxiliary problem
    x0_A = [concatenate((A[0], [0.0]), axis=None)]
    x0_b = b

    # precision
    epsilon = 1e-3

    # TODO: implement general search of x0
    # x0 for x0 search, yes
    x0 = [2.0, 10.0]
    curr = -float('inf')
    for phi_i in phi_list:
        temp = phi_i(*x0)
        if curr < temp:
            curr = temp
    x0 = concatenate((x0, [curr]), axis=None)

    # find initial approximation by solving auxiliary problem
    x0 = zoutendijk.run(x0, x0_phi0, x0_phi0_der, x0_phi_list, x0_phi_der_list, x0_A, x0_b, epsilon)
    x0 = x0[:-1]

    # find solution
    ans = zoutendijk.run(x0, phi0, phi0_der, phi_list, phi_der_list, A, b, epsilon)
    print('zoutendijk solution:')
    print(ans)


if __name__ == '__main__':
    main()
