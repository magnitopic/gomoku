/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_config.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:08:24 by alaparic          #+#    #+#             */
/*   Updated: 2025/10/04 12:42:48 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AI_config_HPP_
#define AI_config_HPP_

class Player;

#include <iostream>
#include <climits>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include "../classes/Board.hpp"
#include "../classes/Player.hpp"
#include "../structs.hpp"

/* AI configuration parameters */

#define MAX_DEPTH 5	   // Maximum search depth
#define MAX_BRANCHES 2 // Maximum branches to explore at each node

// Pattern scores for the heuristic
#define FIVE_IN_A_ROW 1000000
#define STONE_CAPTURE 500000
#define OPEN_FOUR 100000
#define FOUR 10000
#define OPEN_THREE 5000
#define THREE 1000
#define OPEN_TWO 200
#define TWO 50

#endif