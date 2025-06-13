/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min-max.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:22:51 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/09 20:21:54 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gomoku.h"
#include <unistd.h>

int minMax(int board_size, int **board, int maximizingPlayer, int depth, int alpha, int beta, t_move *bestMove)
{
	// If we've reached maximum depth, evaluate the board
	if (depth >= MAX_DEPTH)
	{
		bestMove->score = evaluateBoard(board_size, board);
		debug_update_node(-1, -1, bestMove->score, depth, 0); // Update stats
		return bestMove->score;
	}

	// Check if there are any valid moves
	int validMovesExist = 0;
	for (int i = 0; i < board_size && !validMovesExist; i++)
	{
		for (int j = 0; j < board_size && !validMovesExist; j++)
		{
			if (board[i][j] == EMPTY && isNearExistingStones(board_size, board, i, j))
			{
				validMovesExist = 1;
			}
		}
	}

	// If no valid moves, evaluate the current position
	if (!validMovesExist)
	{
		bestMove->score = evaluateBoard(board_size, board);
		debug_update_node(-1, -1, bestMove->score, depth, 0);
		return bestMove->score;
	}

	if (maximizingPlayer)
	{
		int maxEval = INT_MIN;
		int moveFound = 0;

		for (int i = 0; i < board_size; i++)
		{
			for (int j = 0; j < board_size; j++)
			{
				if (board[i][j] == EMPTY && isNearExistingStones(board_size, board, i, j))
				{
					board[i][j] = BLACK;
					t_move move = {i, j, 0};

					// Debug visualization
					debug_begin_move_eval(i, j, BLACK, depth);

					int eval = minMax(board_size, board, 0, depth + 1, alpha, beta, &move);
					board[i][j] = EMPTY;

					// Update debug heat map
					debug_update_node(i, j, eval, depth, 0);

					if (eval > maxEval || !moveFound)
					{
						maxEval = eval;
						bestMove->row = i;
						bestMove->col = j;
						bestMove->score = eval;
						moveFound = 1;
					}

					alpha = alpha > eval ? alpha : eval;
					if (beta <= alpha)
					{
						debug_update_node(i, j, eval, depth, 1); // Mark as pruned
						break;
					}
				}
			}
			if (beta <= alpha)
				break;
		}
		return maxEval;
	}
	else
	{
		int minEval = INT_MAX;
		int moveFound = 0;

		for (int i = 0; i < board_size; i++)
		{
			for (int j = 0; j < board_size; j++)
			{
				if (board[i][j] == EMPTY && isNearExistingStones(board_size, board, i, j))
				{
					board[i][j] = WHITE;
					t_move move = {i, j, 0};

					// Debug visualization
					debug_begin_move_eval(i, j, WHITE, depth);

					int eval = minMax(board_size, board, 1, depth + 1, alpha, beta, &move);
					board[i][j] = EMPTY;

					// Update debug heat map
					debug_update_node(i, j, eval, depth, 0);

					if (eval < minEval || !moveFound)
					{
						minEval = eval;
						bestMove->row = i;
						bestMove->col = j;
						bestMove->score = eval;
						moveFound = 1;
					}

					beta = beta < eval ? beta : eval;
					if (beta <= alpha)
					{
						debug_update_node(i, j, eval, depth, 1); // Mark as pruned
						break;
					}
				}
			}
			if (beta <= alpha)
				break;
		}

		return minEval;
	}
}

int isNearExistingStones(int board_size, int **board, int row, int col)
{
	// If there are no stones on the board, consider the center position as valid
	int found_stone = 0;
	for (int i = 0; i < board_size && !found_stone; i++)
	{
		for (int j = 0; j < board_size && !found_stone; j++)
		{
			if (board[i][j] != EMPTY)
			{
				found_stone = 1;
			}
		}
	}

	// If no stones on the board, prioritize center moves
	if (!found_stone)
	{
		int center = board_size / 2;
		int dist_to_center = abs(row - center) + abs(col - center);
		return dist_to_center <= 2; // Only consider positions near center
	}

	const int distance = 2; // Look 2 squares around existing stones

	for (int i = row - distance; i <= row + distance; i++)
	{
		for (int j = col - distance; j <= col + distance; j++)
		{
			if (i >= 0 && i < board_size && j >= 0 && j < board_size)
			{
				if (board[i][j] != EMPTY)
					return 1;
			}
		}
	}
	return 0;
}

