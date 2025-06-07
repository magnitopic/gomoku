/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gomoku.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 08:24:58 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/07 13:14:40 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/gomoku.hpp"
#include "classes/GameLogic.hpp"

int main(void)
{
	s_game_config config = initial_game_config();

	GameLogic game(config);
	game.startGame();

	if (config.save_history)
		game.getHistory()->createHistoryFile(config);
	return (EXIT_SUCCESS);
}
