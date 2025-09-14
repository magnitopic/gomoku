/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   History.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:21:15 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/14 17:29:32 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include "../../include/classes/History.hpp"

History::History() {}

History::History(const History &copy)
{
	*this = copy;
}

History &History::operator=(const History &assign)
{
	if (this != &assign)
	{
		this->events = assign.events;
	}
	return *this;
}

History::~History() {}

/* Methods */

void History::addMove(const std::pair<int, int> &move, Player *player)
{
	HistoryEvent event;
	event.type = MOVE;
	event.timestamp = std::time(nullptr);

	std::stringstream ss;
	ss << player->getName() << " Player placed a stone at position ("
	   << move.first << ", " << move.second << ")";
	event.message = ss.str();

	this->events.push_back(event);
}

void History::addCapture(Player *current, Player *inactive)
{
	HistoryEvent event;
	event.type = CAPTURE;
	event.timestamp = std::time(nullptr);

	std::string playerName = current->getName();
	std::string opponentName = inactive->getName();
	std::stringstream ss;
	ss << playerName << " Player captured stones from " << opponentName << " Player (" << current->getTakenStones() << " in total)";
	event.message = ss.str();

	this->events.push_back(event);
}

void History::addWin(Player *player)
{
	HistoryEvent event;
	event.type = WIN;
	event.timestamp = std::time(nullptr);

	std::stringstream ss;
	ss << player->getName() << " Player won the game!";
	event.message = ss.str();

	this->events.push_back(event);
}

void History::addTie()
{
	HistoryEvent event;
	event.type = TIE;
	event.timestamp = std::time(nullptr);

	event.message = "The game ended in a tie!";

	this->events.push_back(event);
}

void History::createHistoryFile(const s_game_config &config)
{
	if (this->events.empty())
	{
		std::cout << T_YELLOW << "No events to save in history" << T_RESET << std::endl;
		return;
	}

	std::ofstream historyFile("game_history.txt");
	if (!historyFile.is_open())
	{
		std::cerr << T_RED << "Error: Could not create history file" << T_RESET << std::endl;
		return;
	}

	// Write game configuration
	historyFile << "===== GOMOKU GAME HISTORY =====" << std::endl;
	historyFile << "Board Size: " << config.board_size << "x" << config.board_size << std::endl;
	historyFile << "Game Mode: " << config.game_mode << std::endl;
	historyFile << "AI Opponent: " << (config.is_ai ? "Yes" : "No") << std::endl;
	if (config.is_ai)
		historyFile << "AI Difficulty: " << config.difficulty << std::endl;
	historyFile << std::endl;
	historyFile << "===== GAME EVENTS =====" << std::endl;

	for (const auto &event : this->events)
	{
		// Format timestamp
		auto time = event.timestamp;
		historyFile << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] ";

		historyFile << event.message << std::endl;
	}

	historyFile.close();
	std::cout << T_GREEN << "Game history saved!" << T_RESET << std::endl;
}
