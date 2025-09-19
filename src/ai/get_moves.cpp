/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_moves.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:19:20 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/19 18:50:52 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ai/ai_config.hpp"

static bool checkIllegalMove(Board *board, const std::pair<int, int> &cell, int color)
{
	int col = cell.first;
	int row = cell.second;

	// Check if the move is out of bounds
	if (!board->inBounds(col, row))
		return true;

	// Check if the cell is already occupied
	if (checkOccupiedCell(*board, col, row))
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

static void sortMoves(Board *board, int color, std::vector<std::pair<int, int>> &moves)
{
	std::vector<s_scored_move> scoredMoves;

	for (const auto &move : moves)
	{
		board->set(move.first, move.second, color);
		int score = staticBoardEvaluation(board, color);
		scoredMoves.push_back({move, score});
		board->set(move.first, move.second, EMPTY);
	}

	// Sort scored moves by score
	std::sort(scoredMoves.begin(), scoredMoves.end(),
			  [](const s_scored_move &a, const s_scored_move &b)
			  {
				  return a.score > b.score;
			  });

	// Clear original moves and add sorted moves
	moves.clear();
	for (int i = 0; i < MAX_BRANCHES && i < static_cast<int>(scoredMoves.size()); i++)
		moves.push_back(scoredMoves[i].move);
}

std::vector<std::pair<int, int>> getValidMoves(Board *board, int color)
{
	std::vector<std::pair<int, int>> candidates = board->getAdjacentEmptyPositions();
	std::vector<std::pair<int, int>> validMoves;

	if (board->getGameMode() == "standard")
	{
		for (const auto &move : candidates)
		{
			if (!checkIllegalMove(board, move, color))
				validMoves.push_back(move);
		}
	}
	else
	{
		// In non-standard modes, all adjacent empty positions are valid
		validMoves = candidates;
	}

	sortMoves(board, color, validMoves);
	return validMoves;
}