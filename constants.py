# Global program constants
BOARD_SIZE = 920
COLS, ROWS = 19, 19   # This is the number of intersections
GRID_SIZE = COLS - 1  # This is the number of spaces between intersections
MARGIN = 50
BORDER_MARGIN = 30
CELL_SIZE = (BOARD_SIZE - 2 * MARGIN) // GRID_SIZE

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
BORDER_COLOR = (150, 150, 150)
WOOD = (222, 184, 135)
RED = (150, 0, 0)


# Directions
DIRECTIONS = [
    (1, 0),   # Horizontal
    (0, 1),   # Vertical
    (1, 1),   # Diagonal (up-right)
    (1, -1)   # Diagonal (up-left)
]
