/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AI.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:01:46 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/13 14:13:47 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AI_HPP_
#define AI_HPP_

#include <iostream>
#include "ai_config.hpp"


class AI
{
public:
	AI();
	AI(const AI &copy);
	AI &operator=(const AI &assign);
	~AI();

	std::pair<int, int> getMove(Board *board, int player);

private:
	int evaluateBoard(Board *board, int player);
	std::pair<int, int> getBestMove(Board *board, int player);
};

#endif
