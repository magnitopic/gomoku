/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_threats.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:42:12 by adiaz-uf          #+#    #+#             */
/*   Updated: 2025/10/15 13:27:23 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ai/AI.hpp"

/**
 * Evaluates the threat/opportunity score of a specific position.
 * Returns a score based on patterns that would be created by playing at this position.
 * Higher scores indicate better positions.
 *
 * Score breakdown:
 * - AI immediate win: 100000
 * - Block opponent win: 99999
 * - AI open four: 4000
 * - AI four: 3500
 * - Block opponent open four: 3000
 * - Block opponent four: 2500
 * - AI open three: 1500
 * - Block opponent open three: 1000
 *
 * @param board The current board state
 * @param move The position to evaluate
 * @param aiColor The AI's color (BLACK_STONE or WHITE_STONE)
 * @return The threat/opportunity score for this position
 */
int AI::evaluateThreatScore(Board *board, const std::pair<int, int> &move, int aiColor)
{
	int opponentColor = -aiColor;
	int totalScore = 0;

	// FIRST PRIORITY: Check if AI can win in next move (immediate win)
	board->set(move.first, move.second, aiColor);

	if (board->simpleCheckWin(move.first, move.second, aiColor))
	{
		board->set(move.first, move.second, EMPTY);
		return 100000; // Immediate win - highest score!
	}

	board->set(move.first, move.second, EMPTY);

	// SECOND PRIORITY: Check if opponent can win in next move (must block)
	board->set(move.first, move.second, opponentColor);

	if (board->simpleCheckWin(move.first, move.second, opponentColor))
	{
		board->set(move.first, move.second, EMPTY);
		return 99999; // Must block opponent win!
	}

	board->set(move.first, move.second, EMPTY);

	// THIRD PRIORITY: Evaluate AI offensive patterns
	board->set(move.first, move.second, aiColor);

	bool aiHasOpenFour = false;
	bool aiHasFour = false;
	bool aiHasOpenThree = false;

	for (const std::pair<int, int> &direction : DIRECTIONS)
	{
		std::vector<int> line;

		for (int offset = -5; offset <= 5; offset++)
		{
			int checkX = move.first + (direction.first * offset);
			int checkY = move.second + (direction.second * offset);

			if (board->inBounds(checkX, checkY))
				line.push_back(board->get(checkX, checkY));
		}

		if (!line.empty())
		{
			std::vector<s_pattern> patterns = this->findPatterns(line, aiColor);

			for (const s_pattern &pattern : patterns)
			{
				if (pattern.type == e_OPEN_FOUR)
					aiHasOpenFour = true;
				else if (pattern.type == e_FOUR)
					aiHasFour = true;
				else if (pattern.type == e_OPEN_THREE)
					aiHasOpenThree = true;
			}
		}
	}

	board->set(move.first, move.second, EMPTY);

	// FOURTH PRIORITY: Evaluate opponent defensive patterns
	board->set(move.first, move.second, opponentColor);

	bool oppHasOpenFour = false;
	bool oppHasFour = false;
	bool oppHasOpenThree = false;

	for (const std::pair<int, int> &direction : DIRECTIONS)
	{
		std::vector<int> line;

		for (int offset = -5; offset <= 5; offset++)
		{
			int checkX = move.first + (direction.first * offset);
			int checkY = move.second + (direction.second * offset);

			if (board->inBounds(checkX, checkY))
				line.push_back(board->get(checkX, checkY));
		}

		if (!line.empty())
		{
			std::vector<s_pattern> patterns = this->findPatterns(line, opponentColor);

			for (const s_pattern &pattern : patterns)
			{
				if (pattern.type == e_OPEN_FOUR)
					oppHasOpenFour = true;
				else if (pattern.type == e_FOUR)
					oppHasFour = true;
				else if (pattern.type == e_OPEN_THREE)
					oppHasOpenThree = true;
			}
		}
	}

	board->set(move.first, move.second, EMPTY);

	// Calculate total score based on patterns found
	if (aiHasOpenFour)
		totalScore += 4000;
	else if (aiHasFour)
		totalScore += 3500;
	else if (aiHasOpenThree)
		totalScore += 1500;

	if (oppHasOpenFour)
		totalScore += 3000;
	else if (oppHasFour)
		totalScore += 2500;
	else if (oppHasOpenThree)
		totalScore += 1000;

	return totalScore;
}
