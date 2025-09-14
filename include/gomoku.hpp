/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gomoku.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:36:16 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/14 17:29:32 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GOMOKU_HPP_
#define GOMOKU_HPP_

#include <iostream>
#include <string>

#include "consts.hpp"
#include "structs.hpp"
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>

class Board;

s_game_config initial_game_config();

/* Board checks */
bool checkOccupiedCell(const Board &board, int x, int y);
bool checkDoubleThree(Board &board, const std::pair<int, int> &lastMove, int color);
bool checkMoveIntoCapture(Board &board, const std::pair<int, int> &lastMove, int color);
std::vector<std::pair<int, int>> checkCapture(const Board &board, const std::pair<int, int> &lastMove, int color);

#endif
