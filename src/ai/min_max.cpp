/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_max.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:01:13 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/10 20:38:10 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <set>
#include <algorithm>
#include "../../include/ai/ai_config.hpp"

/* Temporary! Until better move system ------------ */
static bool checkIllegalMove(Board *board, const std::pair<int, int> &cell, int color)
{
	int col = cell.first;
	int row = cell.second;

	// Check if the move is out of bounds
	if (!board->inBounds(col, row))
		return true;

	// Check if the cell is already occupied
	if (checkOccupiedCell(*board, col, row))
		return true;

	if (board->getGameMode() != "standard")
	{
		// Check if the move creates a double three
		if (checkDoubleThree(*board, cell, color))
			return true;

		// Check if the move is into a capture
		if (checkMoveIntoCapture(*board, cell, color))
			return true;
	}

	return false;
}

static std::vector<std::pair<int, int>> getValidMoves(Board *board, int color)
{
	std::vector<std::pair<int, int>> validMoves;
	std::vector<std::pair<int, int>> occupiedTiles = board->getOccupiedTiles();
	for (const std::pair<int, int> &tile : occupiedTiles)
	{
		int row = tile.first;
		int col = tile.second;

		// Check all 4 directions for valid moves
		for (const std::pair<int, int> &dir : DIRECTIONS)
		{
			for (int i = -1; i <= 1; i += 2)
			{
				int newRow = row + dir.first * i;
				int newCol = col + dir.second * i;

				if (board->inBounds(newRow, newCol) && board->isEmpty(newRow, newCol) && !checkIllegalMove(board, {newRow, newCol}, color))
					validMoves.push_back({newRow, newCol});
			}
		}
	}
	return validMoves;
}
/* ---------------------------------------------- */

int minMax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer, int player, t_move *bestMove)
{
	// If at maximum depth, evaluate the board
	if (depth >= MAX_DEPTH)
	{
		bestMove->score = staticBoardEvaluation(board, player);
		return bestMove->score;
	}

	int currentPlayer = maximizingPlayer ? player : -player;
	std::vector<std::pair<int, int>> validMoves = getValidMoves(board, currentPlayer);

	std::cout << T_YELLOW << "Depth: " << depth << " | Valid moves: " << validMoves.size() << T_BLUE << std::endl;

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