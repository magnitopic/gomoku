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
    pass


def check_double_three(board, last_move) -> bool:
    pass


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
