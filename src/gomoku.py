from constants import *
print(T_GRAY, end='')
import pygame
from Game import Game
from board_validations import is_move_in_bounds

pygame.init()


def main():
    running: bool = True
    clock = pygame.time.Clock()

    game = Game()

    game.draw_board()

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
                if not is_move_in_bounds((col, row)):
                    continue
                cell = (col, row)

                running = game.handle_turn(cell)

        clock.tick(10)

    pygame.quit()


if __name__ == "__main__":
    main()
    print(T_RESET)
