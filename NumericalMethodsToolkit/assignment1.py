"""
In this assignment you should interpolate the given function.
"""

import numpy as np
import time
import random
import torch
import assignment2
from scipy.linalg import solve_banded
import  functionUtils
import matplotlib.pyplot as plt
import sampleFunctions



class Assignment1:
    def __init__(self):
        """
        Here goes any one time calculation that need to be made before 
        starting to interpolate arbitrary functions.
        """

        pass

    def interpolate(self, f: callable, a: float, b: float, n: int) -> callable:
        """
        Interpolate the function f in the closed range [a,b] using at most n 
        points. Your main objective is minimizing the interpolation error.
        Your secondary objective is minimizing the running time. 
        The assignment will be tested on variety of different functions with 
        large n values. 
        
        Interpolation error will be measured as the average absolute error at 
        2*n random points between a and b. See test_with_poly() below. 

        Note: It is forbidden to call f more than n times. 

        Note: This assignment can be solved trivially with running time O(n^2)
        or it can be solved with running time of O(n) with some preprocessing.
        **Accurate O(n) solutions will receive higher grades.** 
        
        Note: sometimes you can get very accurate solutions with only few points, 
        significantly less than n. 
        
        Parameters
        ----------
        f : callable. it is the given function
        a : float
            beginning of the interpolation range.
        b : float
            end of the interpolation range.
        n : int
            maximal number of points to use.

        Returns
        -------
        The interpolating function.
        """


        def cubic_interpolation_chavichav():
            def chebyshev_nodes(n, a=-1, b=1):
                # Compute Chebyshev nodes in [-1, 1]
                nodes = np.cos((2 * np.arange(1, n + 1) - 1) / (2 * n) * np.pi)

                # Transform nodes to [a, b]
                if a != -1 or b != 1:
                    nodes = 0.5 * (nodes * (b - a) + (b + a))

                return np.sort(nodes)

            def solve_tridiagonal(a, b, c, d):
                """
                Solves a tridiagonal system using the Thomas algorithm.
                a: lowerdiagonal (n-1 elements)
                b: diagonal (n elements)
                c: upperdiagonal (n-1 elements)
                d: right-hand side (n elements)
                Returns the solution vector x (n elements).
                """
                n = len(d)
                c_prime = np.zeros(n - 1)
                d_prime = np.zeros(n)

                # Forward elimination
                c_prime[0] = c[0] / b[0]
                d_prime[0] = d[0] / b[0]
                for i in range(1, n - 1):
                    denom = b[i] - a[i - 1] * c_prime[i - 1]
                    c_prime[i] = c[i] / denom
                    d_prime[i] = (d[i] - a[i - 1] * d_prime[i - 1]) / denom
                d_prime[n - 1] = (d[n - 1] - a[n - 2] * d_prime[n - 2]) / (b[n - 1] - a[n - 2] * c_prime[n - 2])

                # Back substitution
                x = np.zeros(n)
                x[-1] = d_prime[-1]
                for i in range(n - 2, -1, -1):
                    x[i] = d_prime[i] - c_prime[i] * x[i + 1]

                return x

            def find_points_to_interpolate(f, n, left, right):
                # Generate Chebyshev-spaced nodes including the endpoints
                x_vals = chebyshev_nodes(n - 2, left, right)
                x_vals = np.insert(x_vals, 0, left)
                x_vals = np.append(x_vals, right)
                y_vals = np.array([f(x) for x in x_vals])
                return x_vals, y_vals

            # Generate interpolation points and evaluate f
            p_x, p_y = find_points_to_interpolate(f, n, a, b)

            # Compute segment lengths between nodes
            intervals_length = np.array([p_x[i + 1] - p_x[i] for i in range(n - 1)])

            # find diagonals for matrix
            main_diagonal = np.array([(intervals_length[i - 1] + intervals_length[i]) / 3 for i in range(1, n - 1)])
            lower_diagonal = np.array([intervals_length[i - 1] / 6 for i in range(2, n - 1)])
            upper_diagonal = np.array([intervals_length[i] / 6 for i in range(2, n - 1)])

            # Construct right-hand side vector
            right_side_vector = np.array(
                [(p_y[i + 1] - p_y[i]) / intervals_length[i] - (p_y[i] - p_y[i - 1]) / intervals_length[i - 1] for i in
                 range(1, n - 1)])

            # Solve for second derivatives at internal nodes
            M = solve_tridiagonal(lower_diagonal, main_diagonal, upper_diagonal, right_side_vector)
            M = np.append(M, 0)
            M = np.insert(M, 0, 0)

            def interpolate_func(x):
                # Locate the interval index for x
                idx = np.searchsorted(p_x, x, side='left')
                if idx == len(p_x):
                    idx -= 1
                if p_x[idx] == x:
                    return p_y[idx]

                # Evaluate cubic polynomial using second derivatives
                a = (M[idx] - M[idx - 1]) / (6 * intervals_length[idx - 1])
                b = M[idx - 1] / 2
                c = (p_y[idx] - p_y[idx - 1]) / intervals_length[idx - 1] - (intervals_length[idx - 1] / 6) * (
                        M[idx] + 2 * M[idx - 1])
                d = p_y[idx - 1]

                res = a * ((x - p_x[idx - 1]) ** 3) + b * ((x - p_x[idx - 1]) ** 2) + c * (x - p_x[idx - 1]) + d
                return res

            return interpolate_func

        return cubic_interpolation_chavichav()

