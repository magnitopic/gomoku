import ctypes
import sys
from constants import *


class Player:
    def __init__(self, color, ai=False):
        self.ai = ai
        self.taken_stones = 0
        self.timer = 0
        self.color = color
        self.lib = None
        self.board = None

        if self.ai:
            class t_move(ctypes.Structure):
                _fields_ = [("row", ctypes.c_int),
                            ("col", ctypes.c_int),
                            ("score", ctypes.c_int),
                            ("time_taken", ctypes.c_double)]

            try:
                self.lib = ctypes.CDLL('./src/min-max/min-max.so')
            except:
                print(
                    f"{T_RED}Error: Could not load algorithm file. Please ensure it exists.{T_RESET}")
                exit()

            # Configure function prototype
            self.lib.ai_algorithm.argtypes = [
                ctypes.c_int,                          # board_size
                ctypes.POINTER(ctypes.POINTER(ctypes.c_int)),  # int** board
                ctypes.c_int                           # color
            ]
            self.lib.ai_algorithm.restype = t_move

            self.board = [[0 for _ in range(COLS)] for _ in range(ROWS)]

    def handle_taken_stones(self):
        self.taken_stones += 2
        if self.taken_stones == 10:
            return True
        return False

    def new_ai_move(self):
        c_rows = (ctypes.POINTER(ctypes.c_int) * ROWS)()
        for i, row in enumerate(self.board):
            c_rows[i] = (ctypes.c_int * COLS)(*row)
        color = -1 if self.color == 1 else 1

        result = self.lib.ai_algorithm(
            ctypes.c_int(19),
            c_rows,
            ctypes.c_int(color)
        )

        return [
            result.row, result.col, result.score, result.time_taken
        ]
