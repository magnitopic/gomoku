/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_threats.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adiaz-uf <adiaz-uf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:42:12 by adiaz-uf          #+#    #+#             */
/*   Updated: 2025/10/15 10:41:41 by adiaz-uf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ai/AI.hpp"

/**
 * Detects immediate threats from the opponent (open three or four in a row)
 * and returns the best blocking position.
 * This function is called before minmax to provide faster response to critical threats.
 * 
 * @param board The current board state
 * @param aiColor The AI's color (BLACK_STONE or WHITE_STONE)
 * @return A pointer to the blocking position, or nullptr if no immediate threat
 */
std::pair<int, int> *AI::detectImmediateThreat(Board *board, int aiColor)
{
	int opponentColor = -aiColor;
	std::vector<std::pair<int, int>> adjacentMoves = board->getAdjacentEmptyPositions();
	std::vector<std::pair<int, int>> validMoves;

	if (board->getGameMode() == "subject")
	{
		for (const std::pair<int, int> &move : adjacentMoves)
		{
			if (!this->checkIllegalMove(board, move, aiColor))
				validMoves.push_back(move);
		}
	}
	else
	{
		// In non-standard modes, all adjacent empty positions are valid
		validMoves = adjacentMoves;
	}
	
	// Structure to hold threat information
	struct ThreatInfo {
		std::pair<int, int> position;
		int priority;
		int score;
	};
	
	std::vector<ThreatInfo> threats;
	
	for (const std::pair<int, int> &move : validMoves)
	{
		// 1. check if AI can win in next move
		// Check what happens if AI plays at this position
		board->set(move.first, move.second, aiColor);
		
		// Check if AI would win
		if (board->simpleCheckWin(move.first, move.second, aiColor))
		{
			board->set(move.first, move.second, EMPTY);
			// AI can win next move, must block!
			return new std::pair<int, int>(move);
		}
		
		// 2. check if AI can win in next move
		// Check what happens if OPPONENT plays at this position
		board->set(move.first, move.second, opponentColor);
		
		// Check if opponent would win
		if (board->simpleCheckWin(move.first, move.second, opponentColor))
		{
			board->set(move.first, move.second, EMPTY);
			// opponent can win next move, must block!
			return new std::pair<int, int>(move);
		}
		
		// Check if opponent would create an open four or four
		bool hasOpenFour = false;
		bool hasFour = false;
		int maxScore = 0;
		
		for (const std::pair<int, int> &direction : DIRECTIONS)
		{
			std::vector<int> line;
			
			// Build line in this direction centered on the move
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
					{
						hasOpenFour = true;
						maxScore = std::max(maxScore, pattern.score);
					}
					else if (pattern.type == e_FOUR)
					{
						hasFour = true;
						maxScore = std::max(maxScore, pattern.score);
					}
				}
			}
		}
		
		board->set(move.first, move.second, EMPTY);
		
		if (hasOpenFour)
			threats.push_back({move, 2000, maxScore}); // Open four is very urgent
		else if (hasFour)
			threats.push_back({move, 1500, maxScore}); // Four is urgent
	}
	
	// Second, check for existing open threes that need blocking
	for (const std::pair<int, int> &move : validMoves)
	{
		// Skip if already identified as higher priority threat
		bool alreadyFound = false;
		for (const ThreatInfo &threat : threats)
		{
			if (threat.position == move && threat.priority >= 1500)
			{
				alreadyFound = true;
				break;
			}
		}
		if (alreadyFound)
			continue;
		
		// Check if placing AI stone here blocks an existing open three
		bool blocksOpenThree = false;
		int maxScore = 0;
		
		for (const std::pair<int, int> &direction : DIRECTIONS)
		{
			std::vector<int> lineWithoutBlock;
			std::vector<int> lineWithBlock;
			
			// Build lines in this direction
			for (int offset = -5; offset <= 5; offset++)
			{
				int checkX = move.first + (direction.first * offset);
				int checkY = move.second + (direction.second * offset);
				
				if (board->inBounds(checkX, checkY))
				{
					int value = board->get(checkX, checkY);
					lineWithoutBlock.push_back(value);
					lineWithBlock.push_back(offset == 0 ? aiColor : value);
				}
			}
			
			// Check if opponent has open three in original line
			if (!lineWithoutBlock.empty())
			{
				std::vector<s_pattern> patternsWithout = this->findPatterns(lineWithoutBlock, opponentColor);
				std::vector<s_pattern> patternsWith = this->findPatterns(lineWithBlock, opponentColor);
				
				// Check if we had an open three that gets blocked
				bool hadOpenThree = false;
				for (const s_pattern &pattern : patternsWithout)
				{
					if (pattern.type == e_OPEN_THREE)
					{
						hadOpenThree = true;
						maxScore = std::max(maxScore, pattern.score);
						break;
					}
				}
				
				// Check that placing AI stone removes the threat
				bool stillHasOpenThree = false;
				for (const s_pattern &pattern : patternsWith)
				{
					if (pattern.type == e_OPEN_THREE)
					{
						stillHasOpenThree = true;
						break;
					}
				}
				
				if (hadOpenThree && !stillHasOpenThree)
					blocksOpenThree = true;
			}
		}
		
		if (blocksOpenThree)
			threats.push_back({move, 1000, maxScore}); // Open three blocking
	}
	
	// If we found threats, return the highest priority one
	if (!threats.empty())
	{
		// Sort by priority (descending), then by score
		std::sort(threats.begin(), threats.end(),
			[](const ThreatInfo &a, const ThreatInfo &b) {
				if (a.priority != b.priority)
					return a.priority > b.priority;
				return a.score > b.score;
			});
		
		return new std::pair<int, int>(threats[0].position);
	}
	
	return nullptr;
}