##########################################################################


import unittest
from functionUtils import *
from tqdm import tqdm


class TestAssignment1(unittest.TestCase):
        def test_with_poly(self):
            T = time.time()

            ass1 = Assignment1()
            mean_err = 0

            d = 30
            for i in tqdm(range(100)):
                a = np.random.randn(d)

                f = np.poly1d(a)

                ff = ass1.interpolate(f, -10, 10, 1000)

                xs = np.random.random(200)
                err = 0
                for x in xs:
                    yy = ff(x)
                    y = f(x)
                    err += abs(y - yy)

                err = err / 200
                mean_err += err
            mean_err = mean_err / 100

            T = time.time() - T
            print(T)
            print(mean_err)

        def test_with_poly(self):
            import time
            from tqdm import tqdm

            T = time.time()
            print("Testing with multi random polynomials")
            ass1 = Assignment1()
            mean_err = 0

            d = 20
            for i in tqdm(range(100)):
                a = np.random.randn(d)
                f = np.poly1d(a)
                ff = ass1.interpolate(f, -10, 10, 100)
                xs = np.random.random(200)
                xs = (xs - 0.5) * 20
                err = 0
                for x in xs:
                    yy = ff(x)
                    y = f(x)
                    err += abs(y - yy)
                err = err / 200
                mean_err += err
            mean_err = mean_err / 100

            T = time.time() - T
            print(T)
            print("mean error:", mean_err)

        def test_with_poly_restrict_1(self):
            print("test with poly deg 3")
            ass1 = Assignment1()
            f = np.poly1d([7, 4, -2, 0])
            ff = ass1.interpolate(f, -10, 10, 100)
            xs = np.random.random(200)
            xs = (xs - 0.5) * 20
            err = 0
            for x in xs:
                yy = ff(x)
                y = f(x)
                err += abs(y - yy)
            print("mean error:", err / len(xs))

        def test_with_sin(self):
            print("test_with_sin")
            ass1 = Assignment1()
            f = sampleFunctions.sinus()
            ff = ass1.interpolate(f, -100, 100, 100)
            xs = np.random.random(200)
            xs = (xs - 0.5) * 200
            err = 0
            for x in xs:
                yy = ff(x)
                y = f(x)
                err += abs(y - yy)
            print("mean error:", err / len(xs))

        def test_complex_sin_function(self):
            ass1 = Assignment1()

            def f(x):
                return np.sin(x) * np.exp(-0.1 * x)

            ff = ass1.interpolate(f, -10, 10, 100)
            xs = np.random.random(200)
            xs = (xs - 0.5) * 20
            err = 0
            for x in xs:
                yy = ff(x)
                y = f(x)
                err += abs(y - yy)
            print("complex sin:")
            print("mean error:", err / len(xs))

        def test_with_linear_polynomial(self):
            print("Running Test: Linear Polynomial y = 3x + 7")
            ass1 = Assignment1()
            f = np.poly1d([3, 7])
            ff = ass1.interpolate(f, -20, 20, 150)
            xs = np.random.random(300)
            xs = (xs - 0.5) * 40
            err = 0
            for x in xs:
                yy = ff(x)
                y = f(x)
                err += abs(y - yy)
            print("mean error:", err / len(xs))

        def test_with_quadratic_polynomial(self):
            print("Running Test: Quadratic Polynomial y = 2x^2 - 3x + 5")
            ass1 = Assignment1()
            f = np.poly1d([2, -3, 5])
            ff = ass1.interpolate(f, -15, 15, 120)
            xs = np.random.random(250)
            xs = (xs - 0.5) * 30
            err = 0
            for x in xs:
                yy = ff(x)
                y = f(x)
                err += abs(y - yy)
            print("mean error:", err / len(xs))

        def test_with_10th_degree_polynomial(self):
            print("Running Test: 10th-Degree Polynomial")
            ass1 = Assignment1()
            f = np.poly1d([1, -3, 5, -7, 9, -11, 13, -15, 17, -19, 21])
            ff = ass1.interpolate(f, -10, 10, 150)
            xs = np.random.random(300)
            xs = (xs - 0.5) * 20
            err = 0
            for x in xs:
                yy = ff(x)
                y = f(x)
                err += abs(y - yy)
            print("mean error:", err / len(xs))

        def test_with_12th_degree_polynomial(self):
            print("Running Test: 12th-Degree Polynomial")
            ass1 = Assignment1()
            f = np.poly1d([2, -4, 6, -8, 10, -12, 14, -16, 18, -20, 22, -24, 26])
            ff = ass1.interpolate(f, -15, 15, 1000)
            xs = np.random.random(100)
            xs = (xs - 0.5) * 30
            err = 0
            for x in xs:
                yy = ff(x)
                y = f(x)
                err += abs(y - yy)
            print("mean error:", err / len(xs))



if __name__ == "__main__":
    unittest.main()
