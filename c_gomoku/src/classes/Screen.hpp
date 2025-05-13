/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Screen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:59:10 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/13 13:56:12 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#include <iostream>
#include <iomanip>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <numbers>
#include <string>
#include <sstream>
#include "MLX42.h"
#include "../include/consts.hpp"
#include "../classes/Player.hpp"
#include "../classes/Board.hpp"

class Screen
{
private:
	mlx_t *mlx;
	static void keyCallback(mlx_key_data_t keydata, void *param);
	int board_size;
	int cell_size;

public:
	Screen();
	Screen(int board_size);
	Screen(const Screen &copy);
	Screen &operator=(const Screen &assign);
	~Screen();

	// Getters
	mlx_t *getMLX() const;

	// Draw methods
	void drawBoard(Player *player1, Player *player2);
	void drawPlayerInfo(Player *player1, Player *player2);
	void drawStone(int x, int y, int color);
	void drawAllStones(const Board *board);
	void drawWinScreen();
	void drawTieScreen();

	// Helper method to clear a rectangle area
	void clearArea(int x, int y, int width, int height, int color);
};

#endif