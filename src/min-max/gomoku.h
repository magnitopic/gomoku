/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gomoku.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:17:34 by alaparic          #+#    #+#             */
/*   Updated: 2025/04/13 00:34:41 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GOMOKU_H
#define GOMOKU_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>

typedef struct s_move
{
	int row;
	int col;
	int score;
	double time_taken;
} t_move;

#define EMPTY 0
#define BLACK 1
#define WHITE -1
#define MAX_DEPTH 3 // Reduced for visualization testing

// Direction vectors for pattern detection
extern const int dx[4];
extern const int dy[4];

// Pattern scores for the heuristic
#define FIVE_IN_A_ROW 100000
#define OPEN_FOUR 10000
#define FOUR 1000
#define OPEN_THREE 500
#define THREE 50
#define OPEN_TWO 10
#define TWO 5

// Debug visualization modes
#define DEBUG_NORMAL 0
#define DEBUG_HEATMAP 1
#define DEBUG_VISITS 2
#define DEBUG_PRUNING 3

// Core algorithm functions
int minMax(int board_size, int **board, int isMaximizingPlayer, int depth, int alpha, int beta, t_move *bestMove);
int isNearExistingStones(int board_size, int **board, int row, int col);
int evaluateBoard(int board_size, int **board);
t_move ai_algorithm(int board_size, int **board, int color);

// Debug visualization functions
void init_debug_visualizer(int board_size);
void free_debug_visualizer(int board_size);
void debug_update_node(int row, int col, int score, int depth, int pruned);
void debug_begin_move_eval(int row, int col, int player, int depth);
void print_debug_board(int board_size, int **board, int mode);
void print_debug_stats(void);
void print_debug_visualization(int board_size, int **board, t_move best_move);

#endif