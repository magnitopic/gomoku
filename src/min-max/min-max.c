/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min-max.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:22:51 by alaparic          #+#    #+#             */
/*   Updated: 2025/04/03 11:42:28 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gomoku.h"

int count = 1;

int minMax(int board_size, int **board, int isMaximizingPlayer, int depth, int alpha, int beta, t_move *bestMove)
{
	printf("minMax, %d\n", count++);
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
