from constants import *


class History:
    def __init__(self):
        self.history = []

    def create_history_file(self):
        try:
            file = open("history.txt", "w")
            file.write("Game History\n")
            file.write("------------\n")
            for move in self.history:
                file.write(f"{move}\n")
            print(f"{T_GREEN}Game history saved to history.txt{T_GRAY}")
            file.close()
        except:
            print(f"{T_RED}Error: Could not write to history file.{T_GRAY}")

    def add_move(self, cell, player):
        move = f"{"Black" if player == 1 else "White"}: {cell[0]}, {cell[1]}"
        self.history.append(move)

    def add_capture(self, player, total_captured):
        move = f"{"White" if player == "Black" else "Black"} captured {player}'s stones!"
        self.history.append(move)
