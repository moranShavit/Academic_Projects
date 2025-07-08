"""
In this assignment you should fit a model function of your choice to data 
that you sample from a given function. 

The sampled data is very noisy so you should minimize the mean least squares 
between the model you fit and the data points you sample.  

During the testing of this assignment running time will be constrained. You
receive the maximal running time as an argument for the fitting method. You 
must make sure that the fitting function returns at most 5 seconds after the 
allowed running time elapses. If you take an iterative approach and know that 
your iterations may take more than 1-2 seconds break out of any optimization 
loops you have ahead of time.

Note: You are NOT allowed to use any numeric optimization libraries and tools 
for solving this assignment. 

"""

import numpy as np
import time
import random
import torch
import matplotlib.pyplot as plt
from commons import *

class Assignment4:
    def __init__(self):
        """
        Here goes any one time calculation that need to be made before 
        solving the assignment for specific functions. 
        """

        def chebyshev_nodes(n, a=-1, b=1):
            """
            Generate Chebyshev nodes for n points in the interval [a, b].

            Parameters:
            n (int): Number of nodes.
            a (float): Start of the interval. Default is -1.
            b (float): End of the interval. Default is 1.

            Returns:
            numpy.ndarray: Chebyshev nodes in the interval [a, b].
            """
            # Compute Chebyshev nodes in [-1, 1]
            nodes = np.cos((2 * np.arange(1, n + 1) - 1) / (2 * n) * np.pi)

            # Transform nodes to [a, b]
            if a != -1 or b != 1:
                nodes = 0.5 * (nodes * (b - a) + (b + a))

            return np.sort(nodes)

        self.chav_dict = {i: chebyshev_nodes(i, -1, 1) for i in range(1, 100)}

    def fit(self, f: callable, a: float, b: float, d:int, maxtime: float) -> callable:
        """
        Build a function that accurately fits the noisy data points sampled from
        some closed shape. 
        
        Parameters
        ----------
        f : callable. 
            A function which returns an approximate (noisy) Y value given X. 
        a: float
            Start of the fitting range
        b: float
            End of the fitting range
        d: int 
            The expected degree of a polynomial matching f
        maxtime : float
            This function returns after at most maxtime seconds. 

        Returns
        -------
        a function:float->float that fits f between a and b
        """

        def chebyshev_nodes(n, a=-1, b=1):
            """
            Generate Chebyshev nodes for n points in the interval [a, b].

            Parameters:
            n (int): Number of nodes.
            a (float): Start of the interval. Default is -1.
            b (float): End of the interval. Default is 1.

            Returns:
            numpy.ndarray: Chebyshev nodes in the interval [a, b].
            """
            # Compute Chebyshev nodes in [-1, 1]
            nodes = np.cos((2 * np.arange(1, n + 1) - 1) / (2 * n) * np.pi)

            # Transform nodes to [a, b]
            if a != -1 or b != 1:
                nodes = 0.5 * (nodes * (b - a) + (b + a))

            return np.sort(nodes)

        def sample_points(a, b, max_time, sample_num,  f, chav:bool):
            """
           Sample input points and average their noisy outputs over time.

           Parameters:
           a, b : float
               Interval to sample.
           max_time : float
               Max time to perform sampling.
           sample_num : int
               Number of x points to sample.
           f : callable
               Function to evaluate.
           chav : bool
               Whether to use Chebyshev nodes or uniform spacing.

           Returns:
           x_vals : numpy.ndarray
               Sampled x values.
           y_vals : numpy.ndarray
               Averaged noisy outputs corresponding to x_vals.
           """
            start_time = time.time()

            if chav:
                if sample_num not in self.chav_dict.keys():
                    x_vals = chebyshev_nodes(sample_num,a,b)
                else:
                    x_vals = 0.5 * (self.chav_dict[sample_num] * (b - a) + (b + a))
                x_vals = np.insert(x_vals, 0, a)
                x_vals = np.append(x_vals, b)
            else:
                x_vals = np.linspace(a, b, sample_num)
                # x_vals = np.append(x_vals, np.linspace(1.1, b, sample_num))
            conut = 0
            y_vals = y_vals = np.zeros(len(x_vals), dtype=np.float64)
            while True:
                conut += 1
                if (time.time() - start_time) > max_time:
                    break
                for i in range(len(x_vals)):
                    if (time.time() - start_time) > max_time:
                        break
                    temp = f(x_vals[i])
                    y_vals[i] = (y_vals[i]*(conut-1) + temp)/conut
            return x_vals, y_vals

        def calc_coefficient(x_vals, y_vals, degree):
            """
            Calculate polynomial coefficients using least squares fit.

            Parameters:
            x_vals : numpy.ndarray
                Sampled x values.
            y_vals : numpy.ndarray
                Corresponding averaged y values.
            degree : int
                Degree of the polynomial.

            Returns:
            torch.Tensor
                Coefficients of the polynomial.
            """
            y_vals = torch.tensor(y_vals).view(-1, 1)
            x_vals = torch.tensor(x_vals)
            # Construct Vandermonde-like matrix
            T_inv = torch.stack([x_vals ** i for i in reversed(range(degree + 1))])
            T = T_inv.T  # Transpose to get columns
            coeff = ((T_inv.mm(T)).inverse()).mm(T_inv.mm(y_vals))
            return coeff

        # Sample noisy data using Chebyshev nodes and time-budgeted averaging
        x_vals, y_vals = sample_points(a, b, maxtime * 0.85, 3000* d + 1, f, True)
        x_vals = 2 * (x_vals - a) / (b - a) - 1
        coeff = calc_coefficient(x_vals, y_vals, d)
        coeff = coeff.squeeze().numpy()
        res = np.poly1d(coeff)

        def dense_poly(x):
            x = 2 * (x - a) / (b - a) - 1 # Normalize input before applying poly
            return res(x)
        return dense_poly



