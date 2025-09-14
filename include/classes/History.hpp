/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   History.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:19:32 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/14 17:29:32 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_HPP_
#define HISTORY_HPP_

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include "../gomoku.hpp"
#include "Player.hpp"

// Forward declaration to avoid circular dependency
class Player;

enum EventType
{
	MOVE,
	CAPTURE,
	WIN,
	TIE
};

struct HistoryEvent
{
	EventType type;
	std::time_t timestamp;
	std::string message;
};

class History
{
private:
	std::vector<HistoryEvent> events;

public:
	History();
	History(const History &copy);
	History &operator=(const History &assign);
	~History();

	void addMove(const std::pair<int, int> &move, Player *player);
	void addCapture(Player *current, Player *inactive);
	void addWin(Player *player);
	void addTie();
	void createHistoryFile(const s_game_config &config);
};

#endif
