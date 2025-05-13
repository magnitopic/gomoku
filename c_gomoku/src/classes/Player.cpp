/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:26:30 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/13 07:55:34 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/Player.hpp"

Player::Player() {}

Player::Player(int color, bool ai)
{
	this->color = color;
	this->name = (color == 1) ? "Black" : "White";
	this->isAI = ai;
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
		this->isAI = assign.isAI;
		this->takenStones = assign.takenStones;
		this->timer = assign.timer;
	}
	return *this;
}

Player::~Player() {}