int evaluateBoard(int board_size, int **board)
{
	// Simple evaluation function
	int score = 0;
	int black_score = 0;
	int white_score = 0;

	// Check for patterns in all 4 directions
	for (int i = 0; i < board_size; i++)
	{
		for (int j = 0; j < board_size; j++)
		{
			if (board[i][j] == EMPTY)
				continue;

			int player = board[i][j];

			// Check all 4 directions
			for (int dir = 0; dir < 4; dir++)
			{
				int count = 1;
				int open_ends = 0;

				// Check backward
				int r = i - dx[dir];
				int c = j - dy[dir];
				if (r >= 0 && r < board_size && c >= 0 && c < board_size)
				{
					if (board[r][c] == EMPTY)
						open_ends++;
				}

				// Check forward
				for (int k = 1; k < 5; k++)
				{
					r = i + k * dx[dir];
					c = j + k * dy[dir];

					if (r < 0 || r >= board_size || c < 0 || c >= board_size)
						break;

					if (board[r][c] == player)
						count++;
					else if (board[r][c] == EMPTY)
					{
						open_ends++;
						break;
					}
					else
						break;
				}

				// Assign score based on pattern
				int pattern_score = 0;

				if (count >= 5)
					pattern_score = FIVE_IN_A_ROW;
				else if (count == 4 && open_ends == 2)
					pattern_score = OPEN_FOUR;
				else if (count == 4 && open_ends == 1)
					pattern_score = FOUR;
				else if (count == 3 && open_ends == 2)
					pattern_score = OPEN_THREE;
				else if (count == 3 && open_ends == 1)
					pattern_score = THREE;
				else if (count == 2 && open_ends == 2)
					pattern_score = OPEN_TWO;
				else if (count == 2 && open_ends == 1)
					pattern_score = TWO;

				if (player == BLACK)
					black_score += pattern_score;
				else
					white_score += pattern_score;
			}
		}
	}

	score = black_score - white_score;
	return score;
}

/**
 * Function to find the best move using min-max with alpha-beta pruning
 */
t_move findBestMove(int board_size, int **board, int player)
{
	// Create a default move in case algorithm fails
	t_move bestMove = {-1, -1, player == BLACK ? INT_MIN : INT_MAX, 0};

	// First move on empty board - go to center
	int empty_board = 1;
	for (int i = 0; i < board_size && empty_board; i++)
	{
		for (int j = 0; j < board_size && empty_board; j++)
		{
			if (board[i][j] != EMPTY)
			{
				empty_board = 0;
			}
		}
	}

	if (empty_board)
	{
		int center = board_size / 2;
		bestMove.row = center;
		bestMove.col = center;
		bestMove.score = 0;
		return bestMove;
	}

	// Initialize debug visualizer
	init_debug_visualizer(board_size);

	// Start search from depth 0
	minMax(board_size, board, player == BLACK, 0, INT_MIN, INT_MAX, &bestMove);

	// If no move was found, find any valid move
	if (bestMove.row == -1 || bestMove.col == -1)
	{
		printf("Warning: No move found by minMax, looking for any valid move\n");
		for (int i = 0; i < board_size; i++)
		{
			for (int j = 0; j < board_size; j++)
			{
				if (board[i][j] == EMPTY && isNearExistingStones(board_size, board, i, j))
				{
					bestMove.row = i;
					bestMove.col = j;
					bestMove.score = 0;
					break;
				}
			}
			if (bestMove.row != -1)
				break;
		}
	}

	// Print debug visualization
	print_debug_visualization(board_size, board, bestMove);
	// Free debug resources
	free_debug_visualizer(board_size);

	return bestMove;
}

t_move ai_algorithm(int board_size, int **board, int color)
{
	t_move best_move;
	time_t start, end;

	start = time(NULL);
	best_move = findBestMove(board_size, board, color);
	end = time(NULL);
	best_move.time_taken = (double)(end - start);

	printf("AI chose move: [%d,%d] with score %d\n",
		   best_move.row, best_move.col, best_move.score);

	return best_move;
}
