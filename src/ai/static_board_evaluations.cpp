/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_board_evaluations.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:49:45 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/06 14:43:19 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <memory>
#include "../../include/ai/ai_config.hpp"

int staticBoardEvaluation(Board *board, int player)
{
	// Check for immediate win/loss
	if (hasWon(board, player))
		return INT_MAX;
	if (hasWon(board, player * -1))
		return INT_MIN;

	int score = 0;

	for (int x = 0; x < board->getSize(); x++)
	{
		for (int y = 0; y < board->getSize(); y++)
		{
			if (board->get(x, y) == player)
				score += evaluatePosition(board, x, y, player);
			else if (board->get(x, y) == (player * -1))
				score -= evaluatePosition(board, x, y, player * -1);
		}
	}

	return score;
}

int evaluatePosition(Board *board, int row, int col, int player)
{
	int score = 0;

	for (std::pair<int, int> direction : DIRECTIONS)
	{
		int count = 1; // This piece
		bool blocked1 = false, blocked2 = false;

		// Count in positive direction
		for (int i = 1; i < 5; i++)
		{
			int newRow = row + i * direction.first;
			int newCol = col + i * direction.second;

			if (isIllegalMove(board, newRow, newCol, player) ||
				board->get(newRow, newCol) == player * -1)
			{
				blocked1 = true;
				break;
			}
			else if (board->get(newRow, newCol) == player)
			{
				count++;
			}
			else
			{
				break; // Empty space
			}
		}

		// Count in negative direction
		for (int i = 1; i < 5; i++)
		{
			int newRow = row - i * direction.first;
			int newCol = col - i * direction.second;

			if (isIllegalMove(board, newRow, newCol, player) ||
				board->get(newRow, newCol) == player * -1)
			{
				blocked2 = true;
				break;
			}
			else if (board->get(newRow, newCol) == player)
			{
				count++;
			}
			else
			{
				break; // Empty space
			}
		}

		if (count >= 5)
		{
			score += FIVE_IN_A_ROW;
		}
		else if (count == 4)
		{
			if (!blocked1 || !blocked2)
				score += OPEN_FOUR;
			else
				score += FOUR;
		}
		else if (count == 3)
		{
			if (!blocked1 && !blocked2)
				score += OPEN_THREE;
			else if (!blocked1 || !blocked2)
				score += OPEN_THREE;
			else
				score += THREE;
		}
		else if (count == 2)
		{
			if (!blocked1 && !blocked2)
				score += OPEN_TWO;
			else if (!blocked1 || !blocked2)
				score += TWO;
			else
				score += BLOCKED_TWO;
		}
	}
}
