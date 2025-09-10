/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:36:17 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/10 22:36:29 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ai/ai_config.hpp"

// TODO -> optimize by using board->checkWin, passing the x y values that have changed
/* bool hasWon(Board *board, int player)
{
	for (int x = 0; x < board->getSize(); x++)
	{
		for (int y = 0; y < board->getSize(); y++)
		{
			for (const auto &direction : DIRECTIONS)
			{
				if (board->checkNInARow({x, y}, player, direction, 5))
					return true;
			}
		}
	}
	return false;
}

bool isIllegalMove(Board *board, int x, int y, int player)
{
	if (board->inBounds(x, y))
		return true;

	if (checkOccupiedCell(*board, x, y))
		return true;

	if (board->getGameMode() == "standard")
	{
		if (checkDoubleThree(*board, std::make_pair(x, y), player))
			return true;
		if (checkMoveIntoCapture(*board, std::make_pair(x, y), player))
			return true;
	}
	return false;
} */

/* e_patterns classifyPattern(std::vector<int> line, int player)
{
	int opponent = -player;
	std::vector<std::pair<std::vector<int>, e_patterns>> patterns = {
		{{player, player, player, player, player}, e_FIVE_IN_A_ROW},
		{{0, player, player, player, player, 0}, e_OPEN_FOUR},
		{{opponent, player, player, player, player, 0}, e_FOUR},
		{{player, player, player, 0, player}, e_FOUR},
		{{player, player, 0, player, player}, e_FOUR},
		{{0, player, player, player, 0}, e_OPEN_THREE},
		{{player, player, 0, player}, e_THREE},
		{{opponent, player, player, player, 0}, e_THREE},
		{{opponent, player, player, 0, player, 0}, e_OPEN_THREE},
		{{opponent, player, 0, player, player, 0}, e_OPEN_THREE},
		{{player, player, 0, 0, player}, e_OPEN_THREE},
		{{player, 0, player, 0, player}, e_OPEN_THREE},
		{{opponent, 0, player, player, player, 0, opponent}, e_THREE},
		{{player, 0, 0, 0, player}, e_OPEN_TWO},
		{{0, player, 0, player, 0}, e_OPEN_TWO},
		{{player, 0, 0, player}, e_TWO},
		{{opponent, player, player, 0, 0, 0}, e_TWO},
		{{opponent, player, 0, player, 0, 0}, e_TWO},
		{{opponent, player, 0, 0, player, 0}, e_TWO},
		{{0, player, player, 0, 0}, e_OPEN_TWO},
	};
}
 */