from constants import *
import constants


def is_move_in_bounds(cell) -> bool:
    col, row = cell
    return 0 <= col < constants.COLS and 0 <= row < constants.ROWS


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
        separate_threes = False

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
                i += 1
                nx, ny = x + dx * i, y + dy * i
                if not is_move_in_bounds((nx, ny)):
                    break
                # if after the space there is a stone of the same color, continue counting
                if board[ny][nx] == color:
                    stones += 1
                    i += 1
                    separate_threes = True
                else:
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
                i += 1
                nx, ny = x - dx * i, y - dy * i
                if not is_move_in_bounds((nx, ny)):
                    break
                if board[ny][nx] == color:
                    stones += 1
                    i += 1
                    separate_threes = True
                else:
                    break
            else:
                break

        if stones == 3 and spaces_before > 0 and spaces_after > 0 and not separate_threes:
            free_threes.append(direction)
            print(T_YELLOW + "Found a free three!"+T_GRAY)
        elif stones == 3 and ((spaces_before == 2 and spaces_after == 1) or (spaces_before == 1 and spaces_after == 2)) and separate_threes:
            free_threes.append(direction)
            print(T_YELLOW + "Found a free three with a space!"+T_GRAY)

    return free_threes


def check_same_direction_double_three(board, last_move, color):
    x, y = last_move

    pattern = [0, color, color, 0]

    # check all directions
    for direction in DIRECTIONS:
        sides = 0
        dx, dy = direction

        for i in range(1, 4):
            next_pos = (x + dx * i, y + dy * i)

            if not is_move_in_bounds(next_pos):
                break

            if board[next_pos[1]][next_pos[0]] != pattern[i - 1]:
                break
        else:
            sides += 1

        for i in range(1, 4):
            next_pos = (x - dx * i, y - dy * i)

            if not is_move_in_bounds(next_pos):
                break

            if board[next_pos[1]][next_pos[0]] != pattern[i - 1]:
                break
        else:
            sides += 1

        if sides == 2:
            return True

    return False


def check_double_three(board, last_move, color) -> bool:
    x, y = last_move

    if check_same_direction_double_three(board, last_move, color):
        return True

    board[y][x] = color

    free_threes = check_three(board, last_move, color)

    board[y][x] = 0

    return len(free_threes) >= 2


def check_capture(board, last_move, color):
    x, y = last_move

    stones_to_delete = []

    for direction in DIRECTIONS:
        dx, dy = direction

        for i in range(1, 4):
            next_pos = (x + dx * i, y + dy * i)

            if not is_move_in_bounds(next_pos):
                break

            if board[next_pos[1]][next_pos[0]] == -color and i == 1:
                stones_to_delete.append(next_pos)
            elif board[next_pos[1]][next_pos[0]] == -color and i == 2:
                stones_to_delete.append(next_pos)
            elif board[next_pos[1]][next_pos[0]] == color and i == 3:
                return stones_to_delete

            else:
                break

        stones_to_delete = []

        for i in range(1, 4):
            next_pos = (x - dx * i, y - dy * i)

            if not is_move_in_bounds(next_pos):
                break

            if board[next_pos[1]][next_pos[0]] == -color and i == 1:
                stones_to_delete.append(next_pos)
            elif board[next_pos[1]][next_pos[0]] == -color and i == 2:
                stones_to_delete.append(next_pos)
            elif board[next_pos[1]][next_pos[0]] == color and i == 3:
                return stones_to_delete
            else:
                break

    return False


def check_move_into_capture(board, last_move, color) -> bool:
    x, y = last_move

    board[y][x] = color

    for direction in DIRECTIONS:
        dx, dy = direction

        next_stone = (x + dx, y + dy)
        if check_capture(board, next_stone, -color) != False:
            board[y][x] = 0
            return True

        print("_"*20)

        next_stone = (x - dx, y - dy)
        if check_capture(board, next_stone, -color) != False:
            board[y][x] = 0
            return True

    board[y][x] = 0

    return False


""" End conditions """


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


""" Algorithm functions """


def get_valid_neighbour_move(board):
    """ Get all valid moves. For efficiency, only get the moves around the existing stones. """
    pass
