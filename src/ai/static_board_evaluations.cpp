/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_board_evaluations.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:49:45 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/14 17:14:31 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ai/ai_config.hpp"

int detectPatterns(Board *board, int color)
{
	int boardScore = 0;

	for (std::pair<int, int> direction : DIRECTIONS)
	{
		// get the number of lines to check based on direction
		int iterations = direction == DIRECTIONS[0] || direction == DIRECTIONS[1] ? board->getSize() - 1 : (board->getSize() * 2) - 2;

		for (int i = 0; i <= iterations; i++)
		{
			std::vector<int> line = board->getLine(direction, i);
			if (line.empty())
				continue;
			int linePatterns = evaluateLine(line, color);
			boardScore += linePatterns;
		}
	}

	return boardScore;
}

int staticBoardEvaluation(Board *board, int player)
{
	// Evaluate both colors
	int blackScore = detectPatterns(board, BLACK_STONE);
	int whiteScore = detectPatterns(board, WHITE_STONE);

	// Return score from the perspective of the current player
	if (player == BLACK_STONE)
		return blackScore - whiteScore;
	else
		return whiteScore - blackScore;
}
