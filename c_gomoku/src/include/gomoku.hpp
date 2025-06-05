/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gomoku.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:36:16 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/05 11:33:06 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GOMOKU_HPP_
#define GOMOKU_HPP_

#include <iostream>
#include <string>

#include "Consts.hpp"
#include "structs.hpp"

class Board;

s_game_config initial_game_config();

/* Board checks */
bool checkOccupiedCell(const Board &board, int x, int y);
bool checkDoubleThree(const Board &board, const std::pair<int, int> &lastMove, int color);

#endif
