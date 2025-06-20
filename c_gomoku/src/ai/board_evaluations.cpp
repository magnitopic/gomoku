/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board_evaluations.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:49:45 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/20 12:37:08 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ai_config.hpp"

static bool isOpen(Board *board, int x, int y)
{
	return board->inBounds(x, y) && board->get(x, y) == EMPTY;
}

static t_pattern analyzeLine(Board *board, int startX, int startY, int dx, int dy, int color)
{
	t_pattern pattern = {0, 0, false, -1, 0};

	// Count stones and check for patterns with gaps
	int x = startX;
	int y = startY;
	int consecutiveStones = 0;
	int totalStones = 0;
	int maxConsecutive = 0;
	int emptyCount = 0;
	int sequenceLength = 0;

	// First, check backward for open end
	if (isOpen(board, startX - dx, startY - dy))
		pattern.openEnds++;

	// Analyze forward direction
	std::vector<int> sequence; // 1 for stone, 0 for empty, -1 for enemy/boundary

	for (int i = 0; i < 6; i++)
	{ // Check up to 6 positions
		if (!board->inBounds(x, y))
		{
			sequence.push_back(-1);
			break;
		}

		int cell = board->get(x, y);
		if (cell == color)
		{
			sequence.push_back(1);
			consecutiveStones++;
			totalStones++;
			maxConsecutive = std::max(maxConsecutive, consecutiveStones);
		}
		else if (cell == EMPTY)
		{
			sequence.push_back(0);
			emptyCount++;
			if (consecutiveStones > 0 && emptyCount == 1 && !pattern.hasGap)
			{
				// First gap encountered
				pattern.hasGap = true;
				pattern.gapPosition = i;
			}
			consecutiveStones = 0;

			// Stop if we encounter second empty space
			if (emptyCount > 1)
				break;
		}
		else
		{
			// Enemy stone or boundary
			sequence.push_back(-1);
			break;
		}

		x += dx;
		y += dy;
		sequenceLength++;
	}

	// Check for open end at the end
	if (sequenceLength > 0 && sequence.back() == 0)
		pattern.openEnds++;
	else if (board->inBounds(x, y) && board->get(x, y) == EMPTY)
		pattern.openEnds++;

	pattern.length = maxConsecutive;
	pattern.totalPotential = totalStones;

	// Special pattern recognition for gaps
	if (pattern.hasGap && pattern.totalPotential >= 4)
	{
		// This could be a dangerous pattern like XXX_X or XX_XX
		pattern.length = pattern.totalPotential; // Treat as potential threat
	}

	return pattern;
}

std::vector<t_pattern> detectPatterns(Board *board, std::pair<int, int> start, std::pair<int, int> direction, int color)
{
	std::vector<t_pattern> patterns;

	int x = start.first;
	int y = start.second;
	int dx = direction.first;
	int dy = direction.second;

	t_pattern forward = analyzeLine(board, x, y, dx, dy, color);
	t_pattern backward = analyzeLine(board, x, y, -dx, -dy, color);

	t_pattern combinedPattern;
	combinedPattern.length = forward.length + backward.length;
	combinedPattern.openEnds = forward.openEnds + backward.openEnds;
	combinedPattern.hasGap = forward.hasGap || backward.hasGap;
	combinedPattern.totalPotential = forward.totalPotential + backward.totalPotential - 1; // Subtract 1 for the overlap at the start position

	if (isOpen(board, x + dx * forward.length, y + dy * forward.length))
		combinedPattern.openEnds++;
	if (isOpen(board, x - dx * backward.length, y - dy * backward.length))
		combinedPattern.openEnds++;

	if (combinedPattern.length >= 2)
		patterns.push_back(combinedPattern);

	return patterns;
}

int evaluatePosition(Board *board, int x, int y, int color)
{
	int score = 0;

	for (std::pair<int, int> direction : DIRECTIONS)
	{
		std::vector<s_pattern> patterns = detectPatterns(board, {x, y}, direction, color);

		for (const s_pattern &pattern : patterns)
		{
			if (pattern.length >= 5)
				return FIVE_IN_A_ROW;
			else if (pattern.openEnds == 2 && pattern.length == 4)
				score += OPEN_FOUR;
			else if (pattern.openEnds == 1 && pattern.length == 4)
				score += FOUR;
			else if (pattern.openEnds == 2 && pattern.length == 3)
				score += OPEN_THREE;
			else if (pattern.openEnds == 1 && pattern.length == 3)
				score += THREE;
			else if (pattern.openEnds == 2 && pattern.length == 2)
				score += OPEN_TWO;
			else if (pattern.openEnds == 1 && pattern.length == 2)
				score += TWO;

			// Bonus for patterns with strategic gaps
			if (pattern.hasGap && pattern.totalPotential >= 4)
				score += THREE;
		}
	}

	return score;
}

int staticBoardEvaluation(Board *board, int player)
{
	int blackScore = 0;
	int whiteScore = 0;

	board->getOccupiedTiles();

	for (const auto &tile : board->getOccupiedTiles())
	{
		int posScore = evaluatePosition(board, tile.first, tile.second, player);
		if (board->get(tile.first, tile.second) == BLACK_STONE)
			blackScore += posScore;
		else
			whiteScore += posScore;
	}

	return whiteScore - blackScore;
}