##########################################################################


import unittest
from sampleFunctions import *
from tqdm import tqdm


def plot_functions(func1, func2, range_start, range_end, num_points=1000, labels=("real function", "fitted function")):
    """
    Plots two functions over a specified range.

    Parameters:
    - func1: First function to plot.
    - func2: Second function to plot.
    - range_start: Start of the range (x-axis).
    - range_end: End of the range (x-axis).
    - num_points: Number of points to sample in the range (default: 1000).
    - labels: Tuple containing labels for the two functions (default: ("Function 1", "Function 2")).
    """
    # Generate x values
    x_values = np.linspace(range_start, range_end, num_points)

    # Compute y values for both functions
    y_values_func1 = func1(x_values)
    y_values_func2 = func2(x_values)

    # Plot the functions
    plt.figure(figsize=(10, 6))
    plt.plot(x_values, y_values_func1, label=labels[0], linewidth=2)
    plt.plot(x_values, y_values_func2, label=labels[1], linestyle='--', linewidth=2)
    plt.axhline(0, color='black', linewidth=0.8, linestyle='--')  # x-axis
    plt.axvline(0, color='black', linewidth=0.8, linestyle='--')  # y-axis
    plt.title("Comparison of Two Functions")
    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.legend()
    plt.grid()
    plt.show()

