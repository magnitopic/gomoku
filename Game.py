import pygame
from time import sleep
from constants import *
from board_validations import *


class Game:
    def __init__(self):
        self.board = [[0 for _ in range(COLS)] for _ in range(ROWS)]
        self.turn = 1

        # Pygame vars
        self.screen = pygame.display.set_mode((BOARD_SIZE, BOARD_SIZE))
        pygame.display.set_caption("gomoku")
        self.font = pygame.font.SysFont("Arial", 30)

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
        star_points = [3, 9, 15]
        for x in star_points:
            for y in star_points:
                pygame.draw.circle(
                    self.screen, BLACK,
                    (MARGIN + x * CELL_SIZE, MARGIN + y * CELL_SIZE),
                    4
                )

        pygame.display.flip()

    def draw_stone(self, cell, color):
        pygame.draw.circle(
            self.screen, color,
            (MARGIN + cell[0] * CELL_SIZE, MARGIN + cell[1] * CELL_SIZE),
            CELL_SIZE // 2 - 2
        )
        pygame.display.flip()

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
        sleep(2)

    """ game logic """

    def handle_turn(self, cell) -> bool:
        col, row = cell

        # Check if the cell is already occupied
        if self.board[row][col] != 0:
            return True

        self.board[row][col] = self.turn

        # Check if it's a winning move
        if check_win(self.board, cell, self.turn):
            self.draw_win_screen()
            return False

        # Draw the stone
        self.draw_stone(cell, BLACK if self.turn == 1 else WHITE)
        self.turn = -self.turn

        return True
