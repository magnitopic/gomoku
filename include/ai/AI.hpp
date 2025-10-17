/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AI.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adiaz-uf <adiaz-uf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 17:42:20 by alaparic          #+#    #+#             */
/*   Updated: 2025/10/15 18:25:21 by adiaz-uf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AI_HPP_
#define AI_HPP_

#include <iostream>
#include <unordered_map>
#include "./ai_config.hpp"
#include "../structs.hpp"

class AI
{
private:
	std::unordered_map<uint64_t, float> cache;
	static const size_t MAX_ENTRIES = 10000000;
	bool isFirstMove = true;
	
public:
	std::string difficulty;
	AI();
	AI(std::string difficulty);
	AI(const AI &copy);
	AI &operator=(const AI &assign);
	~AI();

	// Main AI function
	std::pair<int, int> getAIMove(Board *board, int color);

	/** min_max.cpp **/
	int minMax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer, int player, s_move *bestMove, std::string difficulty, int maxDepth = MAX_DEPTH);

	/** detect_threats.cpp **/
	int evaluateThreatScore(Board *board, const std::pair<int, int> &move, int aiColor);

	/** static_board_evaluations.cpp **/
	int detectPatterns(Board *board, int color);
	int staticBoardEvaluation(Board *board, int player);
	int getBoardValue(Board *board, int player);

	/** evaluate_line.cpp **/
	std::vector<s_pattern_match> getPatternDefinitions(int player);
	std::vector<s_pattern> resolveOverlaps(std::vector<s_pattern> &matches);
	std::vector<s_pattern> findPatterns(const std::vector<int> &line, int player);
	int evaluateLine(const std::vector<int> &line, int player);

	/** get_moves.cpp **/
	int quickEvaluatePos(const std::pair<int, int> &pos, Board *board, int color);
	bool checkIllegalMove(Board *board, const std::pair<int, int> &cell, int color);
	void sortMoves(Board *board, int color, std::vector<std::pair<int, int>> &moves, std::string difficulty);
	std::vector<std::pair<int, int>> getValidMoves(Board *board, int color, std::string difficulty);
};

#endif
