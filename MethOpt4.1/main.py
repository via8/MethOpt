# all the designations set as zoutendijk method description source
# (from schoolbook, no link)
import zoutendijk
from numpy import concatenate


def main():
    # objective function and it's gradient
    def phi0(x, y, z):
        return 3.0 * x * x + y * y - x * y - (2.0 * x + 3.0 * y) / z

    def phi0_der(x, y, z):
        return [6.0 * x - y - 2.0 / z,
                2.0 * y - x - 3.0 / z,
                (2.0 * x + 3.0 * y) / (z * z)]

    # list of phi_i(x1, x2, ...) <= 0 and their gradients
    phi_list = []
    def phi1(x, y, z): return x - 5.0 * y - 5.0
    def phi2(x, y, z): return 2.0 * x * x - y + 1.0
    def phi3(x, y, z): return -y - x * z
    def phi4(x, y, z): return -x
    def phi5(x, y, z): return -y
    def phi6(x, y, z): return -z + 0.5
    phi_list.append(phi1)
    phi_list.append(phi2)
    phi_list.append(phi3)
    phi_list.append(phi4)
    phi_list.append(phi5)
    phi_list.append(phi6)
    phi_der_list = []
    def phi1_der(x, y, z): return [1.0, -5.0, 0.0]
    def phi2_der(x, y, z): return [4.0 * x, -1.0, 0.0]
    def phi3_der(x, y, z): return [-z, -1, -x]
    def phi4_der(x, y, z): return [-1.0, 0.0, 0.0]
    def phi5_der(x, y, z): return [0.0, -1.0, 0.0]
    def phi6_der(x, y, z): return [0.0, 0.0, -1.0]
    phi_der_list.append(phi1_der)
    phi_der_list.append(phi2_der)
    phi_der_list.append(phi3_der)
    phi_der_list.append(phi4_der)
    phi_der_list.append(phi5_der)
    phi_der_list.append(phi6_der)

    # objective function for searching initial approximation (x0)
    def x0_phi0(x, y, z, eta): return eta
    def x0_phi0_der(x, y, z, eta): return [0.0, 0.0, 0.0, 1.0]

    # list of phi_i(x1, x2, ..., eta) <= 0 and their gradients for x0
    x0_phi_list = []
    def x0_phi1(x, y, z, eta): return phi1(x, y, z) - eta
    def x0_phi2(x, y, z, eta): return phi2(x, y, z) - eta
    def x0_phi3(x, y, z, eta): return phi3(x, y, z) - eta
    def x0_phi4(x, y, z, eta): return phi4(x, y, z) - eta
    def x0_phi5(x, y, z, eta): return phi5(x, y, z) - eta
    def x0_phi6(x, y, z, eta): return phi6(x, y, z) - eta
    x0_phi_list.append(x0_phi1)
    x0_phi_list.append(x0_phi2)
    x0_phi_list.append(x0_phi3)
    x0_phi_list.append(x0_phi4)
    x0_phi_list.append(x0_phi5)
    x0_phi_list.append(x0_phi6)
    x0_phi_der_list = []
    def x0_phi1_der(x, y, z, eta): return concatenate((phi1_der(x, y, z), [-1.0]), axis=None)
    def x0_phi2_der(x, y, z, eta): return concatenate((phi2_der(x, y, z), [-1.0]), axis=None)
    def x0_phi3_der(x, y, z, eta): return concatenate((phi3_der(x, y, z), [-1.0]), axis=None)
    def x0_phi4_der(x, y, z, eta): return concatenate((phi4_der(x, y, z), [-1.0]), axis=None)
    def x0_phi5_der(x, y, z, eta): return concatenate((phi5_der(x, y, z), [-1.0]), axis=None)
    def x0_phi6_der(x, y, z, eta): return concatenate((phi6_der(x, y, z), [-1.0]), axis=None)
    x0_phi_der_list.append(x0_phi1_der)
    x0_phi_der_list.append(x0_phi2_der)
    x0_phi_der_list.append(x0_phi3_der)
    x0_phi_der_list.append(x0_phi4_der)
    x0_phi_der_list.append(x0_phi5_der)
    x0_phi_der_list.append(x0_phi6_der)

    # matrix A, vector b from limitations Ax = b
    A = [[4.0, -1.0, 1.0],
         [1.0, 1.0, -3.0]]
    b = [-2.0, 1.0]

    # matrix A, vector b from limitations Ax = b for auxiliary problem
    x0_A = [[0.0] * 3] * 2
    x0_A[0] = concatenate((A[0], [0.0]), axis=None)
    x0_A[1] = concatenate((A[1], [0.0]), axis=None)
    x0_b = b

    # precision
    epsilon = 1e-6

    # TODO: implement general search of x0
    # x0 for x0 search, yes
    x0 = [1.0, 9.0, 3.0]
    curr = -float('inf')
    for phi_i in phi_list:
        temp = phi_i(*x0)
        if curr < temp:
            curr = temp
    x0 = concatenate((x0, [curr]), axis=None)

    # find initial approximation by solving auxiliary problem
    x0 = zoutendijk.run(x0, x0_phi0, x0_phi0_der, x0_phi_list, x0_phi_der_list, x0_A, x0_b, epsilon, trace=False)
    x0 = x0[:-1]
    print('initial approximation:')
    print(str(x0) + '\n')

    # find solution
    ans = zoutendijk.run(x0, phi0, phi0_der, phi_list, phi_der_list, A, b, epsilon, trace=True)
    print('zoutendijk solution:')
    print(str(ans) + '\n')
    print('objective function value:')
    print(str(phi0(*ans)))
    print('objective function der value:')
    print(str(phi0_der(*ans)))
    print('phis:')
    for phi_i in phi_list:
        print(str(phi_i(*ans)))
    print('phi_ders:')
    for phi_der_i in phi_der_list:
        print(str(phi_der_i(*ans)))

if __name__ == '__main__':
    main()
