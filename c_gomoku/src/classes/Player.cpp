/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:26:30 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/13 14:24:03 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/Player.hpp"

Player::Player() {}

Player::Player(int color, bool ai)
{
	this->color = color;
	this->name = (color == 1) ? "Black" : "White";
	this->is_ai = ai;
	this->takenStones = 0;
	this->timer = 0;
	this->ai = new AI();
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

Player::~Player()
{
	delete this->ai;
}

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

#include <iostream>

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
	{
		return true;
	}
	return false;
}

std::pair<int, int> Player::getAIMove(Board *board) const
{
	std::pair<int, int> bestMove = this->ai->getMove(board, this->color);
	std::cout << "AI Move: (" << bestMove.first << ", " << bestMove.second << ")" << std::endl;
	return bestMove;
}
