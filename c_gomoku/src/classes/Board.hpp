/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Board.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:11:35 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/15 13:02:33 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <iostream>
#include <vector>
#include <numeric>
#include "../include/gomoku.hpp"

class Board
{
private:
	std::vector<int> board;
	int size;
	std::string gameMode;
	std::string difficulty;

public:
	Board();
	Board(int board_size, s_game_config config);
	Board(const Board &copy);
	Board &operator=(const Board &assign);
	~Board();

	int getSize() const;
	std::string getGameMode() const;
	int get(int x, int y) const;
	void set(int x, int y, int value);
	bool isEmpty(int x, int y) const;
	bool isEmpty() const;
	bool isFull() const;

	bool inBounds(int x, int y) const;
	std::vector<std::pair<int, int>> getOccupiedTiles() const;
	bool checkNInARow(const std::pair<int, int> &lastMove, int color, const std::pair<int, int> &direction, int n) const;
	bool checkWin(int x, int y, int player) const;
	std::vector<std::pair<int, int>> checkCapture(const std::pair<int, int> &lastMove, int color) const;
};

#endif
