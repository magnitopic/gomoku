/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_max.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adiaz-uf <adiaz-uf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:01:13 by alaparic          #+#    #+#             */
/*   Updated: 2025/10/10 12:45:36 by adiaz-uf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ai/AI.hpp"

int AI::minMax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer, int player, s_move *bestMove, int maxDepth = MAX_DEPTH)
{
	// If at maximum depth, evaluate the board
	if (depth >= maxDepth)
	{
		bestMove->score = this->getBoardValue(board, player);
		return bestMove->score;
	}

	int currentPlayer = maximizingPlayer ? player : -player;
	std::vector<std::pair<int, int>> validMoves = this->getValidMoves(board, currentPlayer);

	/* std::cout << T_YELLOW << "Depth: " << depth << " | Valid moves: " << validMoves.size() << T_BLUE << std::endl; */
	//std::cout << T_YELLOW << "Depth: " << depth << std::endl;
	if (validMoves.empty())
		return this->getBoardValue(board, player);

	if (maximizingPlayer)
	{
		int maxEval = INT_MIN;
		bool isFirstMove = true;
		
		for (const std::pair<int, int> &move : validMoves)
		{
			board->set(move.first, move.second, player);

			// Check for immediate win
			if (board->simpleCheckWin(move.first, move.second, player))
			{
				board->set(move.first, move.second, EMPTY);
				bestMove->row = move.first;
				bestMove->col = move.second;
				bestMove->score = FIVE_IN_A_ROW;
				return FIVE_IN_A_ROW;
			}

			s_move tempMove;
			int searchDepth = (isFirstMove && depth == 0) ? maxDepth : 2;
			int eval = minMax(board, depth + 1, alpha, beta, false, player, &tempMove, searchDepth);

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
				break; // Alpha-beta pruning
			
			isFirstMove = false;
		}
		return maxEval;
	}
	else
	{
		int minEval = INT_MAX;
		int opponent = (player == BLACK_STONE) ? WHITE_STONE : BLACK_STONE;
		bool isFirstMove = true;
		
		for (const std::pair<int, int> &move : validMoves)
		{
			board->set(move.first, move.second, opponent);

			// Check for immediate opponent win
			if (board->simpleCheckWin(move.first, move.second, opponent))
			{
				board->set(move.first, move.second, EMPTY);
				bestMove->row = move.first;
				bestMove->col = move.second;
				bestMove->score = -FIVE_IN_A_ROW;
				return -FIVE_IN_A_ROW;
			}

			s_move tempMove;
			int searchDepth = (isFirstMove && depth == 0) ? maxDepth : 2;
            int eval = minMax(board, depth + 1, alpha, beta, true, player, &tempMove, searchDepth);

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
				break; // Alpha-beta pruning
			
			isFirstMove = false;
		}
		return minEval;
	}
}
