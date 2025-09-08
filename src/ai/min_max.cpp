/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_max.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:01:13 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/06 14:41:00 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <set>
#include <algorithm>
#include "../../include/ai/ai_config.hpp"

int minMax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer, int player, t_move *bestMove)
{
	// If at maximum depth, evaluate the board
	if (depth >= MAX_DEPTH)
	{
		bestMove->score = staticBoardEvaluation(board, player);
		return bestMove->score;
	}

	int currentPlayer = maximizingPlayer ? player : (player == BLACK_STONE ? WHITE_STONE : BLACK_STONE);
	std::vector<std::pair<int, int>> validMoves = getValidMoves(board, currentPlayer);

	if (validMoves.empty())
		return staticBoardEvaluation(board, player);

	if (maximizingPlayer)
	{
		int maxEval = INT_MIN;

		for (const auto &move : validMoves)
		{
			board->set(move.first, move.second, player);

			// Check for immediate win
			if (board->checkWin(move.first, move.second, player))
			{
				board->set(move.first, move.second, EMPTY);
				bestMove->row = move.first;
				bestMove->col = move.second;
				bestMove->score = FIVE_IN_A_ROW;
				return FIVE_IN_A_ROW;
			}

			t_move tempMove;
			int eval = minMax(board, depth + 1, alpha, beta, false, player, &tempMove);

			board->set(move.first, move.second, EMPTY);

			if (eval > maxEval)
			{
				maxEval = eval;
				bestMove->row = move.first;
				bestMove->col = move.second;
				bestMove->score = eval;
			}

			alpha = std::max(alpha, eval);
			if (beta <= alpha)
			{
				break; // Alpha-beta pruning
			}
		}
		return maxEval;
	}
	else
	{
		int minEval = INT_MAX;
		int opponent = (player == BLACK_STONE) ? WHITE_STONE : BLACK_STONE;

		for (const auto &move : validMoves)
		{
			board->set(move.first, move.second, opponent);

			// Check for immediate opponent win
			if (board->checkWin(move.first, move.second, opponent))
			{
				board->set(move.first, move.second, EMPTY);
				bestMove->row = move.first;
				bestMove->col = move.second;
				bestMove->score = -FIVE_IN_A_ROW;
				return -FIVE_IN_A_ROW;
			}

			t_move tempMove;
			int eval = minMax(board, depth + 1, alpha, beta, true, player, &tempMove);

			board->set(move.first, move.second, EMPTY);

			if (eval < minEval)
			{
				minEval = eval;
				bestMove->row = move.first;
				bestMove->col = move.second;
				bestMove->score = eval;
			}

			beta = std::min(beta, eval);
			if (beta <= alpha)
			{
				break; // Alpha-beta pruning
			}
		}
		return minEval;
	}
}