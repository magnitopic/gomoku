/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AI.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adiaz-uf <adiaz-uf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 18:05:26 by alaparic          #+#    #+#             */
/*   Updated: 2025/10/15 18:36:31 by adiaz-uf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ai/AI.hpp"
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()

// Static variable to ensure srand is called only once
static bool randomInitialized = false;

AI::AI() 
{
	this->difficulty = "";
	if (!randomInitialized)
	{
		srand(static_cast<unsigned int>(time(NULL)));  // Initialize random seed once
		randomInitialized = true;
	}
}

AI::AI(std::string difficulty) 
{
	this->difficulty = difficulty;
	if (!randomInitialized)
	{
		srand(static_cast<unsigned int>(time(NULL)));  // Initialize random seed once
		randomInitialized = true;
	}
}

AI::AI(const AI &copy)
{
	*this = copy;
}

AI &AI::operator=(const AI &assign)
{
	if (this != &assign)
	{
		this->cache = assign.cache;
		this->difficulty = assign.difficulty;
	}
	return *this;
}

AI::~AI() {}

std::pair<int, int> AI::getAIMove(Board *board, int color)
{
	s_move *bestMove = new s_move{-1, -1, 0};

	this->minMax(board, 0, INT_MIN, INT_MAX, true, color, bestMove, this->difficulty, MAX_DEPTH);

	/* std::cout << T_CYAN << "(AI) chooses move: (" << bestMove->row << ", " << bestMove->col << ") With a score of: " << bestMove->score << T_GRAY << std::endl; */
	std::pair<int, int> result = std::make_pair(bestMove->row, bestMove->col);
	delete bestMove;
	return result;
}
