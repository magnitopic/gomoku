/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Screen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:59:10 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/12 13:48:24 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "MLX42.h"
#include "../include/consts.hpp"

class Screen
{
private:
	mlx_t *mlx;
	static void keyCallback(mlx_key_data_t keydata, void *param);

public:
	Screen();
	Screen(const Screen &copy);
	Screen &operator=(const Screen &assign);
	~Screen();

	void drawBoard();
	void drawPlayerInfo();
	void drawStone(int x, int y, int color);
	void drawAllStones();
	void drawWinScreen();
	void drawTieScreen();
};

#endif