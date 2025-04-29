import constants
from GameLogic import GameLogic
from game_config import initial_game_config
from board_validations import is_move_in_bounds
import pygame
from constants import *
print(T_GRAY, end='')


pygame.init()


def main():
    # Game configuration
    config = initial_game_config()

    constants.COLS = constants.ROWS = int(config["board_size"])
    constants.GRID_SIZE = int(config["board_size"]) - 1
    constants.CELL_SIZE = (constants.BOARD_SIZE - 2 *
                           constants.MARGIN) // constants.GRID_SIZE

    running: bool = True
    clock = pygame.time.Clock()

    # Initialize the game logic
    gameLogic = GameLogic(config)

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
                col = round((mouse_pos[0] - MARGIN) / constants.CELL_SIZE)
                row = round((mouse_pos[1] - MARGIN) / constants.CELL_SIZE)

                # Ensure the position is within bounds
                if not is_move_in_bounds((col, row)):
                    continue
                cell = (col, row)

                running = gameLogic.handle_turn(cell)

                # If playing against AI, make AI move
                if gameLogic.player2.ai:
                    ai_move = gameLogic.current_player.new_ai_move(
                        gameLogic.board)
                    ai_cell = (ai_move[1], ai_move[0])
                    running = gameLogic.handle_turn(ai_cell)

        clock.tick(10)

    pygame.quit()
    """ if game.save_history:
        game.game_history.create_history_file() """


if __name__ == "__main__":
    main()
    print(T_RESET)
