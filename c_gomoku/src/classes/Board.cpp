/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Board.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:11:46 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/15 13:04:47 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Board.hpp"

Board::Board()
{
	this->size = 0;
	this->board.clear();
}

// Constructors
Board::Board(int board_size, s_game_config config)
{
	this->size = board_size;
	this->board.resize(this->size * this->size, EMPTY);
	this->gameMode = config.game_mode;
	this->difficulty = config.difficulty;
}

Board::Board(const Board &copy)
{
	*this = copy;
}

Board &Board::operator=(const Board &assign)
{
	if (this != &assign)
	{
		this->board = assign.board;
		this->size = assign.size;
	}
	return *this;
}

Board::~Board() {}

/* Basic operators */

int Board::getSize() const
{
	return this->size;
}

std::string Board::getGameMode() const
{
	return this->gameMode;
}

int Board::get(int x, int y) const
{
	if (Board::inBounds(x, y))
		return this->board[y * this->size + x];
	return -2;
}

void Board::set(int x, int y, int value)
{
	if (Board::inBounds(x, y))
		this->board[y * this->size + x] = value;
}

bool Board::isEmpty(int x, int y) const
{
	return (get(x, y) == EMPTY);
}

bool Board::isEmpty() const
{
	int sum = std::accumulate(this->board.begin(), this->board.end(), 0);
	return (sum == 0);
}

bool Board::isFull() const
{
	for (int i = 0; i < this->size * this->size; ++i)
	{
		if (this->board[i] == EMPTY)
			return false;
	}
	return true;
}

/* Methods */

bool Board::inBounds(int x, int y) const
{
	return (x >= 0 && x < this->size && y >= 0 && y < this->size);
}

std::vector<std::pair<int, int>> Board::getOccupiedTiles() const
{
	std::vector<std::pair<int, int>> occupiedTiles;

	// Reserve space to avoid reallocations (worst case: all tiles occupied)
	occupiedTiles.reserve(this->size * this->size);

	for (int y = 0; y < this->size; ++y)
	{
		for (int x = 0; x < this->size; ++x)
		{
			if (!isEmpty(x, y))
				occupiedTiles.push_back({x, y});
		}
	}

	return occupiedTiles;
}

bool Board::checkNInARow(const std::pair<int, int> &lastMove, int color, const std::pair<int, int> &direction, int n) const
{
	int count = 1;
	int x = lastMove.first;
	int y = lastMove.second;
	int dx = direction.first;
	int dy = direction.second;

	// Check in positive direction
	for (int i = 1; i < n; i++)
	{
		int newX = x + dx * i;
		int newY = y + dy * i;
		if (!Board::inBounds(newX, newY) || get(newX, newY) != color)
			break;
		count++;
	}

	// Check in negative direction
	for (int i = 1; i < n; i++)
	{
		int newX = x - dx * i;
		int newY = y - dy * i;
		if (!Board::inBounds(newX, newY) || get(newX, newY) != color)
			break;
		count++;
	}

	return count >= n;
}

bool Board::checkWin(int x, int y, int player) const
{
	for (const auto &direction : DIRECTIONS)
	{
		if (checkNInARow({x, y}, player, direction, 5))
			return true;
	}
	return false;
}

std::vector<std::pair<int, int>> Board::checkCapture(const std::pair<int, int> &lastMove, int color) const
{
	int x = lastMove.first;
	int y = lastMove.second;
	std::vector<std::pair<int, int>> stonesToDelete;

	for (const auto &direction : DIRECTIONS)
	{
		int dx = direction.first;
		int dy = direction.second;
		std::vector<std::pair<int, int>> tempStonesToDelete;

		// Check in positive direction
		for (int i = 1; i <= 3; i++)
		{
			int newX = x + dx * i;
			int newY = y + dy * i;
			if (!Board::inBounds(newX, newY))
				break;

			if (i <= 2 && get(newX, newY) == -color)
				tempStonesToDelete.push_back({newX, newY});
			else if (i == 3 && get(newX, newY) == color)
			{
				stonesToDelete.insert(stonesToDelete.end(), tempStonesToDelete.begin(), tempStonesToDelete.end());
				break;
			}
			else
				break;
		}

		tempStonesToDelete.clear();

		// Check in negative direction
		for (int i = 1; i <= 3; i++)
		{
			int newX = x - dx * i;
			int newY = y - dy * i;
			if (!Board::inBounds(newX, newY))
				break;

			if (i <= 2 && get(newX, newY) == -color)
				tempStonesToDelete.push_back({newX, newY});
			else if (i == 3 && get(newX, newY) == color)
			{
				stonesToDelete.insert(stonesToDelete.end(), tempStonesToDelete.begin(), tempStonesToDelete.end());
				break;
			}
			else
				break;
		}
	}

	return stonesToDelete;
}
