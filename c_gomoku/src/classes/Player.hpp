/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:26:43 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/12 13:49:09 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <string>

class Player
{

private:
	int color; // 1 for black, -1 for white
	std::string name;
	bool isAI;
	int takenStones;
	int timer;

	Player();

public:
	Player(int color, bool ai);
	Player(const Player &copy);
	Player &operator=(const Player &assign);
	~Player();
};

#endif
