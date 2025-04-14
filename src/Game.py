import pygame
import time
from time import sleep
from constants import *
from board_validations import *
from Player import Player


class Game:
    def __init__(self):
        self.board = [[0 for _ in range(COLS)] for _ in range(ROWS)]
        self.turn = 1
        self.player1 = Player(1)    # Player 1 is black
        self.player2 = Player(-1, True)   # Player 2 is white
        self.turn_start_time = time.time()

        # Pygame vars
        self.screen = pygame.display.set_mode((BOARD_SIZE, BOARD_SIZE))
        pygame.display.set_caption("gomoku")
        self.font = pygame.font.SysFont("Arial", 30)
        self.font_small = pygame.font.SysFont("Arial", 20)
        self.font_underline = pygame.font.SysFont("Arial", 30)
        self.font_underline.set_underline(True)

    """  Draw functions """

    def draw_board(self):
        self.screen.fill(WOOD)

        # Draw border around the board
        border_rect = pygame.Rect(
            MARGIN - BORDER_MARGIN,
            MARGIN - BORDER_MARGIN,
            GRID_SIZE * CELL_SIZE + 2 * BORDER_MARGIN,
            GRID_SIZE * CELL_SIZE + 2 * BORDER_MARGIN
        )
        pygame.draw.rect(self.screen, BORDER_COLOR, border_rect, 2)

        # Draw the grid lines (19 vertical and 19 horizontal lines for intersections)
        for i in range(COLS):
            pygame.draw.line(
                self.screen, BLACK,
                (MARGIN + i * CELL_SIZE, MARGIN),
                (MARGIN + i * CELL_SIZE, MARGIN + GRID_SIZE * CELL_SIZE),
                2
            )

        for i in range(ROWS):
            pygame.draw.line(
                self.screen, BLACK,
                (MARGIN, MARGIN + i * CELL_SIZE),
                (MARGIN + GRID_SIZE * CELL_SIZE, MARGIN + i * CELL_SIZE),
                2
            )

        # Draw star points
        star_points = [x for x in range(3, COLS, 6)]
        for x in star_points:
            for y in star_points:
                pygame.draw.circle(
                    self.screen, BLACK,
                    (MARGIN + x * CELL_SIZE, MARGIN + y * CELL_SIZE),
                    4
                )

        pygame.display.flip()

    def draw_player_info(self):
        player1_pos = (10, 10)
        player2_pos = (10, BOARD_SIZE - 50)

        player1_rect = pygame.Rect(
            player1_pos[0], player1_pos[1], BOARD_SIZE, 50)
        player2_rect = pygame.Rect(
            player2_pos[0], player2_pos[1], BOARD_SIZE, 50)

        self.screen.fill(WOOD, player1_rect)
        self.screen.fill(WOOD, player2_rect)

        if self.turn == 1:
            text_to_show1 = self.screen.blit(
                self.font_underline.render("Player1", True, BLACK), player1_pos)
            text_to_show2 = self.screen.blit(
                self.font.render("Player2", True, WHITE), player2_pos)
        else:
            text_to_show1 = self.screen.blit(
                self.font.render("Player1", True, BLACK), player1_pos)
            text_to_show2 = self.screen.blit(
                self.font_underline.render("Player2", True, WHITE), player2_pos)

        timer1 = self.screen.blit(self.font_small.render(
            f"Time: {self.player1.timer:.3f}s", True, BLACK), (BOARD_SIZE - 140, 10))
        timer2 = self.screen.blit(self.font_small.render(
            f"Time: {self.player2.timer:.3f}s", True, WHITE), (BOARD_SIZE - 140, BOARD_SIZE - 50))

        stones_counter1 = self.screen.blit(self.font_small.render(
            f"Taken: {self.player1.taken_stones}", True, BLACK), (BOARD_SIZE - 250, 10))
        stones_counter2 = self.screen.blit(self.font_small.render(
            f"Taken: {self.player2.taken_stones}", True, WHITE), (BOARD_SIZE - 250, BOARD_SIZE - 50))

        pygame.display.update([player1_rect, player2_rect, text_to_show1,
                              text_to_show2, timer1, timer2, stones_counter1, stones_counter2])

    def draw_stone(self, cell, color):
        pygame.draw.circle(
            self.screen, color,
            (MARGIN + cell[0] * CELL_SIZE, MARGIN + cell[1] * CELL_SIZE),
            CELL_SIZE // 2 - 2
        )
        pygame.display.flip()

    def draw_all_stones(self):
        for y, value in enumerate(self.board):
            for x, cell_value in enumerate(value):
                cell = (x, y)
                if (cell_value != 0):
                    self.draw_stone(cell, BLACK if cell_value == 1 else WHITE)

    def draw_win_screen(self):
        self.screen.fill(WOOD)
        pos = (BOARD_SIZE/2 - 70, BOARD_SIZE/2)
        self.screen.blit(self.font.render("Game Over", True, RED), pos)
        if self.turn == 1:
            self.screen.blit(self.font.render("Black wins!", True, BLACK),
                             (pos[0], pos[1] - 50))
        else:
            self.screen.blit(self.font.render("White wins!", True, WHITE),
                             (pos[0], pos[1] - 50))
        pygame.display.flip()
        player = "Black" if self.turn == 1 else "White"
        print(f"{T_GREEN}{player} player wins!!")
        sleep(2)

    def draw_tie_screen(self):
        self.screen.fill(WOOD)
        pos = (BOARD_SIZE/2 - 70, BOARD_SIZE/2)
        self.screen.blit(self.font.render("Game Over", True, RED), pos)
        self.screen.blit(self.font.render("Game Tie!", True, RED),
                         (pos[0], pos[1] - 50))
        pygame.display.flip()
        print(f"{T_GREEN}Tie reached!")
        sleep(2)

    """ game logic """

    def handle_capture(self, stones_to_delete) -> bool:
        for stone in stones_to_delete:
            self.board[stone[1]][stone[0]] = 0
        self.draw_board()
        self.draw_all_stones()
        if self.turn == 1:
            if self.player1.handle_taken_stones():
                return True
        else:
            if self.player2.handle_taken_stones():
                return True
        return False

    def handle_ai_turn(self):
        result = self.player2.new_ai_move(self.board)
        """ print(f"{T_YELLOW}AI move: {result[0]}, {result[1]}{T_GRAY}") """

        self.player2.timer = result[3]

        self.turn = 1
        self.turn_start_time = time.time()

        if result[0] == -1 and result[1] == -1:
            return False

        self.board[result[0]][result[1]] = -1
        self.draw_stone((result[0], result[1]), WHITE)

    def handle_turn(self, cell) -> bool:
        # x, y = cell
        col, row = cell

        # Calculate time taken for this move
        current_time = time.time()
        time_taken = current_time - self.turn_start_time

        # Update the appropriate player's timer
        if self.turn == 1:
            self.player1.timer = time_taken
        else:
            if not self.player2.ai:
                self.player2.timer = time_taken

        # Check if the cell is already occupied
        if self.board[row][col] != 0:
            print(f"{T_BLUE}Can't place on an occupied tile! Invalid move!{T_GRAY}")
            return True

        # Check for double three
        if check_double_three(self.board, cell, self.turn):
            print(f"{T_PURPLE}Double three detected! Invalid move!{T_GRAY}")
            return True

        """ # Check if new position moves into capture
        if check_move_into_capture(self.board, cell, self.turn):
            print(f"{T_RED}Can't move into capture! Invalid move!{T_GRAY}")
            return True """

        self.board[row][col] = self.turn

        # Check if there is a capture
        capture_check_result = check_capture(self.board, cell, self.turn)
        if capture_check_result:
            player = "Black" if self.turn == -1 else "White"
            print(f"{T_CYAN}{player}'s stones were captured!{T_GRAY}")
            if self.handle_capture(capture_check_result):
                self.draw_win_screen()
                return False

        # Check if it's a winning move
        if check_win(self.board, cell, self.turn):
            self.draw_win_screen()
            return False

        # Draw the stone
        self.draw_stone(cell, BLACK if self.turn == 1 else WHITE)
        self.turn = -self.turn

        self.turn_start_time = time.time()

        # Check if the board is full
        if check_board_full(self.board):
            self.draw_tie_screen()
            return False

        if self.turn == -1 and self.player2.ai:
            self.handle_ai_turn()

        self.draw_player_info()

        return True
