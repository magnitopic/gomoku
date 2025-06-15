/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_max.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:01:13 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/15 08:58:52 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ai_config.hpp"

static std::vector<std::pair<int, int>> getValidMoves(Board *board)
{
	// TODO: add check for valid moves
	std::vector<std::pair<int, int>> validMoves;
	std::vector<std::pair<int, int>> occupiedTiles = board->getOccupiedTiles();
	for (const std::pair<int, int> &tile : occupiedTiles)
	{
		int row = tile.first;
		int col = tile.second;

		// Check all 4 directions for valid moves
		for (const std::pair<int, int> &dir : DIRECTIONS)
		{
			for (int i = -1; i <= 1; i += 2) // Check both directions
			{
				int newRow = row + dir.first * i;
				int newCol = col + dir.second * i;

				if (board->inBounds(newRow, newCol) && board->isEmpty(newRow, newCol))
					validMoves.push_back({newRow, newCol});
			}
		}
	}
	return validMoves;
}

static int evaluateBoard(Board *board)
{
	// Simple evaluation function
	int score = 0;
	int black_score = 0;
	int white_score = 0;

	// Check for patterns in all 4 directions
	for (int i = 0; i < board->getSize(); i++)
	{
		for (int j = 0; j < board->getSize(); j++)
		{
			if (board->get(i, j) == EMPTY)
				continue;

			int player = board->get(i, j);

			// Check all 4 directions
			for (std::pair<int, int> dir : DIRECTIONS)
			{
				int dx = dir.first;
				int dy = dir.second;
				int count = 1;
				int open_ends = 0;

				// Check backward
				int r = i - dx;
				int c = j - dy;
				if (r >= 0 && r < board->getSize() && c >= 0 && c < board->getSize())
				{
					if (board->get(r, c) == EMPTY)
						open_ends++;
				}

				// Check forward
				for (int k = 1; k < 5; k++)
				{
					r = i + k * dx;
					c = j + k * dy;

					if (r < 0 || r >= board->getSize() || c < 0 || c >= board->getSize())
						break;

					if (board->get(r, c) == player)
						count++;
					else if (board->get(r, c) == EMPTY)
					{
						open_ends++;
						break;
					}
					else
						break;
				}

				// Assign score based on pattern
				int pattern_score = 0;

				if (count >= 5)
					pattern_score = FIVE_IN_A_ROW;
				else if (count == 4 && open_ends == 2)
					pattern_score = OPEN_FOUR;
				else if (count == 4 && open_ends == 1)
					pattern_score = FOUR;
				else if (count == 3 && open_ends == 2)
					pattern_score = OPEN_THREE;
				else if (count == 3 && open_ends == 1)
					pattern_score = THREE;
				else if (count == 2 && open_ends == 2)
					pattern_score = OPEN_TWO;
				else if (count == 2 && open_ends == 1)
					pattern_score = TWO;

				if (player == BLACK)
					black_score += pattern_score;
				else
					white_score += pattern_score;
			}
		}
	}

	score = black_score - white_score;
	return score;
}

int minMax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer, int player, t_move *bestMove)
{
	// If at maximum depth, evaluate the board
	if (depth >= MAX_DEPTH)
	{
		bestMove->score = evaluateBoard(board);
		return bestMove->score;
	}

	std::vector<std::pair<int, int>> validMoves = getValidMoves(board);
	if (validMoves.empty())
		return evaluateBoard(board);

	if (maximizingPlayer)
	{
		int maxEval = INT_MIN;
		for (std::pair<int, int> move : validMoves)
		{
			board->set(move.first, move.second, player);
			int eval = minMax(board, depth + 1, alpha, beta, false, player, bestMove);
			board->set(move.first, move.second, EMPTY);
			if (eval > maxEval)
			{
				maxEval = eval;
				bestMove->row = move.first;
				bestMove->col = move.second;
				bestMove->score = eval;
			}
		}
		return maxEval;
	}
	else
	{
		int minEval = INT_MAX;
		for (std::pair<int, int> move : validMoves)
		{
			board->set(move.first, move.second, player * -1);
			int eval = minMax(board, depth + 1, alpha, beta, true, player, bestMove);
			board->set(move.first, move.second, EMPTY);
			if (eval < minEval)
			{
				minEval = eval;
				bestMove->row = move.first;
				bestMove->col = move.second;
				bestMove->score = eval;
			}
		}
		return minEval;
	}
}
