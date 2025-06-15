/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_config.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:08:24 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/15 13:14:25 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AI_config_HPP_
#define AI_config_HPP_

#include <iostream>
#include <climits>
#include "../classes/Board.hpp"

typedef struct s_move
{
	int row;
	int col;
	int score;
} t_move;

/* AI configuration parameters */

#define MAX_DEPTH 3 // Maximum depth for the Min-Max algorithm
#define MAX_BRANCHING 6 // Maximum number of branches for each node

// Pattern scores for the heuristic
#define FIVE_IN_A_ROW 100000
#define OPEN_FOUR 10000
#define FOUR 1000
#define OPEN_THREE 500
#define THREE 50
#define OPEN_TWO 10
#define TWO 5

// Algorithm functions
int minMax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer, int player, t_move *bestMove);

#endif