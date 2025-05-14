/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Screen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:59:12 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/14 10:49:30 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Screen.hpp"
#include "Player.hpp"

Screen::Screen() {}

Screen::Screen(int board_size)
{
	this->board_size = board_size;
	this->cell_size = (SCREEN_SIZE - 2 * MARGIN) / (this->board_size - 1);
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

void Screen::drawCircle(mlx_image_t *img, int centerX, int centerY, int radius, int color)
{
	for (int dx = -radius; dx <= radius; ++dx)
	{
		for (int dy = -radius; dy <= radius; ++dy)
		{
			if (dx * dx + dy * dy <= radius * radius)
			{
				uint32_t px = centerX + dx;
				uint32_t py = centerY + dy;

				if (px >= 0 && px < img->width && py >= 0 && py < img->height)
					((uint32_t *)img->pixels)[py * img->width + px] = color;
			}
		}
	}
}

/* Getters */

mlx_t *Screen::getMLX() const
{
	return this->mlx;
}

/* Callback */

void Screen::keyCallback(mlx_key_data_t keydata, void *param)
{
	Screen *screen = static_cast<Screen *>(param);

	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		std::cout << T_RESET << "Exiting..." << std::endl;
		mlx_close_window(screen->mlx);
	}
}

/* Methods */

void Screen::drawBoard(Player *player1, Player *player2)
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

	for (int i = 0; i < this->board_size; ++i)
	{
		for (int y = MARGIN; y < MARGIN + (this->board_size - 1) * cell_size; ++y)
			mlx_put_pixel(img, MARGIN + i * cell_size, y, BLACK);

		for (int x = MARGIN; x < MARGIN + (this->board_size - 1) * cell_size; ++x)
			mlx_put_pixel(img, x, MARGIN + i * cell_size, BLACK);
	}

	// Draw star points
	std::vector<int> star_points;
	if (this->board_size == 15)
		star_points = {3, 7, 11};
	else
		star_points = {3, 9, 15};

	// Only draw star points if they're within the board size
	for (int x : star_points)
	{
		if (x >= this->board_size)
			continue;
		for (int y : star_points)
		{
			if (y >= this->board_size)
				continue;

			int center_x = MARGIN + x * cell_size;
			int center_y = MARGIN + y * cell_size;
			drawCircle(img, center_x, center_y, 4, BLACK);
		}
	}

	mlx_image_to_window(this->mlx, img, 0, 0);

	// draw player info on top of the board
	Screen::drawPlayerInfo(player1, player2);

	// Start the main loop
	mlx_loop(this->mlx);
}

void Screen::drawPlayerInfo(Player *player1, Player *player2)
{
	// Clear previous info areas
	this->clearArea(10, 10, SCREEN_SIZE - 20, 30, WOOD);
	this->clearArea(10, SCREEN_SIZE - 40, SCREEN_SIZE - 20, 30, WOOD);

	// Player 1 info
	std::string player1Info = "Player 1: Black";
	std::stringstream p1Timer;
	double p1Time = player1->getTimer();
	int seconds = static_cast<int>(p1Time);
	int milliseconds = static_cast<int>((p1Time - seconds) * 1000);
	p1Timer << "Time: " << seconds << "." << std::setw(3) << std::setfill('0') << milliseconds << "s";
	std::string p1TimerStr = p1Timer.str();
	std::string p1Taken = "Taken: " + std::to_string(player1->getTakenStones());

	// Player 2 info
	std::string player2Info = "Player 2: White";
	std::stringstream p2Timer;
	double p2Time = player2->getTimer();
	seconds = static_cast<int>(p2Time);
	milliseconds = static_cast<int>((p2Time - seconds) * 1000);
	p2Timer << "Time: " << seconds << "." << std::setw(3) << std::setfill('0') << milliseconds << "s";
	std::string p2TimerStr = p2Timer.str();
	std::string p2Taken = "Taken: " + std::to_string(player2->getTakenStones());

	// Display player info
	mlx_put_string(this->mlx, player1Info.c_str(), 10, 10);
	mlx_put_string(this->mlx, p1TimerStr.c_str(), SCREEN_SIZE - 140, 10);
	mlx_put_string(this->mlx, p1Taken.c_str(), SCREEN_SIZE - 250, 10);

	mlx_put_string(this->mlx, player2Info.c_str(), 10, SCREEN_SIZE - 40);
	mlx_put_string(this->mlx, p2TimerStr.c_str(), SCREEN_SIZE - 140, SCREEN_SIZE - 40);
	mlx_put_string(this->mlx, p2Taken.c_str(), SCREEN_SIZE - 250, SCREEN_SIZE - 40);
}

void Screen::drawStone(int x, int y, int color)
{
	int stoneSize = this->cell_size - 4;
	mlx_image_t *stoneImg = mlx_new_image(this->mlx, stoneSize, stoneSize);
	if (!stoneImg)
	{
		fprintf(stderr, "Failed to create stone image\n");
		return;
	}

	int centerX = stoneSize / 2;
	int centerY = stoneSize / 2;
	int radius = stoneSize / 2;

	for (uint32_t i = 0; i < stoneImg->width * stoneImg->height; ++i)
		((uint32_t *)stoneImg->pixels)[i] = 0;

	drawCircle(stoneImg, centerX, centerY, radius, color);

	int boardX = MARGIN + x * this->cell_size;
	int boardY = MARGIN + y * this->cell_size;
	mlx_image_to_window(this->mlx, stoneImg, boardX - radius, boardY - radius);
}

void Screen::drawAllStones(const Board *board)
{
	for (int y = 0; y < this->board_size; ++y)
	{
		for (int x = 0; x < this->board_size; ++x)
		{
			int cell_value = board->get(x, y);
			if (cell_value != EMPTY)
			{
				this->drawStone(x, y, (cell_value == BLACK_STONE) ? BLACK : WHITE);
			}
		}
	}
}

/* Helper method */
void Screen::clearArea(int x, int y, int width, int height, int color)
{
	mlx_image_t *img = mlx_new_image(this->mlx, width, height);
	if (!img)
	{
		fprintf(stderr, "Failed to create image for clearing area\n");
		return;
	}

	// Fill with specified color
	for (uint32_t i = 0; i < img->width * img->height; ++i)
	{
		((uint32_t *)img->pixels)[i] = color;
	}

	mlx_image_to_window(this->mlx, img, x, y);
}
