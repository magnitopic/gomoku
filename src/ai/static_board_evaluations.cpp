/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_board_evaluations.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:49:45 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/10 22:57:45 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <memory>
#include "../../include/ai/ai_config.hpp"

static std::vector<t_pattern> analyzeLine(const std::vector<int> &line, int color)
{
	std::vector<t_pattern> patterns;
	int n = line.size();

	for (int i = 0; i < n; i++)
	{
		if (line[i] != color)
			continue;

		// Find consecutive stones
		int length = 0;
		int j = i;
		while (j < n && line[j] == color)
		{
			length++;
			j++;
		}

		// Check open ends
		int openEnds = 0;
		if (i > 0 && line[i - 1] == EMPTY)
			openEnds++;
		if (j < n && line[j] == EMPTY)
			openEnds++;

		// Create pattern
		t_pattern pattern;
		pattern.length = length;
		pattern.openEnds = openEnds;
		pattern.hasGap = false;
		pattern.gapPosition = -1;
		pattern.totalPotential = length;

		patterns.push_back(pattern);
		i = j - 1; // Skip processed stones
	}

	return patterns;
}

std::vector<t_pattern> detectPatterns(Board *board, int color)
{
	std::vector<t_pattern> patterns;

	for (std::pair<int, int> direction : DIRECTIONS)
	{
		// get the number of lines to check based on direction
		int iterations = direction == DIRECTIONS[0] || direction == DIRECTIONS[1] ? board->getSize() - 1 : (board->getSize() * 2) - 5;

		int starter = direction == DIRECTIONS[0] || direction == DIRECTIONS[1] ? 0 : 3;
		for (int i = starter; i <= iterations; i++)
		{
			std::vector<int> line = board->getLine(direction, i);
			if (line.size() < 5) // if line is less than five no win possible
				continue;

			std::vector<int> filteredLine;
			for (int cell : line)
			{
				if (cell == color || cell == EMPTY)
					filteredLine.push_back(cell);
				else
					filteredLine.push_back(-1);
			}

			std::vector<t_pattern> linePatterns = analyzeLine(filteredLine, color);

			patterns.insert(patterns.end(), linePatterns.begin(), linePatterns.end());
		}
	}

	return patterns;
}

static int evaluatePosition(Board *board, int color)
{
	int score = 0;

	std::vector<t_pattern> patterns = detectPatterns(board, color);

	for (const t_pattern &pattern : patterns)
	{
		// immediate win
		if (pattern.length >= 5)
		{
			/* std::cout << "Found a Five" << std::endl; */
			return FIVE_IN_A_ROW;
		}

		// score based patterns
		if (pattern.length == 4)
		{
			if (pattern.openEnds == 2)
			{
				/* std::cout << "Found a Four open" << std::endl; */
				score += OPEN_FOUR;
			}
			else if (pattern.openEnds == 1)
			{
				/* std::cout << "Found a Four closed" << std::endl; */
				score += FOUR;
			}
		}
		else if (pattern.length == 3)
		{
			if (pattern.openEnds == 2)
			{
				/* std::cout << "Found a Three Open" << std::endl; */
				score += OPEN_THREE;
			}
			else if (pattern.openEnds == 1)
			{
				/* std::cout << "Found a Three closed" << std::endl; */
				score += THREE;
			}
		}
		else if (pattern.length == 2)
		{
			if (pattern.openEnds == 2)
			{
				/* std::cout << "Found a Two open" << std::endl; */
				score += OPEN_TWO;
			}
			else if (pattern.openEnds == 1)
			{
				/* std::cout << "Found a Two closed" << std::endl; */
				score += TWO;
			}
		}

		// Bonus for gap patterns (they can be deceptive)
		if (pattern.hasGap && pattern.totalPotential >= 4)
			score += (pattern.openEnds == 2) ? OPEN_THREE / 2 : THREE / 2;
	}

	return score;
}

int staticBoardEvaluation(Board *board, int player)
{
	std::cout << T_BLUE << "Evaluating board!" << std::endl;
	// Evaluate both colors
	int blackScore = evaluatePosition(board, BLACK_STONE);
	int whiteScore = evaluatePosition(board, WHITE_STONE);

	// Return score from the perspective of the current player
	if (player == BLACK_STONE)
		return blackScore - whiteScore;
	else
		return whiteScore - blackScore;
}
