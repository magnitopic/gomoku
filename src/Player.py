import ctypes
from constants import *
import constants


class Player:
    def __init__(self, color, ai=False):
        self.ai = ai
        self.taken_stones = 0
        self.timer = 0
        self.color = color
        self.name = "Black" if color == 1 else "White"
        self.lib = None

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

    def handle_taken_stones(self):
        self.taken_stones += 2
        if self.taken_stones == 10:
            return True
        return False

    def new_ai_move(self, board):
        c_rows = (ctypes.POINTER(ctypes.c_int) * constants.ROWS)()
        for i, row in enumerate(board):
            c_rows[i] = (ctypes.c_int * constants.ROWS)(*row)
        color = -1 if self.color == 1 else 1

        print(T_WHITE)
        result = self.lib.ai_algorithm(
            ctypes.c_int(constants.ROWS),
            c_rows,
            ctypes.c_int(color)
        )
        print(T_GRAY)

        return [
            result.row, result.col, result.score, result.time_taken
        ]
