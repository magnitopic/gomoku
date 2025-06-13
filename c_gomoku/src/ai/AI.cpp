/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AI.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:02:44 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/13 14:14:08 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AI.hpp"

AI::AI()
{
}

AI::AI(const AI &copy)
{
	*this = copy;
}

AI &AI::operator=(const AI &assign)
{
	if (this != &assign)
	{
	}
	return *this;
}

AI::~AI() {}

/* Functions */
std::pair<int, int> AI::getMove(Board *board, int player)
{
	return getBestMove(board, player);
}

std::pair<int, int> AI::getBestMove(Board *board, int player)
{
	t_move *bestMove = new t_move{-1, -1, 0};

	minMax(board, 0, INT_MIN, INT_MAX, true, player, bestMove);

	return std::make_pair(bestMove->row, bestMove->col);
}
