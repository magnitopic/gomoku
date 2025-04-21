from constants import *
print(T_GRAY, end='')
import pygame
from Game import Game
from board_validations import is_move_in_bounds
from game_config import initial_game_config

pygame.init()


def main():
    import constants
    config = initial_game_config()
    print(f"{T_WHITE}Config: {config}{T_GRAY}")

    running: bool = True
    clock = pygame.time.Clock()

    constants.COLS = int(config["board_size"])
    constants.ROWS = int(config["board_size"])
    constants.GRID_SIZE = int(config["board_size"]) - 1
    constants.CELL_SIZE = (constants.BOARD_SIZE - 2 * constants.MARGIN) // constants.GRID_SIZE

    game = Game(config)

    game.draw_board()
    game.draw_player_info()

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
