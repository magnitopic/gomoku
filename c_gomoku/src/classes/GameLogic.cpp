/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameLogic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:06:26 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/13 12:24:19 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "GameLogic.hpp"

GameLogic::GameLogic() {}

GameLogic::GameLogic(s_game_config config)
{
	this->board_size = config.board_size;
	this->save_history = config.save_history;
	this->subject_mode = (config.game_mode == "subject");
	this->gameMode = config.game_mode;

	this->player1 = Player(BLACK_STONE, false);
	this->player2 = Player(WHITE_STONE, config.is_ai);
	this->currentPlayer = &this->player1;

	this->screen = new Screen(this->board_size);
}

GameLogic::GameLogic(const GameLogic &copy)
{
	*this = copy;
}

GameLogic &GameLogic::operator=(const GameLogic &assign)
{
	if (this != &assign)
	{
		this->board_size = assign.board_size;
		this->save_history = assign.save_history;
		this->subject_mode = assign.subject_mode;
		this->gameMode = assign.gameMode;

		this->player1 = assign.player1;
		this->player2 = assign.player2;
		this->currentPlayer = assign.currentPlayer;
	}
	return *this;
}

GameLogic::~GameLogic()
{
	delete this->screen;
}

/* Methods */

void GameLogic::startGame()
{
	this->screen->drawBoard(&this->player1, &this->player2);
}
