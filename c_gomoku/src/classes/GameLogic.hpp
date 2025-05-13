/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameLogic.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:06:05 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/13 12:17:27 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMELOGIC_HPP_
#define GAMELOGIC_HPP_

#include <iostream>
#include "../include/gomoku.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "Screen.hpp"

class GameLogic
{
private:
	int board_size;
	bool save_history;
	bool subject_mode;

	Player player1;
	Player player2;
	Player *currentPlayer;
	std::string gameMode;

	Screen *screen;
	/* Board *board; */

public:
	GameLogic();
	GameLogic(s_game_config config);
	GameLogic(const GameLogic &copy);
	GameLogic &operator=(const GameLogic &assign);
	~GameLogic();

	void startGame();
};

#endif
