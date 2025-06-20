/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameLogic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:06:26 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/20 13:27:08 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "GameLogic.hpp"
#include <unistd.h>

GameLogic::GameLogic() {}

GameLogic::GameLogic(s_game_config config)
{
	this->board_size = config.board_size;
	this->save_history = config.save_history;
	this->subject_mode = (config.game_mode == "subject");
	this->gameMode = config.game_mode;

	// Initialize players
	this->player1 = Player(BLACK_STONE, false);
	this->player2 = Player(WHITE_STONE, config.is_ai);
	this->currentPlayer = &this->player1;
	this->inactivePlayer = &this->player2;

	this->currentPlayer->startTimer();

	this->board = new Board(config.board_size, config);
	this->screen = new Screen(this->board_size);
	this->history = new History();
}

GameLogic::~GameLogic()
{
	delete this->screen;
	delete this->board;
	delete this->history;
}

/* Getter */

History *GameLogic::getHistory() const
{
	return this->history;
}

/* Mouse Callback */
void GameLogic::mouseButtonCallback(mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	(void)mods;

	GameLogic *game = static_cast<GameLogic *>(param);
	if (action == MLX_PRESS && button == MLX_MOUSE_BUTTON_LEFT)
	{
		int xpos, ypos;
		mlx_get_mouse_pos(game->screen->getMLX(), &xpos, &ypos);

		// Calculate the closest intersection
		int cell_size = (SCREEN_SIZE - 2 * MARGIN) / (game->board_size - 1);
		int col = std::round((xpos - MARGIN) / static_cast<double>(cell_size));
		int row = std::round((ypos - MARGIN) / static_cast<double>(cell_size));

		// Ensure the position is within bounds
		if (col < 0 || col >= game->board_size || row < 0 || row >= game->board_size)
			return;

		std::pair<int, int> cell = {col, row};
		int continueGame = game->handleTurn(cell);
		if (!continueGame)
			mlx_close_window(game->screen->getMLX());
		else if (game->player2.isAI() && game->currentPlayer == &game->player2)
		{
			std::pair<int, int> aiMove = game->player2.getAIMove(game->board);
			if (!game->handleTurn(aiMove))
				mlx_close_window(game->screen->getMLX());
		}
	}
}

/* Methods */

bool GameLogic::checkIllegalMove(const std::pair<int, int> &cell)
{
	int col = cell.first;
	int row = cell.second;

	// Check if the move is out of bounds
	if (!this->board->inBounds(col, row))
		return true;

	// Check if the cell is already occupied
	if (checkOccupiedCell(*this->board, col, row))
		return true;

	if (this->gameMode != "standard")
	{
		// Check if the move creates a double three
		if (checkDoubleThree(*this->board, cell, this->currentPlayer->getColor()))
		{
			std::cout << T_PURPLE << "Illegal move: Double three detected!" << T_GRAY << std::endl;
			return true;
		}

		// Check if the move is into a capture
		if (checkMoveIntoCapture(*this->board, cell, this->currentPlayer->getColor()))
		{
			std::cout << T_PURPLE << "Illegal move: Cannot move into capture!" << T_GRAY << std::endl;
			return true;
		}
	}

	return false;
}

bool GameLogic::checkGameEnd(const std::pair<int, int> &lastMove)
{
	int x = lastMove.first;
	int y = lastMove.second;
	if (this->board->checkWin(x, y, this->currentPlayer->getColor()))
	{
		std::cout << T_WHITE << "-----------------------------------" << std::endl;
		std::cout << T_YELLOW << "Five in a row!" << std::endl;
		std::cout << T_GREEN << this->currentPlayer->getName() << " Player wins!" << T_GRAY << std::endl;
		if (this->save_history)
			this->history->addWin(this->currentPlayer);
		return true;
	}
	else if (this->currentPlayer->getTakenStones() >= 10 && this->gameMode == "subject")
	{
		std::cout << T_WHITE << "-----------------------------------" << std::endl;
		std::cout << T_YELLOW << "The game is over due to " << this->currentPlayer->getTakenStones() << " stones taken!" << std::endl;
		std::cout << T_GREEN << this->currentPlayer->getName() << " Player wins!" << T_GRAY << std::endl;
		if (this->save_history)
			this->history->addWin(this->currentPlayer);
		return true;
	}
	else if (this->board->isFull())
	{
		std::cout << T_WHITE << "-----------------------------------" << std::endl;
		std::cout << T_YELLOW << "The game is a draw!" << T_GRAY << std::endl;
		if (this->save_history)
			this->history->addTie();
		return true;
	}
	return false;
}

bool GameLogic::applyMove(const std::pair<int, int> &cell)
{
	int col = cell.first;
	int row = cell.second;

	// ! Temp
	std::cout << T_CYAN << "Board evaluation for " << this->currentPlayer->getName() << " Player:" << std::endl;
	std::cout << staticBoardEvaluation(this->board, this->currentPlayer->getColor()) << T_GRAY << std::endl;

	// Place stone on board
	this->board->set(col, row, this->currentPlayer->getColor());

	// Draw the stone on screen
	this->screen->drawStone(col, row, this->currentPlayer->getColor() == BLACK_STONE ? BLACK : WHITE);

	this->currentPlayer->stopTimer();

	// Add move to history
	if (this->save_history)
		this->history->addMove(cell, this->currentPlayer);

	// Check for capture
	if (this->gameMode != "standard")
	{
		std::vector<std::pair<int, int>> capturedStones = checkCapture(*this->board, cell, this->currentPlayer->getColor());
		if (!capturedStones.empty())
		{
			std::cout << T_YELLOW << "Capture detected!" << T_GRAY << std::endl;
			this->handleCapture(capturedStones);

			// Add capture to history
			if (this->save_history)
				this->history->addCapture(this->currentPlayer, this->inactivePlayer);
		}
	}

	// Check for end game conditions
	if (checkGameEnd(cell))
		return false;

	// Change active player
	Player *temp = this->currentPlayer;
	this->currentPlayer = this->inactivePlayer;
	this->inactivePlayer = temp;

	this->currentPlayer->startTimer();

	// Update player info display
	this->screen->drawPlayerInfo(&this->player1, &this->player2, this->currentPlayer);

	return true;
}

bool GameLogic::handleTurn(const std::pair<int, int> &cell)
{

	if (this->checkIllegalMove(cell))
		return true;

	return this->applyMove(cell);
}

void GameLogic::handleCapture(const std::vector<std::pair<int, int>> &takenStones)
{
	this->currentPlayer->addTakenStones(takenStones.size());
	for (const std::pair<int, int> &stone : takenStones)
	{
		int col = stone.first;
		int row = stone.second;

		if (this->gameMode == "subject")
			this->board->set(col, row, EMPTY);
		else if (this->gameMode == "reversi")
			this->board->set(col, row, this->currentPlayer->getColor());
	}
	this->screen->clearArea(0, 0, SCREEN_SIZE, SCREEN_SIZE, BLACK);
	this->screen->drawBoard();
	this->screen->drawAllStones(this->board);
}

void GameLogic::startGame()
{
	// Set up the mouse callback
	mlx_mouse_hook(this->screen->getMLX(), &GameLogic::mouseButtonCallback, this);

	// Draw the initial board
	this->screen->drawBoard();

	// Draw player info
	this->screen->drawPlayerInfo(&this->player1, &this->player2, this->currentPlayer);

	// Start the main loop
	mlx_loop(this->screen->getMLX());
}
