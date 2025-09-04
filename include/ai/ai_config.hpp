/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_config.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:08:24 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/25 12:16:59 by alaparic         ###   ########.fr       */
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

typedef struct s_move
{
	int row;
	int col;
	int score;
} t_move;

typedef struct s_pattern
{
	int length;
	int openEnds; // 0, 1, or 2 open ends
	bool hasGap;
	int player;
	int threatLevel;								 // 0-5, 5 being immediate win/block needed
	std::vector<std::pair<int, int>> blockPositions; // Where to block this threat
} t_pattern;

/* AI configuration parameters - AGGRESSIVE SETTINGS */

#define MAX_DEPTH 4		 // Deeper search for aggressive play
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
int staticBoardEvaluation(Board *board, int player, int depth);
std::vector<t_pattern> detectPatterns(Board *board, int color);
std::vector<std::pair<int, int>> findCriticalMoves(Board *board, int opponent);

#endif