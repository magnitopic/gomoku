/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:26:43 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/13 14:22:18 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <string>
#include <chrono>
#include "../ai/AI.hpp"

class Player
{

private:
	int color; // 1 for black, -1 for white
	std::string name;
	bool is_ai;
	AI *ai;
	int takenStones;
	double timer;
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

public:
	Player();
	Player(int color, bool ai);
	Player(const Player &copy);
	Player &operator=(const Player &assign);
	~Player();

	// Getters
	int getColor() const;
	std::string getName() const;
	int getTakenStones() const;
	double getTimer() const;
	bool isAI() const;

	// Methods
	void startTimer();
	void stopTimer();
	bool addTakenStones(int count);
	std::pair<int, int> getAIMove(Board *board) const;
};

#endif
