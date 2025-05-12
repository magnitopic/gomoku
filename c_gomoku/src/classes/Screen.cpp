/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Screen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:59:12 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/12 13:48:45 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Screen.hpp"

Screen::Screen()
{
	// Initialize MLX with settings
	mlx_set_setting(MLX_MAXIMIZED, false);
	this->mlx = mlx_init(SCREEN_SIZE, SCREEN_SIZE, "Gomoku", false);
	if (!this->mlx)
	{
		fprintf(stderr, "%s", mlx_strerror(mlx_errno));
		exit(EXIT_FAILURE);
	}

	// Set up key hooks
	mlx_key_hook(this->mlx, &Screen::keyCallback, this);
}

Screen::Screen(const Screen &copy)
{
	*this = copy;
}

Screen &Screen::operator=(const Screen &assign)
{
	if (this != &assign)
	{
		this->mlx = assign.mlx;
	}
	return *this;
}

Screen::~Screen()
{
	if (this->mlx)
		mlx_terminate(this->mlx);
}

void Screen::drawBoard()
{
	mlx_image_t *img = mlx_new_image(this->mlx, SCREEN_SIZE, SCREEN_SIZE);
	if (!img)
	{
		fprintf(stderr, "Failed to create image\n");
		return;
	}

	for (uint32_t i = 0; i < img->width * img->height; ++i)
		((uint32_t *)img->pixels)[i] = WOOD;

	// Put the image on screen
	mlx_image_to_window(this->mlx, img, 0, 0);

	// Start the main loop
	mlx_loop(this->mlx);
}

void Screen::keyCallback(mlx_key_data_t keydata, void *param)
{
	Screen *screen = static_cast<Screen *>(param);

	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		std::cout << "Exiting..." << std::endl;
		mlx_close_window(screen->mlx);
	}
}