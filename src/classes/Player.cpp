/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:26:30 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/04 12:59:50 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/classes/Player.hpp"

Player::Player() {}

Player::Player(int color, bool ai)
{
	this->color = color;
	this->name = (color == 1) ? "Black" : "White";
	this->is_ai = ai;
	this->takenStones = 0;
	this->timer = 0;
}

Player::Player(const Player &copy)
{
	*this = copy;
}

Player &Player::operator=(const Player &assign)
{
	if (this != &assign)
	{
		this->color = assign.color;
		this->name = assign.name;
		this->is_ai = assign.is_ai;
		this->takenStones = assign.takenStones;
		this->timer = assign.timer;
	}
	return *this;
}

Player::~Player() {}

/* Getters */

int Player::getColor() const
{
	return this->color;
}

std::string Player::getName() const
{
	return this->name;
}

int Player::getTakenStones() const
{
	return this->takenStones;
}

double Player::getTimer() const
{
	return this->timer;
}

bool Player::isAI() const
{
	return this->is_ai;
}

/* Methods */

void Player::startTimer()
{
	this->startTime = std::chrono::high_resolution_clock::now();
}

void Player::stopTimer()
{
	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration<double>(endTime - this->startTime);
	this->timer = duration.count();
}

bool Player::addTakenStones(int count)
{
	this->takenStones += count;
	if (this->takenStones >= 10)
		return true;
	return false;
}

std::pair<int, int> Player::getAIMove(Board *board) const
{
	t_move *bestMove = new t_move{-1, -1, 0};

	minMax(board, 0, INT_MIN, INT_MAX, true, this->color, bestMove);

	std::pair<int, int> result = std::make_pair(bestMove->row, bestMove->col);
	delete bestMove;
	return result;
}
