import pygame
from board_validations import validate_board, check_double_three, check_win

pygame.init()

# Constants
BOARD_SIZE = 920
COLS, ROWS = 18, 18
MARGIN = 35
BORDER_MARGIN = 30
CELL_SIZE = (BOARD_SIZE - 2 * MARGIN) // COLS

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
BORDER_COLOR = (150, 150, 150)
WOOD = (222, 184, 135)

# Global variables
turn = 1
board = [[0 for _ in range(COLS)] for _ in range(ROWS)]


# Create the screen
screen = pygame.display.set_mode((BOARD_SIZE, BOARD_SIZE))
pygame.display.set_caption("gomoku")


def draw_board():
    border_rect = pygame.Rect(
        MARGIN - BORDER_MARGIN,
        MARGIN - BORDER_MARGIN,
        COLS * CELL_SIZE + 2 * BORDER_MARGIN,
        ROWS * CELL_SIZE + 2 * BORDER_MARGIN
    )
    pygame.draw.rect(screen, BORDER_COLOR, border_rect, 2)

    for i in range(ROWS + 1):
        pygame.draw.line(
            screen, WHITE,
            (MARGIN, MARGIN + i * CELL_SIZE),
            (MARGIN + COLS * CELL_SIZE, MARGIN + i * CELL_SIZE),
            2
        )

    for i in range(COLS + 1):
        pygame.draw.line(
            screen, WHITE,
            (MARGIN + i * CELL_SIZE, MARGIN),
            (MARGIN + i * CELL_SIZE, MARGIN + ROWS * CELL_SIZE),
            2
        )

    pygame.display.flip()


def draw_stone(cell, color):
    pygame.draw.circle(
        screen, color,
        (MARGIN + cell[0] * CELL_SIZE, MARGIN + cell[1] * CELL_SIZE),
        CELL_SIZE // 2 - 2
    )
    pygame.display.flip()


def main():
    global turn
    global win
    running = True
    clock = pygame.time.Clock()

    screen.fill(WOOD)
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
                col = mouse_pos[0] // CELL_SIZE
                row = mouse_pos[1] // CELL_SIZE
                cell = (col, row)
                if not (0 <= col <= COLS and 0 <= row <= ROWS):
                    continue
                if board[row][col] != 0:
                    continue
                board[row][col] = turn
                if check_win(board, cell, turn):
                    print("Win")
                    running = False
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
