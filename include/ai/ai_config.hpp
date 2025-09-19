/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_config.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:08:24 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/19 18:41:34 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AI_config_HPP_
#define AI_config_HPP_

class Player;

#include <iostream>
#include <climits>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include "../classes/Board.hpp"
#include "../classes/Player.hpp"
#include "../structs.hpp"

/* AI configuration parameters */

#define MAX_DEPTH 5	   // Deeper search for aggressive play
#define MAX_BRANCHES 2 // More branches to find forcing moves

// Pattern scores for the heuristic
#define FIVE_IN_A_ROW 1000000
#define STONE_CAPTURE 500000
#define OPEN_FOUR 100000
#define FOUR 10000
#define OPEN_THREE 5000
#define THREE 1000
#define OPEN_TWO 200
#define TWO 50

// Algorithm functions
int minMax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer, int player, s_move *bestMove);
int staticBoardEvaluation(Board *board, int player);
int evaluateLine(const std::vector<int> &line, int player);
std::vector<std::pair<int, int>> getValidMoves(Board *board, int color);

#endif