from constants import COLS, ROWS, DIRECTIONS


def is_move_in_bounds(cell) -> bool:
    col, row = cell
    return 0 <= col < COLS and 0 <= row < ROWS


def check_n_in_a_row(board, last_move, color, direction, n):
    count = 1
    x, y = last_move
    dx, dy = direction

    for i in range(1, n):
        next_pos = (x + dx * i, y + dy * i)

        if not is_move_in_bounds(next_pos):
            break

        if board[next_pos[1]][next_pos[0]] == color:
            count += 1
        else:
            break

    for i in range(1, n):
        next_pos = (x - dx * i, y - dy * i)

        if not is_move_in_bounds(next_pos):
            break

        if board[next_pos[1]][next_pos[0]] == color:
            count += 1
        else:
            break

    return count >= n


def check_three(board, last_move, color) -> bool:
    x, y = last_move
    free_threes = []

    # check all directions
    for direction in DIRECTIONS:
        dx, dy = direction
        stones = 1

        spaces_after = 0
        i = 1
        while True:
            nx, ny = x + dx * i, y + dy * i
            if not is_move_in_bounds((nx, ny)):
                break

            if board[ny][nx] == color:
                stones += 1
                i += 1
            elif board[ny][nx] == 0:
                spaces_after += 1
                break
            else:
                break

        spaces_before = 0
        i = 1
        while True:
            nx, ny = x - dx * i, y - dy * i
            if not is_move_in_bounds((nx, ny)):
                break

            if board[ny][nx] == color:
                stones += 1
                i += 1
            elif board[ny][nx] == 0:
                spaces_before += 1
                break
            else:
                break

        if stones == 3 and spaces_before > 0 and spaces_after > 0:
            free_threes.append(direction)

    return free_threes


def check_double_three(board, last_move, color) -> bool:
    x, y = last_move

    if board[y][x] != 0:
        return False

    board[y][x] = color

    free_threes = check_three(board, last_move, color)

    board[y][x] = 0

    return len(free_threes) >= 2


def check_win(board, last_move, color) -> bool:
    for direction in DIRECTIONS:
        if check_n_in_a_row(board, last_move, color, direction, 5):
            return True
    return False


def check_board_full(board) -> bool:
    for row in board:
        if 0 in row:
            return False
    return True


# algorithm functions

def get_valid_neighbour_move(board):
    """ Get all valid moves. For efficiency, only get the moves around the existing stones. """