class TestAssignment4(unittest.TestCase):

    def test_return(self):
        f = NOISY(0.01)(poly(1,1,1))
        ass4 = Assignment4()
        T = time.time()
        shape = ass4.fit(f=f, a=0, b=1, d=10, maxtime=5)
        T = time.time() - T
        self.assertLessEqual(T, 5)

    # def test_delay(self):
    #     f = DELAYED(7)(NOISY(0.01)(poly(1,1,1)))
    #
    #     ass4 = Assignment4()
    #     T = time.time()
    #     shape = ass4.fit(f=f, a=0, b=1, d=10, maxtime=5)
    #     T = time.time() - T
    #     self.assertGreaterEqual(T, 5)

    def test_err(self):
        f = poly(1,1,1)
        nf = NOISY(1)(f)
        ass4 = Assignment4()
        T = time.time()
        ff = ass4.fit(f=nf, a=0, b=1, d=1, maxtime=5)
        T = time.time() - T
        mse=0
        for x in np.linspace(0,1,1000):            
            self.assertNotEquals(f(x), nf(x))
            mse+= (f(x)-ff(x))**2
        mse = mse/1000
        print(mse)

    def test_err_poly_3_case_1(self):
        # Polynomial: f(x) = 3x^3 - 2x^2 + 5x + 1
        f = poly(3, 1, 5, -2, 3)  # Coefficients: [1, 5, -2, 3]
        f = np.poly1d([3, -2, 5, 1])
        nf = NOISY(1)(f)  # Add noise
        ass4 = Assignment4()
        T = time.time()
        ff = ass4.fit(f=nf, a=-1, b=2, d=3, maxtime=5)
        T = time.time() - T
        mse = 0
        for x in np.linspace(-1, 2, 1000):
            self.assertNotEquals(f(x), nf(x))
            mse += (f(x) - ff(x)) ** 2
        mse = mse / 1000
        print("poly_3_case_1 mse: ", mse)
        print("time: ", T)
        plot_functions(f, ff, -1,2, 1000)

    def test_err_poly_3_case_2(self):
        # Polynomial: f(x) = -4x^3 + 3x^2 - x + 2
        f = poly(14, -23, 3, -4)  # Coefficients: [2, -1, 3, -4]
        nf = NOISY(1)(f)  # Add noise
        ass4 = Assignment4()
        T = time.time()
        ff = ass4.fit(f=nf, a=0, b=40, d=3, maxtime=1)
        T = time.time() - T
        mse = 0
        for x in np.linspace(0, 40, 1000):
            self.assertNotEquals(f(x), nf(x))
            mse += (f(x) - ff(x)) ** 2

        mse = mse / 1000
        print("poly_3_case_2 mse: ", mse)
        print("time: ", T)


    def test_err_poly_3_case_3(self):
        # Polynomial: f(x) = x^3 - 6x^2 + 11x - 6
        f = poly(-6, 11, -6, 1)  # Coefficients: [-6, 11, -6, 1]
        nf = NOISY(1)(f)  # Add noise
        ass4 = Assignment4()
        T = time.time()
        ff = ass4.fit(f=nf, a=-2, b=2, d=3, maxtime=0.5)
        T = time.time() - T
        mse = 0
        for x in np.linspace(-2, 2, 1000):
            self.assertNotEquals(f(x), nf(x))
            mse += (f(x) - ff(x)) ** 2
        mse = mse / 1000
        print("poly_3_case_3 mse: ", mse)
        print("time: ", T)


    def test_err_poly_4(self):
        f = poly(4, 3, -2, 1, 5, -2)
        nf = NOISY(1)(f)
        ass4 = Assignment4()
        T = time.time()
        ff = ass4.fit(f=nf, a=-1, b=1,d=4, maxtime=5)
        T = time.time() - T

        mse = 0
        for x in np.linspace(-1, 1, 1000):
            self.assertNotEquals(f(x), nf(x))
            mse += (f(x) - ff(x)) ** 2
        mse = mse / 1000
        print("test_err_poly_4 mse: ", mse)
        print("time: ", T)


    def test_err_poly_5(self):
        f = poly(-4, -5, 6, 2, -2, 1)
        nf = NOISY(1)(f)
        ass4 = Assignment4()
        T = time.time()
        ff = ass4.fit(f=nf, a=-5, b=-1, d=5, maxtime=0.5)
        T = time.time() - T
        mse = 0
        for x in np.linspace(-5, -1, 1000):
            self.assertNotEquals(f(x), nf(x))
            mse += (f(x) - ff(x)) ** 2
        mse = mse / 1000
        print("test_err_poly_5 mse: ", mse)
        print("time: ", T)


    def test_err_poly_6(self):
        f = np.poly1d( [4, 0, 1, 3, -8, 2, 9])
        nf = NOISY(1)(f)
        ass4 = Assignment4()
        T = time.time()
        ff = ass4.fit(f=nf, a=-2, b=2, d=6, maxtime=5)
        T = time.time() - T
        mse = 0
        for x in np.linspace(-2, 2, 1000):
            self.assertNotEquals(f(x), nf(x))
            mse += (f(x) - ff(x)) ** 2
        mse = mse / 1000
        print("test_err_poly_6 mse: ", mse)
        print("time: ", T)

    def test_err_poly_7(self):
        f = np.poly1d([2, 4, -5, 4, -8, 7, 0, 6])
        nf = NOISY(1)(f)
        ass4 = Assignment4()
        T = time.time()
        ff = ass4.fit(f=nf, a=-5, b=5, d=7, maxtime=5)
        T = time.time() - T
        mse = 0
        for x in np.linspace(-5, 5, 1000):
            self.assertNotEquals(f(x), nf(x))
            mse += (f(x) - ff(x)) ** 2
        mse = mse / 1000
        print("test_err_poly_7 mse:", mse)
        print("time:", T)

    def test_err_poly_9(self):
        f = np.poly1d([2, 2, -3, -4, 0, -8, -7, 3, 6, 0])
        nf = NOISY(1)(f)
        ass4 = Assignment4()
        T = time.time()
        ff = ass4.fit(f=nf, a=-2, b=4, d=13, maxtime=5)
        T = time.time() - T
        mse = 0
        for x in np.linspace(-2, 4, 1000):
            self.assertNotEquals(f(x), nf(x))
            mse += (f(x) - ff(x)) ** 2
        mse = mse / 1000
        print("test_err_poly_9 mse:", mse)
        print("time:", T)

    def test_err_test_3_grader(self):
        def harta(x):
            return sin(pow(x, 2))

        f = f3
        nf = f3_noise
        ass4 = Assignment4()
        T = time.time()
        ff = ass4.fit(f=nf, a=-1, b=5, d=20, maxtime=20)
        T = time.time() - T
        mse = 0
        for x in np.linspace(-1, 5, 1000):
            self.assertNotEquals(f(x), nf(x))
            mse += (f(x) - ff(x)) ** 2
        mse = mse / 1000
        print("test_err_test_3_grader mse:", mse)
        print("time:", T)


if __name__ == "__main__":
    unittest.main()
