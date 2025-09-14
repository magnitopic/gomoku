/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_config.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:08:24 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/14 16:36:44 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AI_config_HPP_
#define AI_config_HPP_

class Player;

#include <iostream>
#include <climits>
#include <map>
#include "../classes/Board.hpp"
#include "../classes/Player.hpp"

enum e_patterns
{
	e_FIVE_IN_A_ROW,
	e_OPEN_FOUR,
	e_FOUR,
	e_OPEN_THREE,
	e_THREE,
	e_OPEN_TWO,
	e_TWO
};

typedef struct s_move
{
	int row;
	int col;
	int score;
} t_move;

typedef struct s_pattern
{
	e_patterns type;
	int position;
	int score;
} t_pattern;

typedef struct s_pattern_match
{
	std::vector<int> pattern;
	e_patterns type;
	int score;
} t_pattern_match;

/* AI configuration parameters - AGGRESSIVE SETTINGS */

#define MAX_DEPTH 2		 // Deeper search for aggressive play
#define MAX_BRANCHING 10 // More branches to find forcing moves

// Pattern scores for the heuristic
#define FIVE_IN_A_ROW 1000000
#define OPEN_FOUR 100000
#define FOUR 10000
#define OPEN_THREE 5000
#define THREE 1000
#define OPEN_TWO 200
#define TWO 50

// Algorithm functions
int minMax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer, int player, t_move *bestMove);
int staticBoardEvaluation(Board *board, int player);
int evaluateLine(const std::vector<int> &line, int player);
std::vector<std::pair<int, int>> getValidMoves(Board *board, int color);

#endif