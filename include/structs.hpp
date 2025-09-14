/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:15:29 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/14 17:37:45 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_HPP_
#define STRUCTS_HPP_

#include <string>
#include "./enums.hpp"

struct s_game_config
{
	bool is_ai;
	std::string difficulty;
	int board_size;
	bool save_history;
	std::string game_mode;
};

struct s_move
{
	int row;
	int col;
	int score;
};

struct s_pattern
{
	e_patterns type;
	int position;
	int score;
};

struct s_pattern_match
{
	std::vector<int> pattern;
	e_patterns type;
	int score;
};

struct s_scored_move
{
	std::pair<int, int> move;
	int score;
};

#endif
