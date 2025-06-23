/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board_evaluations.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:49:45 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/23 21:13:51 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ai_config.hpp"

static std::vector<t_pattern> analyzeLine(std::vector<int> line)
{
	std::vector<t_pattern> patterns;
	int line_size = static_cast<int>(line.size());

	for (int start = 0; start < line_size; start++)
	{
		int color = line[start];
		if (color == EMPTY)
			continue;

		// Count consecutive stones of this color
		int length = 0;
		int end = start;

		// Look for consecutive stones, allowing for one gap
		while (end < line_size)
		{
			if (line[end] == color)
			{
				length++;
				end++;
			}
			else if (line[end] == EMPTY && end + 1 < line_size && line[end + 1] == color)
			{
				// Found a gap - this creates a "jump" pattern
				t_pattern gapPattern;
				gapPattern.length = length;
				gapPattern.hasGap = true;
				gapPattern.gapPosition = end - start;
				gapPattern.totalPotential = length + 1; // Include the potential stone

				// Check open ends around the gap pattern
				gapPattern.openEnds = 0;
				if (start > 0 && line[start - 1] == EMPTY)
					gapPattern.openEnds++;
				if (end + 2 < line_size && line[end + 2] == EMPTY)
					gapPattern.openEnds++;

				patterns.push_back(gapPattern);
				end += 2; // Skip the gap and next stone
				length++; // Count the stone after gap
			}
			else
			{
				break; // Sequence ended
			}

			// Create pattern for consecutive sequence
			if (length >= 2)
			{
				t_pattern pattern;
				pattern.length = length;
				pattern.hasGap = false;
				pattern.gapPosition = -1;
				pattern.totalPotential = length;

				// Check for open ends
				pattern.openEnds = 0;
				if (start > 0 && line[start - 1] == EMPTY)
					pattern.openEnds++;
				if (end < line_size && line[end] == EMPTY)
					pattern.openEnds++;

				patterns.push_back(pattern);
			}

			start = end - 1; // Continue from where this sequence ended
		}
	}
	return patterns;
}

std::vector<t_pattern> detectPatterns(Board *board, int color)
{
	std::vector<t_pattern> patterns;

	for (std::pair<int, int> direction : DIRECTIONS)
	{
		int iterations = direction == DIRECTIONS[0] || direction == DIRECTIONS[1] ? board->getSize() - 1 : (board->getSize() * 2) - 1;

		for (int i = 0; i <= iterations; i++)
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

			std::vector<t_pattern> linePatterns = analyzeLine(filteredLine);

			patterns.insert(patterns.end(), linePatterns.begin(), linePatterns.end());
		}
	}

	return patterns;
}

static int evaluatePosition(Board *board, int color)
{
	int score = 0;

	std::vector<s_pattern> patterns = detectPatterns(board, color);

	for (const t_pattern &pattern : patterns)
	{
		// immediate win
		if (pattern.length >= 5)
		{
			std::cout << "Found a Five" << std::endl;
			return FIVE_IN_A_ROW;
		}

		// score based patterns
		if (pattern.length == 4)
		{
			if (pattern.openEnds == 2)
			{
				std::cout << "Found a Four open" << std::endl;
				score += OPEN_FOUR;
			}
			else if (pattern.openEnds == 1)
			{
				std::cout << "Found a Four closed" << std::endl;
				score += FOUR;
			}
		}
		else if (pattern.length == 3)
		{
			if (pattern.openEnds == 2)
			{
				std::cout << "Found a Three Open" << std::endl;
				score += OPEN_THREE;
			}
			else if (pattern.openEnds == 1)
			{
				std::cout << "Found a Three closed" << std::endl;
				score += THREE;
			}
		}
		else if (pattern.length == 2)
		{
			if (pattern.openEnds == 2)
			{
				std::cout << "Found a Two open" << std::endl;
				score += OPEN_TWO;
			}
			else if (pattern.openEnds == 1)
			{
				std::cout << "Found a Two closed" << std::endl;
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
