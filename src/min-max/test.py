# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.py                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/31 00:10:06 by alaparic          #+#    #+#              #
#    Updated: 2025/03/31 00:22:33 by alaparic         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import ctypes
import random

# Define t_move structure equivalent


class t_move(ctypes.Structure):
    _fields_ = [("row", ctypes.c_int),
                ("col", ctypes.c_int),
                ("score", ctypes.c_int),
                ("time_taken", ctypes.c_double)]


# Load shared library
lib = ctypes.CDLL('./src/min-max/test.so')  # Replace with actual library path

# Configure function prototype
lib.ai_algorithm.argtypes = [
    ctypes.c_int,                          # board_size
    ctypes.POINTER(ctypes.POINTER(ctypes.c_int)),  # int** board
    ctypes.c_int                           # color
]
lib.ai_algorithm.restype = t_move

# Prepare board (example 19x19 board)
ROWS = COLS = 19
board = [[0 for _ in range(COLS)] for _ in range(ROWS)]

def new_move():
    # Convert 2D Python list to C-compatible int**
    c_rows = (ctypes.POINTER(ctypes.c_int) * ROWS)()
    for i, row in enumerate(board):
        c_rows[i] = (ctypes.c_int * COLS)(*row)
    
    # Call C function
    color = 1  # or -1
    result = lib.ai_algorithm(
        ctypes.c_int(19),
        c_rows,
        ctypes.c_int(color)
    )
    
    print(f"Best move: ({result.row}, {result.col}) with score {result.score}")
    print(f"Time taken: {result.time_taken:.4f} seconds")
    return result

if __name__ == "__main__":
    while True:
        row = int(input("Enter row: "))
        col = int(input("Enter col: "))
        board[row][col] = 1
        result = new_move()
        board[result.row][result.col] = -1
        if (result.row == -1 and result.col == -1):
            break
        print()
        for row in board:
            for cell in row:
                if cell == 0:
                    print(".", end=" ")
                elif cell == 1:
                    print("X", end=" ")
                else:
                    print("O", end=" ")
            print()
        print()
    