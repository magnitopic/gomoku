/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:26:43 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/28 18:17:13 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <string>
#include <chrono>
#include <iomanip>
#include "../ai/ai_config.hpp"

class Player
{

private:
	int color; // 1 for black, -1 for white
	std::string name;
	bool is_ai;
	int takenStones;
	double timer;
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	double totalTime;
	int playsCount = 0;

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
	void printTimeAverage();
};

#endif
