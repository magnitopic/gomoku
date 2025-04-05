/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gomoku.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:17:34 by alaparic          #+#    #+#             */
/*   Updated: 2025/04/04 11:52:39 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GOMOKU_H
# define GOMOKU_H

# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
#include <time.h>

typedef struct s_move
{
	int	row;
	int	col;
	int	score;
	double time_taken;
}	t_move;

# define EMPTY	0
# define BLACK	1
# define WHITE	-1
# define MAX_DEPTH 50

const int dx[4] = {1, 0, 1, 1};
const int dy[4] = {0, 1, 1, -1};

// Pattern scores for the heuristic
# define FIVE_IN_A_ROW	100000
# define OPEN_FOUR		10000
# define FOUR			1000
# define OPEN_THREE		500
# define THREE			50
# define OPEN_TWO		10
# define TWO			5

int minMax(int board_size, int **board, int isMaximizingPlayer, int depth, int alpha, int beta, t_move *bestMove);
int isNearExistingStones(int board_size, int **board, int row, int col);
int evaluateBoard(int board_size, int **board);
t_move ai_algorithm(int board_size, int **board, int color);

#endif
