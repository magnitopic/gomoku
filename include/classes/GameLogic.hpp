/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameLogic.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:06:05 by alaparic          #+#    #+#             */
/*   Updated: 2025/10/06 12:47:13 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMELOGIC_HPP_
#define GAMELOGIC_HPP_

#include <iostream>
#include <chrono>
#include "../gomoku.hpp"
#include "../ai/AI.hpp"
#include "./Board.hpp"
#include "./Player.hpp"
#include "./Screen.hpp"
#include "./History.hpp"

class GameLogic
{
private:
	int board_size;
	bool save_history;
	bool subject_mode;
	bool multiplayer_game;
	bool prevCaptureCancel = false;

	Board *board;
	AI ai;
	Player player1;
	Player player2;
	Player *currentPlayer;
	Player *inactivePlayer;
	std::string gameMode;

	Screen *screen;
	History *history;

	// Mouse event handling
	static void mouseButtonCallback(mouse_key_t button, action_t action, modifier_key_t mods, void *param);
	// Key event handling
	static void keyPressCallback(mlx_key_data_t keydata, void *param);

	// TODO-> move checks to board_checks.cpp
	bool applyMove(const std::pair<int, int> &cell);
	bool checkIllegalMove(const std::pair<int, int> &cell);
	void handleCapture(const std::vector<std::pair<int, int>> &takenStones);
	bool checkGameEnd(const std::pair<int, int> &lastMove);

public:
	GameLogic();
	GameLogic(s_game_config config);
	~GameLogic();

	// Getters
	History *getHistory() const;

	bool handleTurn(const std::pair<int, int> &cell);
	void startGame();
};

#endif