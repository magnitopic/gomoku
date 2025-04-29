from constants import *


class History:
    def __init__(self):
        self.history = []

    def create_history_file(self, game_config):
        try:
            file = open("history.txt", "w")

            buffer: str = "Game Configuration\n"
            buffer += "------------\n"
            buffer += f"Playing against AI: {game_config["ai"]}\n"
            if game_config["ai"]:
                buffer += f"AI Difficulty: {game_config["difficulty"]}\n"
            buffer += f"Board Size: {game_config["board_size"]}\n"
            buffer += f"Game Mode: {game_config["game_mode"]}\n\n"
            buffer += "Game History\n"
            buffer += "------------\n"

            file.write(buffer)
            for move in self.history:
                file.write(f"{move}\n")
            print(f"{T_GREEN}Game history saved to history.txt{T_GRAY}")
            file.close()
        except Exception as e:
            print(e)
            print(f"{T_RED}Error: Could not write to history file.{T_GRAY}")

    def add_move(self, cell, player):
        move = f"{"Black" if player == 1 else "White"}: {cell[0]}, {cell[1]}"
        self.history.append(move)

    def add_capture(self, current_player, inactive_player):
        move = f"{current_player} captured {inactive_player}'s stones!"
        move += f"\nTotal captured by {current_player}: {current_player.taken_stones}"
        self.history.append(move)

    def add_win(self, player):
        move = f"{player} wins!"
        self.history.append(move)

    def add_tie(self):
        move = "Game ended in a tie!"
        self.history.append(move)
