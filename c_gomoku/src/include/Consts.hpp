/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Consts.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:57:18 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/12 12:58:47 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTS_HPP_
#define CONSTS_HPP_

#include <iostream>

#define SCREEN_SIZE 920
#define MARGIN 90
#define BORDER_MARGIN 30

#define BPP sizeof(int32_t)

// Board values
#define EMPTY = 0
#define BLACK_STONE = 1
#define WHITE_STONE = -1

// Colors
const int BLACK = 0xFF000000;
const int WHITE = 0xFFFFFFFF;
const int BORDER_COLOR = 0xFF969696;
const int WOOD = 0xFF87B8DE;
const int RED = 0xFF0000FF;

// Defining colors for the terminal
#define T_RED = "\033[0;31m"
#define T_GREEN = "\033[0;32m"
#define T_YELLOW = "\033[0;33m"
#define T_BLUE = "\033[0;34m"
#define T_PURPLE = "\033[0;35m"
#define T_CYAN = "\033[0;36m"
#define T_WHITE = "\033[0;37m"
#define T_RESET = "\033[0m"
#define T_GRAY = "\033[2;30m"
#define T_BOLD = "\033[1m"

#endif
