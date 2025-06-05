/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board_checks.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 05:50:12 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/05 11:33:38 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/gomoku.hpp"
#include "classes/Board.hpp"

/******************************
 Helper functions
 ******************************/

/*********************
 Main check functions
 *********************/

bool checkOccupiedCell(const Board &board, int x, int y)
{
	if (!board.inBounds(x, y))
		return false;
	return (board.get(x, y) != EMPTY);
}

bool checkDoubleThree(const Board &board, const std::pair<int, int> &lastMove, int color)
{
	return false;
}
