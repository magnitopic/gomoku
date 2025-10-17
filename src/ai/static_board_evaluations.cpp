/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_board_evaluations.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:49:45 by adiaz-uf          #+#    #+#             */
/*   Updated: 2025/09/28 18:05:12 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ai/AI.hpp"

int AI::detectPatterns(Board *board, int color)
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
			int linePatterns = this->evaluateLine(line, color);
			boardScore += linePatterns;
		}
	}

	return boardScore;
}

int AI::staticBoardEvaluation(Board *board, int player)
{
	// Evaluate both colors
	int blackScore = this->detectPatterns(board, BLACK_STONE);
	int whiteScore = this->detectPatterns(board, WHITE_STONE);

	// Return score from the perspective of the current player
	if (player == BLACK_STONE)
		return blackScore - whiteScore;
	else
		return whiteScore - blackScore;
}

int AI::getBoardValue(Board *board, int player)
{
	uint64_t hash = board->getCurrentBoardHash();

	// look for board hash in cache
	auto it = this->cache.find(hash);
	if (it != this->cache.end())
	{
		/* std::cout << T_BLUE << "Found a pattern! Total patterns: " << this->cache.size() << std::endl; */
		return it->second;
	}

	// if value for board was not found make board evaluations and store in cache
	int evaluation = this->staticBoardEvaluation(board, player);
	if (this->cache.size() >= this->MAX_ENTRIES)
		this->cache.clear();
	this->cache[hash] = evaluation;
	return evaluation;
}
