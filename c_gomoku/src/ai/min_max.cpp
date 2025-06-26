/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_max.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:01:13 by alaparic          #+#    #+#             */
/*   Updated: 2025/06/25 12:14:10 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ai_config.hpp"
#include <set>
#include <algorithm>

static bool checkIllegalMove(Board *board, const std::pair<int, int> &cell, int color)
{
	int col = cell.first;
	int row = cell.second;

	if (!board->inBounds(col, row))
		return true;

	if (checkOccupiedCell(*board, col, row))
		return true;

	if (board->getGameMode() != "standard")
	{
		if (checkDoubleThree(*board, cell, color))
			return true;

		if (checkMoveIntoCapture(*board, cell, color))
			return true;
	}

	return false;
}

// OFFENSIVE: Find moves that create winning threats
static std::vector<std::pair<int, int>> findAggressiveMoves(Board *board, int player)
{
	std::vector<std::pair<int, int>> aggressiveMoves;

	for (int y = 0; y < board->getSize(); y++)
	{
		for (int x = 0; x < board->getSize(); x++)
		{
			if (board->get(x, y) != EMPTY)
				continue;
			if (checkIllegalMove(board, {x, y}, player))
				continue;

			// Try this move
			board->set(x, y, player);

			int aggressionScore = 0;

			// Check if this creates multiple threats
			for (const auto &direction : DIRECTIONS)
			{
				// Check for 4-in-a-row (immediate win threat)
				if (board->checkNInARow({x, y}, player, direction, 4))
				{
					aggressionScore += 1000; // Creates winning threat
				}
				// Check for 3-in-a-row (strong threat)
				else if (board->checkNInARow({x, y}, player, direction, 3))
				{
					aggressionScore += 100; // Creates strong threat
				}
				// Check for 2-in-a-row (building threat)
				else if (board->checkNInARow({x, y}, player, direction, 2))
				{
					aggressionScore += 10; // Building threat
				}
			}

			// Bonus for creating multiple threats (forcing moves)
			std::vector<t_pattern> myPatterns = detectPatterns(board, player);
			int highThreatCount = 0;
			for (const auto &pattern : myPatterns)
			{
				if (pattern.threatLevel >= 4)
				{
					highThreatCount++;
				}
			}

			if (highThreatCount >= 2)
			{
				aggressionScore += 500; // Multiple high threats = very aggressive
				std::cout << T_GREEN << "DOUBLE THREAT OPPORTUNITY at (" << x << "," << y << ")!" << T_RESET << std::endl;
			}

			// Restore board
			board->set(x, y, EMPTY);

			// Consider aggressive moves (score > 50)
			if (aggressionScore >= 50)
			{
				aggressiveMoves.push_back({x, y});
				std::cout << T_CYAN << "Aggressive move (" << x << "," << y << ") score: " << aggressionScore << T_RESET << std::endl;
			}
		}
	}

	// Sort by aggression potential (highest first)
	std::sort(aggressiveMoves.begin(), aggressiveMoves.end(),
			  [board, player](const std::pair<int, int> &a, const std::pair<int, int> &b)
			  {
				  // Calculate aggression score for sorting
				  board->set(a.first, a.second, player);
				  int scoreA = 0;
				  for (const auto &dir : DIRECTIONS)
				  {
					  if (board->checkNInARow(a, player, dir, 4))
						  scoreA += 1000;
					  else if (board->checkNInARow(a, player, dir, 3))
						  scoreA += 100;
				  }
				  board->set(a.first, a.second, EMPTY);

				  board->set(b.first, b.second, player);
				  int scoreB = 0;
				  for (const auto &dir : DIRECTIONS)
				  {
					  if (board->checkNInARow(b, player, dir, 4))
						  scoreB += 1000;
					  else if (board->checkNInARow(b, player, dir, 3))
						  scoreB += 100;
				  }
				  board->set(b.first, b.second, EMPTY);

				  return scoreA > scoreB;
			  });

	return aggressiveMoves;
}

// WINNING: Find immediate wins
static std::vector<std::pair<int, int>> findWinningMoves(Board *board, int player)
{
	std::vector<std::pair<int, int>> winningMoves;

	for (int y = 0; y < board->getSize(); y++)
	{
		for (int x = 0; x < board->getSize(); x++)
		{
			if (board->get(x, y) != EMPTY)
				continue;
			if (checkIllegalMove(board, {x, y}, player))
				continue;

			board->set(x, y, player);
			bool isWin = board->checkWin(x, y, player);
			board->set(x, y, EMPTY);

			if (isWin)
			{
				winningMoves.push_back({x, y});
				std::cout << T_GREEN << "WINNING MOVE FOUND at (" << x << "," << y << ")" << T_RESET << std::endl;
			}
		}
	}

	return winningMoves;
}

// ENHANCED: Find capturing opportunities
static std::vector<std::pair<int, int>> findCaptureMoves(Board *board, int player)
{
	std::vector<std::pair<int, int>> captureMoves;

	for (int y = 0; y < board->getSize(); y++)
	{
		for (int x = 0; x < board->getSize(); x++)
		{
			if (board->get(x, y) != EMPTY)
				continue;
			if (checkIllegalMove(board, {x, y}, player))
				continue;

			// Check if this move captures opponent stones
			std::vector<std::pair<int, int>> captured = checkCapture(*board, {x, y}, player);

			if (!captured.empty())
			{
				captureMoves.push_back({x, y});
				std::cout << T_YELLOW << "CAPTURE OPPORTUNITY at (" << x << "," << y << ") - "
						  << captured.size() << " stones" << T_RESET << std::endl;
			}
		}
	}

	return captureMoves;
}

// ENHANCED: Balanced move generation with aggression
static std::vector<std::pair<int, int>> getValidMoves(Board *board, int color)
{
	int opponent = (color == BLACK_STONE) ? WHITE_STONE : BLACK_STONE;

	// PRIORITY 1: Take immediate wins
	std::vector<std::pair<int, int>> winningMoves = findWinningMoves(board, color);
	if (!winningMoves.empty())
	{
		std::cout << T_GREEN << "🏆 TAKING THE WIN!" << T_RESET << std::endl;
		return winningMoves;
	}

	// PRIORITY 2: Block opponent wins
	std::vector<std::pair<int, int>> opponentWins = findWinningMoves(board, opponent);
	if (!opponentWins.empty())
	{
		std::cout << T_RED << "🛡️ EMERGENCY BLOCK!" << T_RESET << std::endl;
		std::vector<std::pair<int, int>> validBlocks;
		for (const auto &move : opponentWins)
		{
			if (!checkIllegalMove(board, move, color))
			{
				validBlocks.push_back(move);
			}
		}
		if (!validBlocks.empty())
			return validBlocks;
	}

	// PRIORITY 3: AGGRESSIVE OFFENSIVE MOVES (NEW!)
	std::vector<std::pair<int, int>> aggressiveMoves = findAggressiveMoves(board, color);
	std::vector<std::pair<int, int>> strongOffensive;

	// Filter for really strong offensive moves (4-in-a-row builders)
	for (const auto &move : aggressiveMoves)
	{
		board->set(move.first, move.second, color);
		bool createsStrongThreat = false;

		for (const auto &direction : DIRECTIONS)
		{
			if (board->checkNInARow(move, color, direction, 4))
			{
				createsStrongThreat = true;
				break;
			}
		}

		board->set(move.first, move.second, EMPTY);

		if (createsStrongThreat)
		{
			strongOffensive.push_back(move);
		}
	}

	if (!strongOffensive.empty())
	{
		std::cout << T_GREEN << "⚔️ STRONG OFFENSIVE PLAY! " << strongOffensive.size() << " moves" << T_RESET << std::endl;
		return strongOffensive; // Prioritize strong offensive moves!
	}

	// PRIORITY 4: Block serious threats (but less priority than offense)
	std::vector<std::pair<int, int>> criticalDefensive = findCriticalMoves(board, opponent);
	std::vector<std::pair<int, int>> validDefensive;

	for (const auto &move : criticalDefensive)
	{
		if (!checkIllegalMove(board, move, color))
		{
			validDefensive.push_back(move);
		}
	}

	// PRIORITY 5: Capture opportunities
	std::vector<std::pair<int, int>> captureMoves = findCaptureMoves(board, color);

	// PRIORITY 6: Medium offensive moves
	std::vector<std::pair<int, int>> mediumOffensive;
	for (const auto &move : aggressiveMoves)
	{
		if (strongOffensive.empty() ||
			std::find(strongOffensive.begin(), strongOffensive.end(), move) == strongOffensive.end())
		{
			mediumOffensive.push_back(move);
		}
	}

	// BUILD FINAL MOVE LIST with balanced approach
	std::vector<std::pair<int, int>> finalMoves;

	// Add most critical defensive moves (limit to 2)
	int defensiveCount = 0;
	for (const auto &move : validDefensive)
	{
		if (defensiveCount < 2)
		{
			finalMoves.push_back(move);
			defensiveCount++;
		}
	}

	// Add capture moves (high value)
	for (const auto &move : captureMoves)
	{
		if (finalMoves.size() < 6)
		{
			finalMoves.push_back(move);
		}
	}

	// Add medium offensive moves (prioritize aggression)
	for (const auto &move : mediumOffensive)
	{
		if (finalMoves.size() < 8)
		{
			finalMoves.push_back(move);
		}
	}

	// If we still need moves, add strategic positioning
	if (finalMoves.size() < 4)
	{
		std::vector<std::pair<int, int>> occupiedTiles = board->getOccupiedTiles();

		if (occupiedTiles.empty())
		{
			int center = board->getSize() / 2;
			finalMoves.push_back({center, center});
			return finalMoves;
		}

		// Generate strategic moves around existing stones
		std::set<std::pair<int, int>> strategicMoves;

		for (const auto &tile : occupiedTiles)
		{
			for (int dr = -1; dr <= 1; dr++)
			{
				for (int dc = -1; dc <= 1; dc++)
				{
					if (dr == 0 && dc == 0)
						continue;

					int newRow = tile.first + dr;
					int newCol = tile.second + dc;

					if (board->inBounds(newRow, newCol) &&
						board->isEmpty(newRow, newCol) &&
						!checkIllegalMove(board, {newRow, newCol}, color))
					{
						strategicMoves.insert({newRow, newCol});
					}
				}
			}
		}

		for (const auto &move : strategicMoves)
		{
			if (finalMoves.size() >= MAX_BRANCHING)
				break;
			finalMoves.push_back(move);
		}
	}

	// Remove duplicates
	std::sort(finalMoves.begin(), finalMoves.end());
	finalMoves.erase(std::unique(finalMoves.begin(), finalMoves.end()), finalMoves.end());

	std::cout << T_WHITE << "Final move selection: " << finalMoves.size()
			  << " moves (Balanced Offense/Defense)" << T_RESET << std::endl;

	return finalMoves;
}

int minMax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer, int player, t_move *bestMove)
{
	// If at maximum depth, evaluate the board
	if (depth >= MAX_DEPTH)
	{
		bestMove->score = staticBoardEvaluation(board, player, depth);
		return bestMove->score;
	}

	int currentPlayer = maximizingPlayer ? player : (player == BLACK_STONE ? WHITE_STONE : BLACK_STONE);
	std::vector<std::pair<int, int>> validMoves = getValidMoves(board, currentPlayer);

	if (validMoves.empty())
		return staticBoardEvaluation(board, player, depth);

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