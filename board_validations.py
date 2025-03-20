
DIRECTIONS = [
    (1, 0),   # Horizontal
    (0, 1),   # Vertical
    (1, 1),   # Diagonal (up-right)
    (1, -1)   # Diagonal (up-left)
]


def check_n_in_a_row(board, last_move, color, direction, n):
    count = 1
    x, y = last_move
    dx, dy = direction

    for i in range(1, n):
        next_pos = (x + dx * i, y + dy * i)
        if board[next_pos[1]][next_pos[0]] == color:
            count += 1
        else:
            break

    for i in range(1, n):
        next_pos = (x - dx * i, y - dy * i)
        if board[next_pos[1]][next_pos[0]] == color:
            count += 1
        else:
            break

    return count >= n


def check_double_three(board, last_move):
    pass


def check_win(board, last_move, color):
    for direction in DIRECTIONS:
        if check_n_in_a_row(board, last_move, color, direction, 5):
            return True
    return False


def validate_board(board):
    pass
