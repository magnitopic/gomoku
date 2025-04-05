/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min-max.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:22:51 by alaparic          #+#    #+#             */
/*   Updated: 2025/04/05 22:35:44 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gomoku.h"

int count = 1;

int minMax(int board_size, int **board, int maximizingPlayer, int depth, int alpha, int beta, t_move *bestMove)
{
	printf("minMax, %d\n", count++);

	if (depth == 0)
	{
		bestMove->score = evaluateBoard(board_size, board);
		return bestMove->score;
	}

	if (maximizingPlayer)
	{
		int maxEval = INT_MIN;

		for (int i = 0; i < board_size; i++)
		{
			for (int j = 0; j < board_size; j++)
			{
				if (board[i][j] == EMPTY && isNearExistingStones(board_size, board, i, j))
				{
					board[i][j] = BLACK;
					t_move move = {i, j, 0};
					int eval = minMax(board_size, board, 0, depth - 1, alpha, beta, &move);
					board[i][j] = EMPTY;

					if (eval > maxEval)
					{
						maxEval = eval;
						bestMove->row = i;
						bestMove->col = j;
						bestMove->score = eval;
					}
				}
			}
		}
		return maxEval;
	}
	else
	{
		int minEval = INT_MAX;

		for (int i = 0; i < board_size; i++)
		{
			for (int j = 0; j < board_size; j++)
			{
				if (board[i][j] == EMPTY && isNearExistingStones(board_size, board, i, j))
				{
					board[i][j] = WHITE;
					t_move move = {i, j, 0};
					int eval = minMax(board_size, board, 1, depth - 1, alpha, beta, &move);
					board[i][j] = EMPTY;

					if (eval < minEval)
					{
						minEval = eval;
						bestMove->row = i;
						bestMove->col = j;
						bestMove->score = eval;
					}
				}
			}
		}

		return minEval;
	}
}

int isNearExistingStones(int board_size, int **board, int row, int col)
{
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
	return 0;
}

/**
 * Function to find the best move using min-max with alpha-beta pruning
 */
t_move findBestMove(int board_size, int **board, int player)
{
	t_move bestMove = {-1, -1, player == BLACK ? INT_MIN : INT_MAX, 0};

	// Call minMax with alpha-beta pruning
	minMax(board_size, board, MAX_DEPTH, player == BLACK, INT_MIN, INT_MAX, &bestMove);

	return bestMove;
}

t_move ai_algorithm(int board_size, int **board, int color)
{
	t_move best_move;
	clock_t start, end;
	count = 1;

	start = clock();

	best_move = findBestMove(board_size, board, color);

	end = clock();
	best_move.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

	return best_move;
}
