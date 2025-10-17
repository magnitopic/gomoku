/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_moves.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adiaz-uf <adiaz-uf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:19:20 by adiaz-uf          #+#    #+#             */
/*   Updated: 2025/10/15 18:52:18 by adiaz-uf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ai/AI.hpp"
#include <cstdlib>  // For rand()
#include <cmath>    // For abs()

bool AI::checkIllegalMove(Board *board, const std::pair<int, int> &cell, int color)
{
	int col = cell.first;
	int row = cell.second;

	// Check if the move is out of bounds
	if (!board->inBounds(col, row))
		return true;

	// Check if the cell is already occupied
	if (!board->isEmpty(col, row))
		return true;

	if (board->getGameMode() != "standard")
	{
		// Check if the move creates a double three
		if (checkDoubleThree(*board, cell, color))
			return true;

		// Check if the move is into a capture
		if (checkMoveIntoCapture(*board, cell, color))
			return true;
	}

	return false;
}

int AI::quickEvaluatePos(const std::pair<int, int> &pos, Board *board, int color)
{
	int score = 0;

	// Use the threat evaluation system for pattern-based scoring
	int threatScore = this->evaluateThreatScore(board, pos, color);
	score += threatScore;

	// Add bonus for proximity to existing stones (helps with board control)
	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			if (dx == 0 && dy == 0)
				continue;
			
			int nx = pos.first + dx;
			int ny = pos.second + dy;
			
			if (board->inBounds(nx, ny))
			{
				if (board->get(nx, ny) == color)
					score += 10; // Small bonus for being adjacent to own stone
				else if (board->get(nx, ny) == -color)
					score += 5; // Smaller bonus for being adjacent to opponent stone
			}
		}
	}

	return score;
}

void AI::sortMoves(Board *board, int color, std::vector<std::pair<int, int>> &moves, std::string difficulty)
{
	std::vector<s_scored_move> scoredMoves;
	
	// Determine randomness factor based on difficulty
	float randomFactor = 0.0f;
	if (difficulty.compare("easy") == 0)
		randomFactor = 3.0f;      // 200% randomness - very unpredictable (can reverse order)
	else if (difficulty.compare("middle") == 0)
		randomFactor = 1.5f;      // 100% randomness - significant variation
	else
		randomFactor = 0.0f;      // 0% randomness - perfect play

	// Evaluate all moves and store base scores
	for (const std::pair<int, int> &move : moves)
	{
		board->set(move.first, move.second, color);
		int baseScore = quickEvaluatePos(move, board, color);
		scoredMoves.push_back({move, baseScore});
		board->set(move.first, move.second, EMPTY);
	}

	// Apply randomness to the scores AFTER evaluation
	if (randomFactor > 0.0f)
	{
		for (auto &scoredMove : scoredMoves)
		{
			int baseScore = scoredMove.score;
			
			// Generate random noise proportional to the base score
			// Use a larger base value to ensure significant randomness even for low scores
			int scoreForNoise = std::max(std::abs(baseScore), 1000);  // Minimum 1000 for noise calculation
			int maxNoise = static_cast<int>(scoreForNoise * randomFactor);
			
			// Generate random number in range [-maxNoise, +maxNoise]
			int randValue = rand();
			int noise = (randValue % (2 * maxNoise + 1)) - maxNoise;
			scoredMove.score = baseScore + noise;
		}
	}

	// Sort scored moves by (potentially randomized) score
	std::sort(scoredMoves.begin(), scoredMoves.end(),
		[](const s_scored_move &a, const s_scored_move &b){
			return a.score > b.score;
		});
		
	// Clear original moves and add sorted moves
	moves.clear();
	for (int i = 0; i < MAX_BRANCHES && i < static_cast<int>(scoredMoves.size()); i++)
		moves.push_back(scoredMoves[i].move);
}

std::vector<std::pair<int, int>> AI::getValidMoves(Board *board, int color, std::string difficulty)
{
	std::vector<std::pair<int, int>> candidates = board->getAdjacentEmptyPositions();
	std::vector<std::pair<int, int>> validMoves;

	if (board->getGameMode() == "subject")
	{
		for (const std::pair<int, int> &move : candidates)
		{
			if (!this->checkIllegalMove(board, move, color))
				validMoves.push_back(move);
		}
	}
	else
	{
		// In non-standard modes, all adjacent empty positions are valid
		validMoves = candidates;
	}

	this->sortMoves(board, color, validMoves, difficulty);
	return validMoves;
}
