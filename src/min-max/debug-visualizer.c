/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug-visualizer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:00:00 by alaparic          #+#    #+#             */
/*   Updated: 2025/04/16 08:14:41 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gomoku.h"

// Colors for terminal output
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_BOLD "\033[1m"

// Debug visualization structure
typedef struct s_debug_info
{
	int depth;		   // Current depth in the search tree
	int node_count;	   // Number of nodes evaluated
	int prune_count;   // Number of branches pruned
	int **heat_map;	   // Heat map of evaluation scores
	int **visit_count; // Number of times each position was visited
	int max_score;	   // Maximum score seen
	int min_score;	   // Minimum score seen
} t_debug_info;

// Global debug info structure
t_debug_info g_debug;

/**
 * Initialize the debug visualization system
 */
void init_debug_visualizer(int board_size)
{
	g_debug.depth = 0;
	g_debug.node_count = 0;
	g_debug.prune_count = 0;
	g_debug.max_score = INT_MIN;
	g_debug.min_score = INT_MAX;

	// Allocate heat map
	g_debug.heat_map = malloc(board_size * sizeof(int *));
	g_debug.visit_count = malloc(board_size * sizeof(int *));
	for (int i = 0; i < board_size; i++)
	{
		g_debug.heat_map[i] = malloc(board_size * sizeof(int));
		g_debug.visit_count[i] = malloc(board_size * sizeof(int));
		for (int j = 0; j < board_size; j++)
		{
			g_debug.heat_map[i][j] = 0;
			g_debug.visit_count[i][j] = 0;
		}
	}
}

/**
 * Free debug visualization resources
 */
void free_debug_visualizer(int board_size)
{
	for (int i = 0; i < board_size; i++)
	{
		free(g_debug.heat_map[i]);
		free(g_debug.visit_count[i]);
	}
	free(g_debug.heat_map);
	free(g_debug.visit_count);
}

/**
 * Update the debug visualization data
 */
void debug_update_node(int row, int col, int score, int depth, int pruned)
{
	g_debug.node_count++;
	g_debug.depth = depth > g_debug.depth ? depth : g_debug.depth;

	if (pruned)
		g_debug.prune_count++;

	if (row >= 0 && col >= 0)
	{
		g_debug.heat_map[row][col] = score;
		g_debug.visit_count[row][col]++;

		// Update min/max scores
		if (score > g_debug.max_score)
			g_debug.max_score = score;
		if (score < g_debug.min_score)
			g_debug.min_score = score;
	}
}

/**
 * Calculate a color for a heat map value
 */
const char *get_heat_color(int score)
{
	if (score == 0)
		return COLOR_WHITE;

	// Scale from min to max with colors
	if (score > 0)
	{
		if (score > 5000)
			return COLOR_RED;
		else if (score > 1000)
			return COLOR_YELLOW;
		else if (score > 100)
			return COLOR_GREEN;
		else
			return COLOR_CYAN;
	}
	else
	{
		if (score < -5000)
			return COLOR_BLUE;
		else if (score < -1000)
			return COLOR_MAGENTA;
		else if (score < -100)
			return COLOR_CYAN;
		else
			return COLOR_WHITE;
	}
}

/**
 * Print the gomoku board with heat map and visit information
 */
void print_debug_board(int board_size, int **board, int mode)
{
	printf("\n" COLOR_BOLD "   ");
	for (int j = 0; j < board_size; j++)
		printf("\t%2d", j);
	printf("\n");

	for (int i = 0; i < board_size; i++)
	{
		printf(COLOR_BOLD "%2d " COLOR_RESET, i);
		for (int j = 0; j < board_size; j++)
		{
			printf("\t");
			if (board[j][i] == BLACK)
				printf(COLOR_BOLD "●" COLOR_RESET);
			else if (board[j][i] == -1) // Use the numeric value instead of WHITE macro
				printf(COLOR_BOLD "○" COLOR_RESET);
			else
			{
				if (mode == 1) // Heat map mode
				{
					const char *color = get_heat_color(g_debug.heat_map[j][i]);
					if (g_debug.visit_count[j][i] > 0)
						printf("%s%2d" COLOR_RESET, color, g_debug.heat_map[j][i] / 100);
					else
						printf("·");
				}
				else if (mode == 2) // Visit count mode
				{
					if (g_debug.visit_count[j][i] > 0)
						printf(COLOR_YELLOW "%2d " COLOR_RESET, g_debug.visit_count[j][i]);
					else
						printf("·");
				}
				else // Normal mode
					printf("·");
			}
		}
		printf("\n");
		printf("\n");
	}
}

/**
 * Print statistics about the algorithm's execution
 */
void print_debug_stats()
{
	printf("\n" COLOR_BOLD "=== Algorithm Statistics ===" COLOR_RESET "\n");
	printf("Nodes evaluated: %d\n", g_debug.node_count);
	printf("Maximum depth reached: %d\n", g_debug.depth);
	printf("Branches pruned: %d (%.2f%%)\n", g_debug.prune_count,
		   (float)g_debug.prune_count / g_debug.node_count * 100);
	printf("Score range: %d to %d\n", g_debug.min_score, g_debug.max_score);
	printf("\n");
}

/**
 * Captures the beginning of a move evaluation for debugging
 */
void debug_begin_move_eval(int row, int col, int player, int depth)
{
	/* if (depth <= 2) // Only print detailed logs for top-level moves
	{
		printf(COLOR_BOLD "%sEvaluating %s move at [%d,%d] (depth %d)%s\n",
			   player == BLACK ? COLOR_RED : COLOR_BLUE,
			   player == BLACK ? "BLACK" : "WHITE",
			   row, col, depth, COLOR_RESET);
	} */
}

/**
 * Print the main debug visualization after algorithm completion
 */
void print_debug_visualization(int board_size, int **board, t_move best_move)
{
	printf("\n" COLOR_BOLD "=== AI Decision Analysis ===" COLOR_RESET "\n");
	printf("Best move: [%d,%d] with score %d\n", best_move.row, best_move.col, best_move.score);
	printf("Time taken: %.2f seconds\n", best_move.time_taken);

	// Print heat map visualization
	printf("\n" COLOR_BOLD "Heat Map" COLOR_RESET " (values divided by 100 for readability)\n");
	print_debug_board(board_size, board, 1);

	// Print visit count visualization
	printf("\n" COLOR_BOLD "Visit Count" COLOR_RESET "\n");
	print_debug_board(board_size, board, 2);

	// Print statistics
	print_debug_stats();
}