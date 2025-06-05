/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board_checks.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 05:50:12 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/05 13:27:47 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/gomoku.hpp"
#include "classes/Board.hpp"

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

		// Check forward
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
		{
			freeThreeCount++;
			std::cout << T_YELLOW << "Found free three!" << T_GRAY << std::endl;
		}
		else if (stones == 3 && ((spacesBefore == 2 && spacesAfter == 1) || (spacesBefore == 1 && spacesAfter == 2)) && separateThree)
		{
			freeThreeCount++;
			std::cout << T_YELLOW << "Found free three with a space!" << T_GRAY << std::endl;
		}
	}
	return freeThreeCount;
}

/*********************
 Main check functions
 *********************/

bool checkOccupiedCell(const Board &board, int x, int y)
{
	if (!board.inBounds(x, y))
		return false;
	return (board.get(x, y) != EMPTY);
}

bool checkDoubleThree(Board &board, const std::pair<int, int> &lastMove, int color)
{
	int x = lastMove.first;
	int y = lastMove.second;
	board.set(x, y, color);

	int freeThreeCount = checkThree(board, lastMove, color);
	board.set(x, y, EMPTY);

	return (freeThreeCount >= 2);
}
