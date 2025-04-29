import time
from Player import Player
from constants import *
from board_validations import *
import constants
from History import History
from Screen import Screen


class GameLogic:
    def __init__(self, game_config: dict[str, str | bool]):
        self.board = [[0 for _ in range(constants.COLS)]
                      for _ in range(constants.ROWS)]

        self.player1 = Player(1)                        # Player 1 is black
        self.player2 = Player(-1, game_config["ai"])    # Player 2 is white
        self.current_player = self.player1
        self.inactive_player = self.player2

        self.current_player.color_start_time = time.time()
        self.save_history = game_config["save_history"]
        if self.save_history:
            self.game_history = History()

        # When game launches, draw the board
        self.screen = Screen()
        self.screen.draw_board()
        self.screen.draw_player_info(
            self.current_player.num, self.player1, self.player2)

    def handle_capture(self, stones_to_delete) -> bool:
        for stone in stones_to_delete:
            self.board[stone[1]][stone[0]] = 0
        self.screen.draw_board()
        self.screen.draw_all_stones(self.board)
        if self.current_player.num == 1:
            if self.player1.handle_taken_stones():
                return True
        else:
            if self.player2.handle_taken_stones():
                return True
        return False

    def check_capture(self, cell):
        capture_check_result = check_capture(
            self.board, cell, self.current_player.num)
        if capture_check_result:
            player = self.current_player.num
            print(f"{T_CYAN}{player}'s stones were captured!{T_GRAY}")
            if self.save_history:
                self.game_history.add_capture(
                    self.current_player, self.inactive_player)
            if self.handle_capture(capture_check_result):
                self.screen.draw_win_screen()
                return False
        return True

    def check_game_continue(self, cell):
        # Check if there is a win
        if check_win(self.board, cell, self.current_player.num):
            self.screen.draw_win_screen(self.current_player.name)
            if self.save_history:
                self.game_history.add_win(self.current_player.name)
            print(f"{T_GREEN}{self.current_player} player wins!!")
            return False

        # Check if the board is full
        if check_board_full(self.board):
            if self.save_history:
                self.game_history.add_tie()
            print(f"{T_YELLOW}It's a tie!{T_GRAY}")
            self.screen.draw_tie_screen()
            return False

        return True

    def apply_move(self, cell):
        col, row = cell
        self.board[row][col] = self.current_player.num

        if self.save_history:
            self.game_history.add_move(cell, self.current_player.num)

        self.check_capture(cell)

        if not self.check_game_continue(cell):
            return False

        self.screen.draw_stone(
            cell, BLACK if self.current_player.num == 1 else WHITE)

        # Change active player
        temp = self.current_player
        self.current_player = self.inactive_player
        self.inactive_player = temp

        self.screen.draw_player_info(
            self.current_player.num, self.player1, self.player2)

        return True

    def handle_turn(self, cell):
        # Checks (to be implemented)

        return self.apply_move(cell)

    """ def handle_ai_turn(self):
        result = self.player2.new_ai_move(self.board)
        print(f"{T_YELLOW}AI move: {result[0]}, {result[1]}{T_GRAY}")

        self.player2.timer = result[3]

        self.current_player.num= 1
        self.current_player.color_start_time = time.time()

        if result[0] == -1 and result[1] == -1:
            return False

        self.board[result[0]][result[1]] = -1
        self.draw_stone((result[1], result[0]), WHITE)

    def handle_turn(self, cell) -> bool:
        # x, y = cell
        col, row = cell

        # Calculate time taken for this move
        current_time = time.time()
        time_taken = current_time - self.current_player.color_start_time

        # Update the appropriate player's timer
        if self.current_player.num== 1:
            self.player1.timer = time_taken
        else:
            if not self.player2.ai:
                self.player2.timer = time_taken

        # Check if the cell is already occupied
        if self.board[row][col] != 0:
            print(f"{T_BLUE}Can't place on an occupied tile! Invalid move!{T_GRAY}")
            return True

        # Check for double three
        if check_double_three(self.board, cell, self.current_player.num):
            print(f"{T_PURPLE}Double three detected! Invalid move!{T_GRAY}")
            return True

        # Check if new position moves into capture
        if check_move_into_capture(self.board, cell, self.current_player.num):
            print(f"{T_RED}Can't move into capture! Invalid move!{T_GRAY}")
            return True



        self.current_player.color_start_time = time.time()

        return True
 """
