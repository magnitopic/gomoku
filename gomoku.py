import pygame
from time import sleep
from board_validations import validate_board, check_double_three, check_win

pygame.init()

# Constants
BOARD_SIZE = 920
GRID_SIZE = 18  # This is the number of spaces between intersections
COLS, ROWS = 19, 19  # This is the number of intersections
MARGIN = 50
BORDER_MARGIN = 30
CELL_SIZE = (BOARD_SIZE - 2 * MARGIN) // GRID_SIZE

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
BORDER_COLOR = (150, 150, 150)
WOOD = (222, 184, 135)
RED = (150, 0, 0)

# Global variables
turn = 1
board = [[0 for _ in range(COLS)] for _ in range(ROWS)]


# Create the screen
screen = pygame.display.set_mode((BOARD_SIZE, BOARD_SIZE))
pygame.display.set_caption("gomoku")
font = pygame.font.SysFont("Arial", 30)


def draw_board():
    screen.fill(WOOD)

    # Draw border around the board
    border_rect = pygame.Rect(
        MARGIN - BORDER_MARGIN,
        MARGIN - BORDER_MARGIN,
        GRID_SIZE * CELL_SIZE + 2 * BORDER_MARGIN,
        GRID_SIZE * CELL_SIZE + 2 * BORDER_MARGIN
    )
    pygame.draw.rect(screen, BORDER_COLOR, border_rect, 2)

    # Draw the grid lines (19 vertical and 19 horizontal lines for intersections)
    for i in range(COLS):
        pygame.draw.line(
            screen, BLACK,
            (MARGIN + i * CELL_SIZE, MARGIN),
            (MARGIN + i * CELL_SIZE, MARGIN + GRID_SIZE * CELL_SIZE),
            2
        )

    for i in range(ROWS):
        pygame.draw.line(
            screen, BLACK,
            (MARGIN, MARGIN + i * CELL_SIZE),
            (MARGIN + GRID_SIZE * CELL_SIZE, MARGIN + i * CELL_SIZE),
            2
        )

    # Draw star points
    star_points = [3, 9, 15]
    for x in star_points:
        for y in star_points:
            pygame.draw.circle(
                screen, BLACK,
                (MARGIN + x * CELL_SIZE, MARGIN + y * CELL_SIZE),
                4
            )

    pygame.display.flip()


def draw_stone(cell, color):
    pygame.draw.circle(
        screen, color,
        (MARGIN + cell[0] * CELL_SIZE, MARGIN + cell[1] * CELL_SIZE),
        CELL_SIZE // 2 - 2
    )
    pygame.display.flip()


def win_screen():
    screen.fill(WOOD)
    pos = (BOARD_SIZE/2 - 70, BOARD_SIZE/2)
    screen.blit(font.render("Game Over", True, RED), pos)
    if turn == 1:
        screen.blit(font.render("Black wins!", True, BLACK),
                    (pos[0], pos[1] - 50))
    else:
        screen.blit(font.render("White wins!", True, WHITE),
                    (pos[0], pos[1] - 50))
    pygame.display.flip()
    sleep(2)


def main():
    global turn
    running = True
    clock = pygame.time.Clock()

    draw_board()

    while running:
        # Event handling
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    running = False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                mouse_pos = pygame.mouse.get_pos()

                # Calculate the closest intersection
                col = round((mouse_pos[0] - MARGIN) / CELL_SIZE)
                row = round((mouse_pos[1] - MARGIN) / CELL_SIZE)

                # Ensure the position is within bounds
                if not (0 <= col < COLS and 0 <= row < ROWS):
                    continue

                cell = (col, row)
                print(cell)

                # Check if the cell is already occupied
                if board[row][col] != 0:
                    continue

                board[row][col] = turn

                # Check if it's a winning move
                if check_win(board, cell, turn):
                    win_screen()
                    running = False
                    break

                # Draw the stone
                if turn == 1:
                    draw_stone(cell, BLACK)
                    turn = -1
                else:
                    draw_stone(cell, WHITE)
                    turn = 1

        clock.tick(10)

    pygame.quit()


if __name__ == "__main__":
    main()
