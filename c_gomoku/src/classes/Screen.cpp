/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Screen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:59:12 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/13 09:56:13 by alaparic         ###   ########.fr       */
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

Screen::Screen(int board_size)
{
	this->board_size = board_size;
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

/* Methods */

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

	// Draw border around the board
	int border_start_x = MARGIN - BORDER_MARGIN;
	int border_start_y = MARGIN - BORDER_MARGIN;
	int border_width = (this->board_size - 1) * (SCREEN_SIZE - 2 * MARGIN) / (this->board_size - 1) + 2 * BORDER_MARGIN;
	int border_height = border_width;

	// Draw border (rectangle outline)
	for (int i = 0; i < border_width; ++i)
	{
		mlx_put_pixel(img, border_start_x + i, border_start_y, BORDER_COLOR);
		mlx_put_pixel(img, border_start_x + i, border_start_y + border_height, BORDER_COLOR);
	}
	for (int i = 0; i < border_height; ++i)
	{
		mlx_put_pixel(img, border_start_x, border_start_y + i, BORDER_COLOR);
		mlx_put_pixel(img, border_start_x + border_width, border_start_y + i, BORDER_COLOR);
	}

	// Calculate cell size for proper grid spacing
	int cell_size = (SCREEN_SIZE - 2 * MARGIN) / (this->board_size - 1);

	// Draw grid lines
	for (int i = 0; i < this->board_size; ++i)
	{
		// Vertical lines
		for (int y = MARGIN; y < MARGIN + (this->board_size - 1) * cell_size; ++y)
		{
			mlx_put_pixel(img, MARGIN + i * cell_size, y, BLACK);
		}

		// Horizontal lines
		for (int x = MARGIN; x < MARGIN + (this->board_size - 1) * cell_size; ++x)
		{
			mlx_put_pixel(img, x, MARGIN + i * cell_size, BLACK);
		}
	}

	// Draw star points (similar to the Python implementation)
	std::vector<int> star_points;
	if (this->board_size == 15)
	{
		star_points = {3, 7, 11};
	}
	else
	{
		star_points = {3, 9, 15};
	}

	// Only draw star points if they're within the board size
	for (int x : star_points)
	{
		if (x >= this->board_size)
			continue;
		for (int y : star_points)
		{
			if (y >= this->board_size)
				continue;

			// Draw a small filled circle at star points
			int center_x = MARGIN + x * cell_size;
			int center_y = MARGIN + y * cell_size;
			int radius = 4;

			// Simple circle drawing
			for (int dx = -radius; dx <= radius; ++dx)
			{
				for (int dy = -radius; dy <= radius; ++dy)
				{
					if (dx * dx + dy * dy <= radius * radius)
					{
						mlx_put_pixel(img, center_x + dx, center_y + dy, BLACK);
					}
				}
			}
		}
	}

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