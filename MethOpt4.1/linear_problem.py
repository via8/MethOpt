class LinearProblem:
    def __init__(self, n: int, m: int):
        self.N = n
        self.M = m
        self._c = [0.0] * n
        self._A = [[0.0] * n] * m
        self._b = [0.0] * n
        self._signs = [' = '] * m  # ' = ' or ' >= ' or ' <= '

    @property
    def n(self): return self.N

    @property
    def m(self): return self.M

    @property
    def c(self): return self._c

    @property
    def A(self): return self._A

    @property
    def b(self): return self._b

    @property
    def signs(self): return self._signs

    @c.setter
    def c(self, c):
        assert(len(c) == self.N)
        for i in range(0, self.N):
            assert(isinstance(c[i], float))
        self._c = c

    @A.setter
    def A(self, A: [[]]):
        assert(len(A) == self.M)
        assert(len(A[0]) == self.N)
        for i in range(0, self.M):
            for j in range(0, self.N):
                assert(isinstance(A[i][j], float))
        self._A = A

    @b.setter
    def b(self, b: []):
        assert(len(b) == self.M)
        for i in range(0, self.M):
            assert(isinstance(b[i], float))
        self._b = b

    @signs.setter
    def signs(self, signs: []):
        assert(len(signs) == self.M)
        for i in range(0, self.M):
            assert(signs[i] == ' = ' or signs[i] == ' >= ' or signs[i] == ' <= ')
        self._signs = signs
