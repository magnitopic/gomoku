/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   direction-vectors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:22:17 by alaparic          #+#    #+#             */
/*   Updated: 2025/04/13 00:22:18 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "gomoku.h"

// Define the direction vectors (declared as extern in gomoku.h)
const int dx[4] = {1, 0, 1, 1};
const int dy[4] = {0, 1, 1, -1};