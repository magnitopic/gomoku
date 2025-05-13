/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameLogic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:06:26 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/13 13:31:00 by alaparic         ###   ########.fr       */
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

	// Initialize players
	this->player1 = Player(BLACK_STONE, false);
	this->player2 = Player(WHITE_STONE, config.is_ai);
	this->currentPlayer = &this->player1;
	this->inactivePlayer = &this->player2;

	this->board = new Board(config.board_size);
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

		this->board = new Board(assign.board_size);
		this->screen = new Screen(assign.board_size);
		this->player1 = assign.player1;
		this->player2 = assign.player2;
		this->currentPlayer = (assign.currentPlayer == &assign.player1) ? &this->player1 : &this->player2;
		this->inactivePlayer = (assign.inactivePlayer == &assign.player1) ? &this->player1 : &this->player2;
	}
	return *this;
}

GameLogic::~GameLogic()
{
	delete this->screen;
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
		int col = round((xpos - MARGIN) / static_cast<double>(cell_size));
		int row = round((ypos - MARGIN) / static_cast<double>(cell_size));

		// Ensure the position is within bounds
		if (col < 0 || col >= game->board_size || row < 0 || row >= game->board_size)
			return;

		std::pair<int, int> cell = {col, row};
		game->handleTurn(cell);
	}
}

/* Methods */

bool GameLogic::applyMove(const std::pair<int, int> &cell)
{
	int col = cell.first;
	int row = cell.second;

	// Place stone on board
	this->board->set(col, row, this->currentPlayer->getColor());

	// Draw the stone on screen
	this->screen->drawStone(col, row, this->currentPlayer->getColor() == BLACK_STONE ? BLACK : WHITE);

	// Change active player
	Player *temp = this->currentPlayer;
	this->currentPlayer = this->inactivePlayer;
	this->inactivePlayer = temp;

	// Update player info display
	this->screen->drawPlayerInfo(&this->player1, &this->player2);

	return true;
}

bool GameLogic::handleTurn(const std::pair<int, int> &cell)
{

	/* if (this->checkIllegalMove(cell))
		return true; */

	return this->applyMove(cell);
}

void GameLogic::startGame()
{
	// Set up the mouse callback
	mlx_mouse_hook(this->screen->getMLX(), &GameLogic::mouseButtonCallback, this);

	// Draw the initial board
	this->screen->drawBoard(&this->player1, &this->player2);

	// Start the main loop
	mlx_loop(this->screen->getMLX());
}
