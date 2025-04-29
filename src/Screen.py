import pygame
from time import sleep
from constants import *
import constants


class Screen:
    def __init__(self):
        # Pygame vars
        self.screen = pygame.display.set_mode((BOARD_SIZE, BOARD_SIZE))
        pygame.display.set_caption("Gomoku")
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
            constants.GRID_SIZE * constants.CELL_SIZE + 2 * BORDER_MARGIN,
            constants.GRID_SIZE * constants.CELL_SIZE + 2 * BORDER_MARGIN
        )
        pygame.draw.rect(self.screen, BORDER_COLOR, border_rect, 2)

        # Draw the grid lines
        for i in range(constants.COLS):
            pygame.draw.line(
                self.screen, BLACK,
                (MARGIN + i * constants.CELL_SIZE, MARGIN),
                (MARGIN + i * constants.CELL_SIZE, MARGIN +
                 constants.GRID_SIZE * constants.CELL_SIZE),
                2
            )

        for i in range(constants.ROWS):
            pygame.draw.line(
                self.screen, BLACK,
                (MARGIN, MARGIN + i * constants.CELL_SIZE),
                (MARGIN + constants.GRID_SIZE * constants.CELL_SIZE,
                 MARGIN + i * constants.CELL_SIZE),
                2
            )

        # Draw star points
        if constants.COLS == 15:
            star_points = [3, 7, 11]
        else:
            star_points = [3, 9, 15]
        for x in star_points:
            for y in star_points:
                pygame.draw.circle(
                    self.screen, BLACK,
                    (MARGIN + x * constants.CELL_SIZE,
                     MARGIN + y * constants.CELL_SIZE),
                    4
                )

        pygame.display.flip()

    def draw_player_info(self, turn, player1, player2):
        player1_pos = (10, 10)
        player2_pos = (10, BOARD_SIZE - 50)

        player1_rect = pygame.Rect(
            player1_pos[0], player1_pos[1], BOARD_SIZE, 50)
        player2_rect = pygame.Rect(
            player2_pos[0], player2_pos[1], BOARD_SIZE, 50)

        self.screen.fill(WOOD, player1_rect)
        self.screen.fill(WOOD, player2_rect)

        if turn == 1:
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
            f"Time: {player1.timer:.3f}s", True, BLACK), (BOARD_SIZE - 140, 10))
        timer2 = self.screen.blit(self.font_small.render(
            f"Time: {player2.timer:.3f}s", True, WHITE), (BOARD_SIZE - 140, BOARD_SIZE - 50))

        stones_counter1 = self.screen.blit(self.font_small.render(
            f"Taken: {player1.taken_stones}", True, BLACK), (BOARD_SIZE - 250, 10))
        stones_counter2 = self.screen.blit(self.font_small.render(
            f"Taken: {player2.taken_stones}", True, WHITE), (BOARD_SIZE - 250, BOARD_SIZE - 50))

        pygame.display.update([player1_rect, player2_rect, text_to_show1,
                              text_to_show2, timer1, timer2, stones_counter1, stones_counter2])

    def draw_stone(self, cell, color):
        pygame.draw.circle(
            self.screen, color,
            (MARGIN + cell[0] * constants.CELL_SIZE,
             MARGIN + cell[1] * constants.CELL_SIZE),
            constants.CELL_SIZE // 2 - 2
        )
        pygame.display.flip()

    def draw_all_stones(self, board):
        for y, value in enumerate(board):
            for x, cell_value in enumerate(value):
                cell = (x, y)
                if (cell_value != 0):
                    self.draw_stone(cell, BLACK if cell_value == 1 else WHITE)

    def draw_win_screen(self, player_name):
        self.screen.fill(WOOD)
        pos = (BOARD_SIZE/2 - 70, BOARD_SIZE/2)
        self.screen.blit(self.font.render("Game Over", True, RED), pos)
        msg_colour = BLACK if player_name == "Black" else WHITE
        self.screen.blit(self.font.render(f"{player_name} wins!", True, msg_colour),
                         (pos[0], pos[1] - 50))
        pygame.display.flip()
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
