import pygame


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


def main():
    running = True
    clock = pygame.time.Clock()

    while running:
        # Event handling
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    running = False

        # Drawing
        screen.fill(BLACK)
        draw_board()

        clock.tick(10)

    pygame.quit()


if __name__ == "__main__":
    main()
