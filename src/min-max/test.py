from ctypes import CDLL, c_int
lib = CDLL('./src/min-max/test.so')
lib.optimize_algorithm.argtypes = [c_int, c_int]
lib.optimize_algorithm.restype = c_int

result = lib.optimize_algorithm(42, 78)
print(result)
