/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board_evaluations.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:49:45 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/25 12:16:29 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ai_config.hpp"
#include <algorithm>
#include <memory>


// CRITICAL: Find immediate win/block positions
static std::vector<std::pair<int, int>> findImmediateThreats(Board *board, int player)
{
	std::vector<std::pair<int, int>> criticalBlocks;

	for (int y = 0; y < board->getSize(); y++)
	{
		for (int x = 0; x < board->getSize(); x++)
		{
			if (board->get(x, y) != EMPTY)
				continue;

			board->set(x, y, player);
			bool isWinningMove = board->checkWin(x, y, player);
			board->set(x, y, EMPTY);

			if (isWinningMove)
			{
				criticalBlocks.push_back({x, y});
			}
		}
	}

	return criticalBlocks;
}

// ENHANCED: Aggressive pattern analysis with offensive bias
static t_pattern analyzeSequence(Board *board, int startX, int startY,
								 const std::pair<int, int> &direction, int player)
{
	t_pattern pattern;
	pattern.length = 0;
	pattern.openEnds = 0;
	pattern.player = player;
	pattern.hasGap = false;
	pattern.threatLevel = 0;
	pattern.blockPositions.clear();

	int dx = direction.first;
	int dy = direction.second;

	// Count consecutive stones in both directions
	int count = 1;
	std::vector<std::pair<int, int>> sequence;
	sequence.push_back({startX, startY});

	// Check positive direction
	for (int i = 1; i < 5; i++)
	{
		int nx = startX + dx * i;
		int ny = startY + dy * i;

		if (!board->inBounds(nx, ny))
			break;

		if (board->get(nx, ny) == player)
		{
			count++;
			sequence.push_back({nx, ny});
		}
		else
		{
			if (board->get(nx, ny) == EMPTY && count >= 2)
			{
				pattern.blockPositions.push_back({nx, ny});
			}
			break;
		}
	}

	// Check negative direction
	for (int i = 1; i < 5; i++)
	{
		int nx = startX - dx * i;
		int ny = startY - dy * i;

		if (!board->inBounds(nx, ny))
			break;

		if (board->get(nx, ny) == player)
		{
			count++;
			sequence.insert(sequence.begin(), {nx, ny});
		}
		else
		{
			if (board->get(nx, ny) == EMPTY && count >= 2)
			{
				pattern.blockPositions.push_back({nx, ny});
			}
			break;
		}
	}

	pattern.length = count;

	// AGGRESSIVE THREAT ASSESSMENT
	if (count >= 4)
	{
		pattern.threatLevel = 5; // IMMEDIATE WIN/BLOCK
	}
	else if (count == 3)
	{
		// Check extensions more aggressively
		int extensions = 0;
		int nx1 = startX + dx * count;
		int ny1 = startY + dy * count;
		int nx2 = startX - dx;
		int ny2 = startY - dy;

		if (board->inBounds(nx1, ny1) && board->get(nx1, ny1) == EMPTY)
		{
			extensions++;
			pattern.blockPositions.push_back({nx1, ny1});
		}
		if (board->inBounds(nx2, ny2) && board->get(nx2, ny2) == EMPTY)
		{
			extensions++;
			pattern.blockPositions.push_back({nx2, ny2});
		}

		if (extensions >= 2)
		{
			pattern.threatLevel = 4; // VERY HIGH - open three
			pattern.openEnds = 2;
		}
		else if (extensions >= 1)
		{
			pattern.threatLevel = 4; // Still high priority
			pattern.openEnds = 1;
		}
	}
	else if (count == 2)
	{
		// Be more aggressive about 2-in-a-row patterns
		pattern.threatLevel = 3; // Higher than before

		// Check for potential to become 3
		int potential = 0;
		for (int i = -2; i <= 2; i++)
		{
			int nx = startX + dx * i;
			int ny = startY + dy * i;
			if (board->inBounds(nx, ny) && board->get(nx, ny) == EMPTY)
			{
				potential++;
			}
		}

		if (potential >= 2)
		{
			pattern.threatLevel = 3; // Good building potential
		}
	}

	return pattern;
}

std::vector<t_pattern> detectPatterns(Board *board, int color)
{
	std::vector<t_pattern> allPatterns;

	// Scan the entire board for patterns of the specified color
	for (int y = 0; y < board->getSize(); y++)
	{
		for (int x = 0; x < board->getSize(); x++)
		{
			if (board->get(x, y) != color)
				continue;

			// Check all four directions from this stone
			for (const auto &direction : DIRECTIONS)
			{
				t_pattern pattern = analyzeSequence(board, x, y, direction, color);

				if (pattern.length >= 2)
				{
					allPatterns.push_back(pattern);
				}
			}
		}
	}

	return allPatterns;
}

std::vector<std::pair<int, int>> findCriticalMoves(Board *board, int opponent)
{
	std::vector<std::pair<int, int>> criticalMoves;

	// FIRST: Check for immediate winning threats
	std::vector<std::pair<int, int>> immediateThreats = findImmediateThreats(board, opponent);
	if (!immediateThreats.empty())
	{
		return immediateThreats;
	}

	// SECOND: Check for 3+ in-a-row patterns
	std::vector<t_pattern> patterns = detectPatterns(board, opponent);

	for (const auto &pattern : patterns)
	{
		if (pattern.threatLevel >= 4)
		{
			for (const auto &pos : pattern.blockPositions)
			{
				// Quick validation
				if (board->inBounds(pos.first, pos.second) &&
					board->isEmpty(pos.first, pos.second))
				{
					criticalMoves.push_back(pos);
				}
			}
		}
	}

	// Remove duplicates
	std::sort(criticalMoves.begin(), criticalMoves.end());
	criticalMoves.erase(std::unique(criticalMoves.begin(), criticalMoves.end()), criticalMoves.end());

	return criticalMoves;
}

