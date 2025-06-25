/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Board.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:11:46 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/23 21:47:04 by alaparic         ###   ########.fr       */
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

std::string Board::getDifficulty() const
{
	return this->difficulty;
}

Player *Board::getPlayer1() const
{
	return this->player1;
}

Player *Board::getPlayer2() const
{
	return this->player2;
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

std::vector<int> Board::getLine(const std::pair<int, int> &direction, int lineIndex) const
{
	std::vector<int> stones;
	int dx = direction.first;
	int dy = direction.second;

	if (dx == 1 && dy == 0) // Horizontal lines (rows)
	{
		if (lineIndex < 0 || lineIndex >= this->size)
			return stones;

		for (int x = 0; x < this->size; x++)
			stones.push_back(get(x, lineIndex));
	}
	else if (dx == 0 && dy == 1) // Vertical lines (columns)
	{
		if (lineIndex < 0 || lineIndex >= this->size)
			return stones;

		for (int y = 0; y < this->size; y++)
			stones.push_back(get(lineIndex, y));
	}
	else if (dx == 1 && dy == 1) // Main diagonals (top-left to bottom-right)
	{
		if (lineIndex < 0 || lineIndex >= 2 * this->size - 1)
			return stones;

		int startX, startY;
		if (lineIndex < this->size)
		{
			startX = 0;
			startY = lineIndex;
		}
		else
		{
			startX = lineIndex - this->size + 1;
			startY = 0;
		}

		int x = startX;
		int y = startY;
		while (inBounds(x, y))
		{
			stones.push_back(get(x, y));
			x += dx;
			y += dy;
		}
	}
	else if (dx == 1 && dy == -1) // Anti-diagonals (top-right to bottom-left)
	{
		if (lineIndex < 0 || lineIndex >= 2 * this->size - 1)
			return stones;

		int startX, startY;
		if (lineIndex < this->size)
		{
			startX = 0;
			startY = this->size - 1 - lineIndex;
		}
		else
		{
			startX = lineIndex - this->size + 1;
			startY = this->size - 1;
		}

		int x = startX;
		int y = startY;
		while (inBounds(x, y))
		{
			stones.push_back(get(x, y));
			x += dx;
			y += dy;
		}
	}
	// if no stones return empty vector
	if (std::all_of(stones.begin(), stones.end(), [](int stone)
					{ return stone == 0; }))
		return std::vector<int>();

	return stones;
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
