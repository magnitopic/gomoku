/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:26:30 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/13 12:16:27 by alaparic         ###   ########.fr       */
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

int Player::getTimer() const
{
	return this->timer;
}
