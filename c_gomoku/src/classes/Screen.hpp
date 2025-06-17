/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Screen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:59:10 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/17 12:34:47 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#include <iomanip>
#include <sstream>
#include "../../MLX42/include/MLX42/MLX42.h"
#include "../include/Consts.hpp"
#include "../classes/Player.hpp"
#include "../classes/Board.hpp"

class Screen
{
private:
	mlx_t *mlx;
	static void keyCallback(mlx_key_data_t keydata, void *param);
	int board_size;
	int cell_size;

	void drawCircle(mlx_image_t *img, int centerX, int centerY, int radius, int color);

public:
	Screen();
	Screen(int board_size);
	Screen(const Screen &copy);
	Screen &operator=(const Screen &assign);
	~Screen();

	// Getters
	mlx_t *getMLX() const;

	// Draw methods
	void drawBoard();
	void drawPlayerInfo(Player *player1, Player *player2, Player *currentPlayer);
	void drawStone(int x, int y, int color);
	void drawAllStones(const Board *board);

	// Helper method to clear a rectangle area
	void clearArea(int x, int y, int width, int height, int color);
};

#endif