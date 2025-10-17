/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:11:30 by adiaz-uf          #+#    #+#             */
/*   Updated: 2025/09/14 17:29:32 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "../include/gomoku.hpp"

struct ToLower
{
	unsigned char operator()(unsigned char c) const
	{
		return std::tolower(c);
	}
};

static std::string config_input()
{
	std::string value;

	std::cout << T_RESET << ">> ";

	if (!std::getline(std::cin, value))
	{
		if (std::cin.eof())
			std::exit(0);
		std::cin.clear();
		return "";
	}

	// Check if input is empty (default value)
	if (value.empty())
		std::cout << "Default value" << std::endl;

	// Trim whitespace
	value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
	value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);

	// Convert to lowercase
	std::transform(value.begin(), value.end(), value.begin(), ToLower());

	return value;
}

bool isValidBoardSize(const std::string &size)
{
	return (size == "13" || size == "15" || size == "19");
}

bool isValidDifficulty(const std::string &difficulty)
{
	return (difficulty == "easy" || difficulty == "middle" || difficulty == "standard");
}

bool isValidGameMode(const std::string &mode)
{
	return (mode == "standard" || mode == "subject" || mode == "reversi");
}

static bool isValidYesNo(const std::string &response)
{
	return (response == "y" || response == "n" || response.empty());
}

s_game_config initial_game_config()
{
	s_game_config config;

	config.is_ai = true;
	config.difficulty = "standard";
	config.board_size = 19;
	config.save_history = false;
	config.game_mode = "subject";

	std::cout << T_CYAN << "-- Welcome to GOMOKU ⚫️⚪️ --" << std::endl
			  << std::endl;
	std::cout << T_YELLOW << "Please select your game configuration:" << std::endl
			  << std::endl;

	// VS AI or VS Player
	std::cout << T_BLUE << "1. Would you like to play against the AI? (" << T_GREEN << "Y" << T_BLUE "/n)" << std::endl;

	std::string ai_choice = config_input();

	if (!isValidYesNo(ai_choice))
	{
		std::cout << T_RED << "Invalid choice. Defaulting to AI mode." << std::endl;
		config.is_ai = true;
	}
	else if (ai_choice == "n")
	{
		config.is_ai = false;
		config.difficulty = "";
	}

	// AI Difficulty - only if AI mode is enabled
	if (config.is_ai)
	{
		std::cout << "\n"
				  << T_BLUE << "2. Choose AI difficulty: (easy/middle/"
				  << T_GREEN << "standard" << T_BLUE << ")" << std::endl;
		std::string ai_difficulty = config_input();

		if (ai_difficulty.empty())
		{
			config.difficulty = "standard";
		}
		else if (isValidDifficulty(ai_difficulty))
		{
			config.difficulty = ai_difficulty;
		}
		else
		{
			std::cout << T_RED << "Invalid choice. Defaulting to standard difficulty." << std::endl;
			config.difficulty = "standard";
		}
	}

	// Board Size
	std::cout << "\n"
			  << T_BLUE << "3. Choose board size: (13/15/"
			  << T_GREEN << "19" << T_BLUE << ")" << std::endl;
	std::string board_size = config_input();

	if (board_size.empty())
	{
		config.board_size = 19;
	}
	else if (isValidBoardSize(board_size))
	{
		config.board_size = std::stoi(board_size);
	}
	else
	{
		std::cout << T_RED << "Invalid choice. Defaulting to 19x19." << std::endl;
		config.board_size = 19;
	}

	// Save Game History
	std::cout << "\n"
			  << T_BLUE << "4. Would you like to save game history? (y/"
			  << T_GREEN << "N" << T_BLUE << ")" << std::endl;
	std::string save_history = config_input();

	if (save_history == "y")
	{
		config.save_history = true;
	}
	else if (save_history.empty() || save_history == "n")
	{
		config.save_history = false;
	}
	else
	{
		std::cout << T_RED << "Invalid choice. Defaulting to not saving history." << std::endl;
		config.save_history = false;
	}

	// Game Mode
	std::cout << "\n"
			  << T_BLUE << "5. Choose game mode: (standard/reversi/"
			  << T_GREEN << "subject" << T_BLUE << ")" << std::endl;
	std::cout << "\t-" << T_CYAN << " Standard: " << T_BLUE << " Classic Gomoku rules with no restrictions." << std::endl;
	std::cout << "\t-" << T_CYAN << " Reversi: " << T_BLUE << " Captured stones turn to your colour." << std::endl;
	std::cout << "\t-" << T_CYAN << " Subject: " << T_BLUE << " Gomoku with subject mode rules (captures, no double three)." << std::endl;
	std::string game_mode = config_input();

	if (game_mode.empty())
	{
		config.game_mode = "subject";
	}
	else if (isValidGameMode(game_mode))
	{
		config.game_mode = game_mode;
	}
	else
	{
		std::cout << T_RED << "Invalid choice. Defaulting to subject mode." << std::endl;
		config.game_mode = "subject";
	}

	std::cout << "\n"
			  << T_GREEN << "Game configuration complete!" << T_RESET << std::endl;
	std::cout << T_BLUE << "Starting the game...\n"
			  << std::endl;

	std::cout << T_WHITE << " --------------------" << T_GRAY << std::endl;

	return config;
}