// AGGRESSIVE: Enhanced evaluation with offensive bias
static int evaluatePatterns(const std::vector<t_pattern> &patterns, int currentPlayer)
{
	int score = 0;
	int myHighThreats = 0;
	int myMediumThreats = 0;
	int opponentHighThreats = 0;

	// ENHANCED SCORING WITH OFFENSIVE BIAS
	for (const auto &pattern : patterns)
	{
		if (pattern.length >= 5)
		{
			return (pattern.player == currentPlayer) ? FIVE_IN_A_ROW : -FIVE_IN_A_ROW;
		}

		int baseScore = 0;

		// More aggressive scoring
		if (pattern.threatLevel == 5)
		{
			baseScore = 100000; // Immediate threat
		}
		else if (pattern.threatLevel == 4)
		{
			baseScore = 15000; // Increased from 10000
		}
		else if (pattern.threatLevel == 3)
		{
			baseScore = 2000; // Increased from 1000
		}
		else if (pattern.threatLevel == 2)
		{
			baseScore = 300; // Increased from 100
		}

		// MAJOR BONUS for open patterns (more aggressive)
		if (pattern.openEnds == 2)
		{
			baseScore = baseScore * 2; // Double bonus!
		}
		else if (pattern.openEnds == 1)
		{
			baseScore = baseScore * 3 / 2; // 1.5x bonus
		}

		// OFFENSIVE BIAS: Our patterns get extra bonus
		if (pattern.player == currentPlayer)
		{
			score += baseScore * 3 / 2; // 1.5x bonus for our patterns!

			if (pattern.threatLevel >= 4)
				myHighThreats++;
			else if (pattern.threatLevel >= 3)
				myMediumThreats++;
		}
		else
		{
			score -= baseScore; // Normal penalty for opponent

			if (pattern.threatLevel >= 4)
				opponentHighThreats++;
		}
	}

	// BONUS FOR MULTIPLE THREATS (forcing moves)
	if (myHighThreats >= 2)
	{
		score += 10000; // Double threat bonus!
		std::cout << T_GREEN << "DOUBLE HIGH THREAT BONUS!" << T_RESET << std::endl;
	}
	if (myMediumThreats >= 3)
	{
		score += 5000; // Multiple medium threats
	}

	// PENALTY for opponent multiple threats (but less than before)
	if (opponentHighThreats >= 2)
	{
		score -= 15000; // Reduced penalty to encourage aggression
	}
	if (opponentHighThreats >= 1 && myHighThreats == 0)
	{
		score -= 8000; // Reduced emergency penalty
	}

	return score;
}

// ENHANCED: More aggressive positional evaluation
static int evaluatePosition(Board *board, int player)
{
	int positionScore = 0;
	int center = board->getSize() / 2;
	int opponent = (player == BLACK_STONE) ? WHITE_STONE : BLACK_STONE;

	// AGGRESSIVE CENTER CONTROL
	for (int y = std::max(0, center - 3); y <= std::min(board->getSize() - 1, center + 3); y++)
	{
		for (int x = std::max(0, center - 3); x <= std::min(board->getSize() - 1, center + 3); x++)
		{
			int cellValue = board->get(x, y);
			int distance = abs(x - center) + abs(y - center);
			int bonus = (4 - distance) * 15; // Increased center bonus

			if (cellValue == player)
			{
				positionScore += bonus;
			}
			else if (cellValue == opponent)
			{
				positionScore -= bonus / 2; // Less penalty for opponent center control
			}
		}
	}

	// BONUS FOR STONE DENSITY (aggressive clustering)
	std::vector<std::pair<int, int>> myStones;
	for (int y = 0; y < board->getSize(); y++)
	{
		for (int x = 0; x < board->getSize(); x++)
		{
			if (board->get(x, y) == player)
			{
				myStones.push_back({x, y});
			}
		}
	}

	// Bonus for stones close to each other (promotes pattern building)
	int densityBonus = 0;
	for (size_t i = 0; i < myStones.size(); i++)
	{
		for (size_t j = i + 1; j < myStones.size(); j++)
		{
			int dist = abs(myStones[i].first - myStones[j].first) +
					   abs(myStones[i].second - myStones[j].second);
			if (dist <= 3)
			{
				densityBonus += (4 - dist) * 20; // Reward close stones
			}
		}
	}

	positionScore += densityBonus;
	return positionScore;
}

int staticBoardEvaluation(Board *board, int player, int depth)
{
	std::cout << T_BLUE << "AGGRESSIVE Evaluation v3! Depth: " << depth << std::endl;

	// Get all patterns for both players
	std::vector<t_pattern> myPatterns = detectPatterns(board, player);
	std::vector<t_pattern> opponentPatterns = detectPatterns(board, (player == BLACK_STONE) ? WHITE_STONE : BLACK_STONE);

	// Combine patterns for evaluation
	std::vector<t_pattern> allPatterns = myPatterns;
	allPatterns.insert(allPatterns.end(), opponentPatterns.begin(), opponentPatterns.end());

	int patternScore = evaluatePatterns(allPatterns, player);
	int positionScore = evaluatePosition(board, player);

	int finalScore = patternScore + positionScore;

	std::cout << "Pattern: " << patternScore << ", Position: " << positionScore
			  << ", Final: " << finalScore << T_RESET << std::endl;

	return finalScore;
}