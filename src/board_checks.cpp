/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board_checks.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 05:50:12 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/28 17:33:27 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/gomoku.hpp"
#include "../include/classes/Board.hpp"

/******************************
 Helper functions
 ******************************/

static int checkThree(Board &board, const std::pair<int, int> &lastMove, int color)
{
	int x = lastMove.first;
	int y = lastMove.second;
	int freeThreeCount = 0;

	for (std::pair<int, int> direction : DIRECTIONS)
	{
		// Initial values
		int dx = direction.first;
		int dy = direction.second;
		int stones = 1;
		int separateThree = false;

		// Check forwards
		int spacesAfter = 0;
		int i = 1;
		while (42)
		{
			int nx = x + dx * i;
			int ny = y + dy * i;
			if (!board.inBounds(nx, ny))
				break;

			if (board.get(nx, ny) == color)
			{
				stones++;
				i++;
			}
			else if (board.get(nx, ny) == EMPTY)
			{
				spacesAfter++;
				i++;
				nx = x + dx * i;
				ny = y + dy * i;
				if (!board.inBounds(nx, ny))
					break;

				// If after the space there is a stone of the same color, continue counting
				if (board.get(nx, ny) == color)
				{
					stones++;
					i++;
					separateThree = true;
				}
				else
					break;
			}
			else
				break;
		}

		// Check backwards
		int spacesBefore = 0;
		i = 1;
		while (42)
		{
			int nx = x - dx * i;
			int ny = y - dy * i;
			if (!board.inBounds(nx, ny))
				break;

			if (board.get(nx, ny) == color)
			{
				stones++;
				i++;
			}
			else if (board.get(nx, ny) == EMPTY)
			{
				spacesBefore++;
				i++;
				nx = x - dx * i;
				ny = y - dy * i;
				if (!board.inBounds(nx, ny))
					break;

				// If after the space there is a stone of the same color, continue counting
				if (board.get(nx, ny) == color)
				{
					stones++;
					i++;
					separateThree = true;
				}
				else
					break;
			}
			else
				break;
		}

		// Check if we have a free three
		if (stones == 3 && spacesBefore > 0 && spacesAfter > 0 && !separateThree)
			freeThreeCount++;
		else if (stones == 3 && ((spacesBefore == 2 && spacesAfter == 1) || (spacesBefore == 1 && spacesAfter == 2)) && separateThree)
			freeThreeCount++;
	}
	return freeThreeCount;
}

/*********************
 Main check functions
 *********************/

bool checkDoubleThree(Board &board, const std::pair<int, int> &lastMove, int color)
{
	int x = lastMove.first;
	int y = lastMove.second;

	board.set(x, y, color);
	int freeThreeCount = checkThree(board, lastMove, color);
	board.set(x, y, EMPTY);

	return (freeThreeCount >= 2);
}

bool checkMoveIntoCapture(Board &board, const std::pair<int, int> &lastMove, int color)
{
	int x = lastMove.first;
	int y = lastMove.second;
	bool capturePossible = false;

	board.set(x, y, color);

	for (std::pair<int, int> direction : DIRECTIONS)
	{
		int dx = direction.first;
		int dy = direction.second;

		if (board.get(x + dx, y + dy) == color && board.get(x + dx * 2, y + dy * 2) == -color &&
			board.get(x - dx, y - dy) == -color)
			capturePossible = true;

		if (board.get(x - dx, y - dy) == color && board.get(x - dx * 2, y - dy * 2) == -color &&
			board.get(x + dx, y + dy) == -color)
			capturePossible = true;
	}

	board.set(x, y, EMPTY);

	return capturePossible;
}
