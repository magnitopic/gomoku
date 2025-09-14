/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_moves.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:19:20 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/14 17:06:50 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <set>
#include <algorithm>
#include <thread>
#include <future>
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

std::vector<std::pair<int, int>> getValidMoves(Board *board, int color)
{
	// use a set to avoid duplicate moves
	std::set<std::pair<int, int>> validMoves;
	std::vector<std::pair<int, int>> occupiedTiles = board->getOccupiedTiles();
	for (const std::pair<int, int> &tile : occupiedTiles)
	{
		int row = tile.first;
		int col = tile.second;

		// Check all 4 directions for valid moves
		for (const std::pair<int, int> &dir : DIRECTIONS)
		{
			for (int i = -1; i <= 1; i += 2)
			{
				int newRow = row + dir.first * i;
				int newCol = col + dir.second * i;
				if (!checkIllegalMove(board, {newRow, newCol}, color))
					validMoves.insert({newRow, newCol});
			}
		}
	}

	// convert set back to vector
	return std::vector<std::pair<int, int>>(validMoves.begin(), validMoves.end());
}
