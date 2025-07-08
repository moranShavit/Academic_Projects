"""
In this assignment you should find the intersection points for two functions.
"""

import numpy as np
import time
import random
from collections.abc import Iterable


class Assignment2:
    def __init__(self):
        """
        Here goes any one time calculation that need to be made before 
        solving the assignment for specific functions. 
        """

        pass

    def intersections(self, f1: callable, f2: callable, a: float, b: float, maxerr=0.001) -> Iterable:
        """
        Find as many intersection points as you can. The assignment will be
        tested on functions that have at least two intersection points, one
        with a positive x and one with a negative x.
        
        This function may not work correctly if there is infinite number of
        intersection points. 


        Parameters
        ----------
        f1 : callable
            the first given function
        f2 : callable
            the second given function
        a : float
            beginning of the interpolation range.
        b : float
            end of the interpolation range.
        maxerr : float
            An upper bound on the difference between the
            function values at the approximate intersection points.


        Returns
        -------
        X : iterable of approximate intersection Xs such that for each x in X:
            |f1(x)-f2(x)|<=maxerr.

        """

        def create_h_function(f, g):
            # Returns a function h(x) = f(x) - g(x)
            def h(x):
                return f(x) - g(x)

            return h

        def brent(f, a, b, tol_x=1e-6, tol_y=maxerr, max_iter=100):
            """
            Brent's method with custom stopping condition based on |f(x)| (y-values).

            Parameters:
                f: Function for which the root is to be found.
                a: Start of the bracketing interval.
                b: End of the bracketing interval.
                tol_x: Convergence tolerance for x-values (optional).
                tol_y: Convergence tolerance for y-values (|f(x)|).
                max_iter: Maximum iterations allowed.

            Returns:
                The approximate root of the function.
            """
            fa, fb = f(a), f(b)
            if fa * fb >= 0:
                raise ValueError("Function values at the endpoints must have opposite signs.")

            #set b to the "better" point
            if abs(fa) < abs(fb):
                a, b = b, a
                fa, fb = fb, fa

            d = c = a
            fc = fa
            last_method_bisect = False #True = bisection  False = otherwise

            for iteration in range(max_iter):
                if abs(fb) < tol_y:  # Check y-value stopping condition
                    return b

                if fb != fc and fa != fc:
                    # Inverse Quadratic Interpolation
                    s = (a * fb * fc) / ((fa - fb) * (fa - fc)) \
                        + (b * fa * fc) / ((fb - fa) * (fb - fc)) \
                        + (c * fa * fb) / ((fc - fa) * (fc - fb))
                else:
                    # Secant Method
                    s = b - fb * (b - a) / (fb - fa)

                # Conditions to revert to bisection
                condition1 = (s < (3 * a + b) / 4 or s > b)
                condition2 = (iteration > 1 and last_method_bisect is True and abs(s - b) >= abs(b - c) / 2)
                condition3 = (iteration > 2 and last_method_bisect is False and abs(s-b) >= abs(c - d) / 2)
                condition4 = (abs(b - c) < tol_x)
                condition5 = (abs(s - b) < tol_x)

                last_method_bisect = False

                if condition1 or condition2 or condition3 or condition4 or condition5:
                    # Bisection step
                    s = (a + b) / 2
                    last_method_bisect = True

                fs = f(s)
                d = c
                c, fc = b, fb

                # update a or b to s depend on the sign
                if fa * fs < 0:
                    b, fb = s, fs
                else:
                    a, fa = s, fs

                # set b to the "better" point
                if abs(fa) < abs(fb):
                    a, b = b, a
                    fa, fb = fb, fa

            raise RuntimeError("Maximum number of iterations reached.")

        def filter_close_roots(roots, tol=1e-3):
            """
            Filter out roots that are too close to each other (optimized).

            Parameters:
                roots (list): List of roots.
                tol (float): Tolerance to consider two roots as the same.

            Returns:
                List of unique roots.
            """
            if not roots:
                return []

            # Sort the roots first (O(n log n))
            roots = sorted(roots)

            # Compare only consecutive roots (O(n))
            unique_roots = [roots[0]]
            for i in range(1, len(roots)):
                if abs(roots[i] - unique_roots[-1]) >= tol:
                    unique_roots.append(roots[i])
            return unique_roots


        def calc_roots(f1, f2, step_size,a,b, depth = 0):
            """
                  Recursively finds root intervals between f1 and f2 within [a, b] using adaptive refinement.

                  Parameters:
                  f1, f2 : callable
                      Functions whose intersections are being computed.
                  step_size : float
                      Initial sampling resolution.
                  a, b : float
                      Start and end of interval.
                  depth : int
                      Current recursion depth to avoid infinite loops.

                  Returns:
                  list of float
                      Estimated intersection points.
              """
            global g, t
            res = []
            h = create_h_function(f1, f2)
            if step_size < 1e-6 or depth > 20:
                return []

            points = np.arange(a, b + step_size, step_size)

            for i in range(len(points)-1):
                # Directly consider values close enough to zero
                if abs(h(points[i])) < maxerr:
                    res.append(points[i])

                # If sign changes between two points, use Brent's method
                elif h(points[i])*h(points[i+1]) < 0 :
                    try:
                        x = brent(h, points[i], points[i+1])
                    except Exception as e:
                        # Refine recursively if Brent fails
                        res.extend(calc_roots(f1, f2, step_size/10, points[i], points[i+1], depth+1))
                    else:
                        res.append(x)

                # Handle edge cases with near-zero values by refining further
                elif abs(h(points[i])) < step_size or abs(h(points[i+1])) < step_size:
                    res.extend(calc_roots(f1, f2, step_size / 10, points[i], points[i + 1], depth + 1))

            # Check for duplicates or very close points and refine further if needed
            for i in range(len(res)-1):
                if len(res) >= 2:
                    if abs(res[i] - res[i+1]) < 2 * step_size:
                        res += calc_roots(f1, f2, step_size / 10, res[i] + step_size / 10, res[i+1] - step_size / 10)
            return res

        step_size = abs(a-b)/1000
        roots = calc_roots(f1, f2, step_size, a, b)
        roots = filter_close_roots(roots)
        return roots


