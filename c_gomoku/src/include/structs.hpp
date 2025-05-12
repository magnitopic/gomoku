/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:15:29 by alaparic          #+#    #+#             */
/*   Updated: 2025/05/12 13:51:27 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_HPP_
#define STRUCTS_HPP_

#include <string>

struct s_game_config
{
	bool is_ai;
	std::string difficulty;
	int board_size;
	bool save_history;
	std::string game_mode;
};

#endif
