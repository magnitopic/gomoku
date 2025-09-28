/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_line.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:03:00 by alaparic          #+#    #+#             */
/*   Updated: 2025/09/28 17:51:46 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ai/AI.hpp"

std::vector<s_pattern_match> AI::getPatternDefinitions(int player)
{
	int opponent = -player;

	return {
		// Five in a row
		{{player, player, player, player, player}, e_FIVE_IN_A_ROW, FIVE_IN_A_ROW},

		// Capture opportunities
		{{player, opponent, opponent, player}, e_STONE_CAPTURE, STONE_CAPTURE},
		{{opponent, player, player, opponent}, e_STONE_CAPTURE, -STONE_CAPTURE},

		// Open four
		{{0, player, player, player, player, 0}, e_OPEN_FOUR, OPEN_FOUR},

		// Four with one end blocked or with gap
		{{player, player, player, player, 0}, e_FOUR, FOUR},
		{{0, player, player, player, player}, e_FOUR, FOUR},
		{{player, player, player, 0, player}, e_FOUR, FOUR},
		{{player, player, 0, player, player}, e_FOUR, FOUR},
		{{player, 0, player, player, player}, e_FOUR, FOUR},

		// Open three - both ends open
		{{0, player, player, player, 0}, e_OPEN_THREE, OPEN_THREE},
		{{0, player, player, 0, player, 0}, e_OPEN_THREE, OPEN_THREE},
		{{0, player, 0, player, player, 0}, e_OPEN_THREE, OPEN_THREE},

		// Three with one end blocked or gaps
		{{player, player, player, 0, 0}, e_THREE, THREE},
		{{0, 0, player, player, player}, e_THREE, THREE},
		{{player, player, 0, player, 0}, e_THREE, THREE},
		{{0, player, player, 0, player}, e_THREE, THREE},
		{{0, player, 0, player, player}, e_THREE, THREE},
		{{player, 0, player, player, 0}, e_THREE, THREE},
		{{player, player, 0, 0, player}, e_THREE, THREE},
		{{player, 0, 0, player, player}, e_THREE, THREE},
		{{player, 0, player, 0, player}, e_THREE, THREE},

		// Open two - both ends open
		{{0, player, player, 0, 0}, e_OPEN_TWO, OPEN_TWO},
		{{0, 0, player, player, 0}, e_OPEN_TWO, OPEN_TWO},
		{{0, player, 0, player, 0}, e_OPEN_TWO, OPEN_TWO},

		// Two with one end blocked
		{{player, player, 0, 0, 0}, e_TWO, TWO},
		{{0, 0, 0, player, player}, e_TWO, TWO},
		{{player, 0, 0, player, 0}, e_TWO, TWO},
		{{0, player, 0, 0, player}, e_TWO, TWO},
		{{player, 0, player, 0, 0}, e_TWO, TWO},
		{{0, 0, player, 0, player}, e_TWO, TWO},
		{{player, 0, 0, 0, player}, e_TWO, TWO},
	};
}

// Remove overlapping patterns, keeping highest scoring ones
std::vector<s_pattern> AI::resolveOverlaps(std::vector<s_pattern> &matches)
{
	if (matches.empty())
		return matches;

	// Sort by score (highest first), then by position
	std::sort(matches.begin(), matches.end(),
			  [](const s_pattern &a, const s_pattern &b)
			  {
				  if (a.score != b.score)
					  return a.score > b.score;
				  return a.position < b.position;
			  });

	std::vector<s_pattern> result;
	std::vector<bool> used(matches.size(), false);

	for (size_t i = 0; i < matches.size(); i++)
	{
		if (used[i])
			continue;

		result.push_back(matches[i]);

		// Find the pattern definition to get actual length
		std::vector<s_pattern_match> patternDefs = this->getPatternDefinitions(1);
		int length1 = 0;
		for (const s_pattern_match &def : patternDefs)
		{
			if (def.type == matches[i].type)
			{
				length1 = def.pattern.size();
				break;
			}
		}

		// Mark overlapping patterns as used
		for (size_t j = i + 1; j < matches.size(); j++)
		{
			if (used[j])
				continue;

			// Get length of second pattern
			int length2 = 0;
			for (const s_pattern_match &def : patternDefs)
			{
				if (def.type == matches[j].type)
				{
					length2 = def.pattern.size();
					break;
				}
			}

			// Check if patterns overlap
			int start1 = matches[i].position;
			int end1 = start1 + length1;
			int start2 = matches[j].position;
			int end2 = start2 + length2;

			if (!(end1 <= start2 || end2 <= start1))
			{
				used[j] = true;
			}
		}
	}

	return result;
}

std::vector<s_pattern> AI::findPatterns(const std::vector<int> &line, int player)
{
	std::vector<s_pattern> foundPatterns;
	std::vector<s_pattern_match> patternDefs = this->getPatternDefinitions(player);

	// Search for each pattern in the line
	for (const s_pattern_match &patternDef : patternDefs)
	{
		size_t patternSize = patternDef.pattern.size();

		if (patternSize > line.size())
			continue;
		for (size_t i = 0; i <= line.size() - patternSize; i++)
		{
			bool matches = true;

			// Check if pattern matches at this position
			for (size_t j = 0; j < patternSize; j++)
			{
				if (line[i + j] != patternDef.pattern[j])
				{
					matches = false;
					break;
				}
			}

			if (matches)
			{
				s_pattern match;
				match.type = patternDef.type;
				match.position = i;
				match.score = patternDef.score;
				foundPatterns.push_back(match);
			}
		}
	}

	// Remove overlapping patterns to avoid double counting
	return this->resolveOverlaps(foundPatterns);
}

int AI::evaluateLine(const std::vector<int> &line, int player)
{
	std::vector<s_pattern> patterns = this->findPatterns(line, player);
	int totalScore = 0;

	for (const s_pattern &pattern : patterns)
		totalScore += pattern.score;

	return totalScore;
}