##########################################################################


import unittest
from sampleFunctions import *
from tqdm import tqdm

class TestAssignment2(unittest.TestCase):

    def test_sqr(self):

        ass2 = Assignment2()

        f1 = np.poly1d([-1, 0, 1])
        f2 = np.poly1d([1, 0, -1])

        X = ass2.intersections(f1, f2, -1, 1, maxerr=0.001)
        print("test_sqr: ",len(X))
        for x in X:
            self.assertGreaterEqual(0.001, abs(f1(x) - f2(x)))

    def test_poly(self):

        ass2 = Assignment2()

        f1, f2 = randomIntersectingPolynomials(10)

        X = ass2.intersections(f1, f2, -1, 1, maxerr=0.001)
        print("test_random_poly: ",len(X))
        for x in X:
            self.assertGreaterEqual(0.001, abs(f1(x) - f2(x)))

    def test_sin(self):
        ass2 = Assignment2()

        f1 = np.poly1d([0])

        f2 = sinus()

        X = ass2.intersections(f1, f2, -10,10, maxerr=0.001)
        print("test_sin: ",len(X))
        for x in X:
            self.assertGreaterEqual(0.001, abs(f1(x) - f2(x)))

    def test_my_pol(self):
        ass2 = Assignment2()

        f1 = np.poly1d([5, -2, 1, 0, -7, 3,0])

        f2 = np.poly1d([0])

        X = ass2.intersections(f1, f2, -10, 10, maxerr=0.001)
        print("test_my_pol: ",len(X))
        for x in X:
            self.assertGreaterEqual(0.001, abs(f1(x) - f2(x)))

    def test_my_pol_2(self):
        ass2 = Assignment2()

        f1 = np.poly1d([0.2, 8, 0, -7, 0, 0,0,0.5 ])

        f2 = np.poly1d([0])

        X = ass2.intersections(f1, f2, -10, 10, maxerr=0.001)
        print("test_my_pol_2: ",len(X))
        for x in X:
            self.assertGreaterEqual(0.001, abs(f1(x) - f2(x)))


    def test_strong_oscilations(self):
        ass2 = Assignment2()

        f1 = strong_oscilations()

        f2 = np.poly1d([0])

        X = ass2.intersections(f1, f2, -10, -0.07, maxerr=0.001)
        print("test_strong_oscilations: ",len(X))
        for x in X:
            self.assertGreaterEqual(0.001, abs(f1(x) - f2(x)))

    def test_sin_power_3(self):
        ass2 = Assignment2()

        f1 = np.poly1d([0])

        def f2(x):
            return np.sin(x**3)

        X = ass2.intersections(f1, f2, 8.4, 8.8, maxerr=0.001)

        print("test_sin_power_3: ",len(X))
        for x in X:
            self.assertGreaterEqual(0.001, abs(f1(x) - f2(x)))

    def test_sin_cos(self):
        ass2 = Assignment2()

        f1 = np.poly1d([0])

        def f2(x):
            return (5*np.sin(x*6) - np.cos(x/3)*x + x +3.2)

        X = ass2.intersections(f1, f2, -20, 20, maxerr=0.001)

        print("test_sin_cos: ",len(X))
        for x in X:
            self.assertGreaterEqual(0.001, abs(f1(x) - f2(x)))

if __name__ == "__main__":
    unittest.main()
