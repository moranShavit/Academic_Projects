"""
In this assignment you should find the area enclosed between the two given functions.
The rightmost and the leftmost x values for the integration are the rightmost and 
the leftmost intersection points of the two functions. 

The functions for the numeric answers are specified in MOODLE. 


This assignment is more complicated than Assignment1 and Assignment2 because: 
    1. You should work with float32 precision only (in all calculations) and minimize the floating point errors. 
    2. You have the freedom to choose how to calculate the area between the two functions. 
    3. The functions may intersect multiple times. Here is an example: 
        https://www.wolframalpha.com/input/?i=area+between+the+curves+y%3D1-2x%5E2%2Bx%5E3+and+y%3Dx
    4. Some of the functions are hard to integrate accurately. 
       You should explain why in one of the theoretical questions in MOODLE. 

"""

import numpy as np
import time
import random
import scipy
from math import e
from assignment2 import *
from commons import *

class Assignment3:
    def __init__(self):
        """
        Here goes any one time calculation that need to be made before 
        solving the assignment for specific functions. 
        """

        pass

    def integrate(self, f: callable, a: float, b: float, n: int) -> np.float32:
        """
        Integrate the function f in the closed range [a,b] using at most n 
        points. Your main objective is minimizing the integration error. 
        Your secondary objective is minimizing the running time. The assignment
        will be tested on variety of different functions. 
        
        Integration error will be measured compared to the actual value of the 
        definite integral. 
        
        Note: It is forbidden to call f more than n times. 
        
        Parameters
        ----------
        f : callable. it is the given function
        a : float
            beginning of the integration range.
        b : float
            end of the integration range.
        n : int
            maximal number of points to use.

        Returns
        -------
        np.float32
            The definite integral of f between a and b
        """
        def trapozidal(intervals):
            # Applies the trapezoidal rule over the given intervals.
            intervals_area = np.empty((len(intervals)-1, 1), dtype=np.float32)

            x_diff = intervals[1][0] - intervals[0][0] # Differences in x-coordinates
            y_sum = intervals[:-1, 1] + intervals[1:, 1]  # Sum of adjacent y-values

            intervals_area = 0.5 * x_diff * y_sum
            return np.sum(intervals_area)

        def simpson(intervals):
            # Applies Simpson's rule assuming an odd number of points.
            intervals_area = np.empty(((len(intervals)-1)//2, 1), dtype=np.float32)
            y0 = intervals[:-1:2, 1]
            y1 = intervals[1::2, 1]
            y2 = intervals[2::2, 1]
            h = intervals[1][0] - intervals[0][0] # Differences in x-coordinates

            intervals_area = (h/3)*(y0 + 4*y1 + y2)
            return np.sum(intervals_area)

        def simpsons_3_8(intervals):
            # Applies Simpson's 3/8 rule on exactly 4 points.
            h = intervals[1][0] - intervals[0][0]
            res = (3*h/8)*(intervals[0][1] + 3*intervals[1][1] + 3*intervals[2][1] + intervals[3][1])
            return res


        def calc(intervals):
            # Chooses the appropriate integration method based on the number of intervals.
            if len(intervals) < 3:
                return trapozidal(intervals)
            else:
                res = 0
                if len(intervals) % 2 == 0:
                    # For even number of points, apply Simpson's and 3/8 or fallback
                    if len(intervals) > 4:
                        res += simpson(intervals[:-3])
                        res += simpsons_3_8(intervals[-4:])
                    else:
                        res += simpson(intervals[:-1])
                        res += trapozidal(intervals[-2:])
                else:
                    # For odd number of points, apply Simpson's rule directly
                    res += simpson(intervals)
                return res

        # Generate n points in [a, b] and evaluate f at those points
        x_vals = np.linspace(a, b, n)
        y_vals = np.array([f(x) for x in x_vals])
        intervals = np.column_stack((x_vals, y_vals))

        # Perform the integration using a mix of numerical methods
        res = calc(intervals)
        return np.float32(res)








    def areabetween(self, f1: callable, f2: callable) -> np.float32:
        """
        Finds the area enclosed between two functions. This method finds 
        all intersection points between the two functions to work correctly. 
        
        Example: https://www.wolframalpha.com/input/?i=area+between+the+curves+y%3D1-2x%5E2%2Bx%5E3+and+y%3Dx

        Note, there is no such thing as negative area. 
        
        In order to find the enclosed area the given functions must intersect 
        in at least two points. If the functions do not intersect or intersect 
        in less than two points this function returns NaN.  
        This function may not work correctly if there is infinite number of 
        intersection points. 
        

        Parameters
        ----------
        f1,f2 : callable. These are the given functions

        Returns
        -------
        np.float32
            The area between function and the X axis

        """

        ass2 = Assignment2()
        intersaction_points = list(ass2.intersections(f1, f2, 1, 100))

        if len(intersaction_points) < 2:
            return None # Not enough points to form an enclosed area

        area_sum = 0

        for i in range(len(intersaction_points) - 1):
            # Determine number of points to use for integration based on distance between roots
            dist = intersaction_points[i + 1] - intersaction_points[i]
            dist *= 101
            if dist < 101:
                dist = 101 # Minimum number of points to ensure accuracy
            dist = int(dist)

            # Determine which function is on top between the two intersection points
            mid_val = (intersaction_points[i] + intersaction_points[i + 1]) / 2
            sign = 1 if f1(mid_val) > f2(mid_val) else -1

            def diff_func(x):
                return sign * (f1(x) - f2(x))

            # Integrate the signed difference to compute the positive area
            area_sum += self.integrate(diff_func, intersaction_points[i], intersaction_points[i + 1], dist)

        return np.float32(area_sum)



##########################################################################


import unittest
from sampleFunctions import *
from tqdm import tqdm


class TestAssignment3(unittest.TestCase):

    def test_integrate_float32(self):
        ass3 = Assignment3()
        f1 = np.poly1d([-1, 0, 1])
        r = ass3.integrate(f1, -10, 10, 1000)
        self.assertEquals(r.dtype, np.float32)
        print(r)

    def test_integrate_hard_case(self):
        ass3 = Assignment3()
        f1 = strong_oscilations()
        r = ass3.integrate(f1, 0.09, 10, 20)
        print(r)
        true_result = 7.78662 * 10 ** 33
        self.assertGreaterEqual(0.001, abs((r - true_result) / true_result))

    def test_integrate_float32_poly(self):
        ass3 = Assignment3()
        f1 = np.poly1d([-1, 2, 0, -1,0])
        x = np.linspace(-20, 100, 30)
        y = f1(x)
        r_spicy = scipy.integrate.simpson(y , x)
        T1 = time.time()
        r1 = ass3.integrate(f1, -20, 100, 30)
        T1 = time.time() - T1
        print("time: ", T1, "res: ", r1)
        print("error: ",abs(r_spicy-r1))

    def test_integrate_float32_f2(self):
        ass3 = Assignment3()
        f = f2
        r = ass3.integrate(f, 2, 10, 10)
        print(r)
    def test_area_between_poly(self):
        ass3 = Assignment3()
        f1 = np.poly1d([-1, 2, 0, -1,0])
        f2 = np.poly1d([1, -2, 0, 1, 0])
        T1 = time.time()
        r = ass3.areabetween(f1,f2)
        print("Time: ", time.time() - T1)
        print(r)

    def test_area_between_parobala(self):
        ass3 = Assignment3()
        f1 = np.poly1d([1, -8, 13])
        f2 = np.poly1d([-1, 8, -13])
        T = time.time()
        r = ass3.areabetween(f1, f2)
        error  = abs(r-13.856406)
        time_var = time.time() - T
        print("Time: ", time.time() - T)
        print(r)
        print("score: ", (time_var+1)*error/2)

    def test_area_between_sinus_cosinus(self):
        ass3 = Assignment3()
        f1 = sinus()
        def f2(x):
            return np.cos(x)
        T = time.time()
        r = ass3.areabetween(f1,f2)
        error = abs(r -84.85282)
        time_var = time.time() - T
        print("time: ", time_var)
        print(r)
        print("score: ", (time_var+1)*error/2)

    def test_area_between_sinus_pow_log(self):
        ass3 = Assignment3()
        T = time.time()
        r = ass3.areabetween(f3,f10)
        print("time: ", time.time() - T)
        print(r)

    def test_area_between_1(self):
        ass3 = Assignment3()
        T = time.time()
        r = ass3.areabetween(f3,f10)
        time_var = time.time() - T
        error = abs(r - 28.255707)
        print("time: ", time_var)
        print(r)

        print("score: ", (time_var+1)*error/2)


    def test_integrate_float32_e_pow_x(self):
        ass3 = Assignment3()
        def f1(x):
            return 5*pow(e, -1 * pow(x, 2))
        r = ass3.integrate(f1, 0, 4, 8)
        print(r)

if __name__ == "__main__":
    unittest.main()
