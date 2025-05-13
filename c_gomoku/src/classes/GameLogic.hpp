/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameLogic.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:06:05 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/13 13:08:06 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMELOGIC_HPP_
#define GAMELOGIC_HPP_

#include <iostream>
#include <chrono>
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

	Board *board;
	Player player1;
	Player player2;
	Player *currentPlayer;
	Player *inactivePlayer;
	std::string gameMode;

	Screen *screen;

	// Mouse event handling
	static void mouseButtonCallback(mouse_key_t button, action_t action, modifier_key_t mods, void *param);

	bool applyMove(const std::pair<int, int> &cell);

public:
	GameLogic();
	GameLogic(s_game_config config);
	GameLogic(const GameLogic &copy);
	GameLogic &operator=(const GameLogic &assign);
	~GameLogic();

	bool handleTurn(const std::pair<int, int> &cell);
	void startGame();
};

#endif