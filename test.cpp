#include "include/ai/ai_config.hpp"
#include "include/ai/ai_config.hpp"
#include <vector>
#include <algorithm>

struct PatternMatch
{
	e_patterns type;
	int position;
	int score;
};

class GomokuPatternEvaluator
{
private:
	struct PatternDef
	{
		std::vector<int> pattern;
		e_patterns type;
		int score;
	};

	std::vector<PatternDef> getPatternDefinitions(int player)
	{
		return {
			// Five in a row
			{{player, player, player, player, player}, e_FIVE_IN_A_ROW, FIVE_IN_A_ROW},

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

	int getPatternLength(const std::vector<int> &pattern)
	{
		return pattern.size();
	}

	// Remove overlapping patterns, keeping highest scoring ones
	std::vector<PatternMatch> resolveOverlaps(std::vector<PatternMatch> &matches)
	{
		if (matches.empty())
			return matches;

		// Sort by score (highest first), then by position
		std::sort(matches.begin(), matches.end(),
				  [](const PatternMatch &a, const PatternMatch &b)
				  {
					  if (a.score != b.score)
						  return a.score > b.score;
					  return a.position < b.position;
				  });

		std::vector<PatternMatch> result;
		std::vector<bool> used(matches.size(), false);

		for (size_t i = 0; i < matches.size(); i++)
		{
			if (used[i])
				continue;

			result.push_back(matches[i]);

			// Find the pattern definition to get actual length
			auto patternDefs = getPatternDefinitions(1); // Dummy player for length lookup
			int length1 = 0;
			for (const auto &def : patternDefs)
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
				for (const auto &def : patternDefs)
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

public:
	std::vector<PatternMatch> findPatterns(const std::vector<int> &line, int player)
	{
		std::vector<PatternMatch> foundPatterns;
		auto patternDefs = getPatternDefinitions(player);

		// Search for each pattern in the line
		for (const auto &patternDef : patternDefs)
		{
			int patternSize = patternDef.pattern.size();

			for (size_t i = 0; i <= line.size() - patternSize; i++)
			{
				bool matches = true;

				// Check if pattern matches at this position
				for (int j = 0; j < patternSize; j++)
				{
					if (line[i + j] != patternDef.pattern[j])
					{
						matches = false;
						break;
					}
				}

				if (matches)
				{
					PatternMatch match;
					match.type = patternDef.type;
					match.position = i;
					match.score = patternDef.score;
					foundPatterns.push_back(match);
				}
			}
		}

		// Remove overlapping patterns to avoid double counting
		return resolveOverlaps(foundPatterns);
	}

	int evaluateLine(const std::vector<int> &line, int player)
	{
		auto patterns = findPatterns(line, player);
		int totalScore = 0;

		for (const auto &pattern : patterns)
		{
			totalScore += pattern.score;
		}

		return totalScore;
	}
};

// Main interface function using your existing enum
std::vector<PatternMatch> classifyPattern(const std::vector<int> &line, int player)
{
	GomokuPatternEvaluator evaluator;
	return evaluator.findPatterns(line, player);
}

// Convenience function for board evaluation with wall handling
int evaluateLinePatterns(const std::vector<int> &line, int player)
{
	GomokuPatternEvaluator evaluator;
	return evaluator.evaluateLine(line, player);
}

// Function to convert board line with wall markers
std::vector<int> prepareBoardLine(const std::vector<int> &boardLine, bool addWallStart = false, bool addWallEnd = false)
{
	std::vector<int> line;

	if (addWallStart)
	{
		line.push_back(-2); // Wall marker
	}

	for (int cell : boardLine)
	{
		line.push_back(cell);
	}

	if (addWallEnd)
	{
		line.push_back(-2); // Wall marker
	}

	return line;
}

// Helper function for debugging
std::string getPatternName(e_patterns pattern)
{
	switch (pattern)
	{
	case e_FIVE_IN_A_ROW:
		return "FIVE_IN_A_ROW";
	case e_OPEN_FOUR:
		return "OPEN_FOUR";
	case e_FOUR:
		return "FOUR";
	case e_OPEN_THREE:
		return "OPEN_THREE";
	case e_THREE:
		return "THREE";
	case e_OPEN_TWO:
		return "OPEN_TWO";
	case e_TWO:
		return "TWO";
	default:
		return "UNKNOWN";
	}
}

int main(void)
{
	std::vector<int> line = {1, 1, 1, 1, 0, 0, 0, 0, 1, 1};
	int color = 1;

	auto patterns = classifyPattern(line, color);
	for (const auto &pattern : patterns)
	{
		std::cout << "Pattern: " << getPatternName(pattern.type)
				  << ", Position: " << pattern.position
				  << ", Score: " << pattern.score << std::endl;
	}
	return 0;
}
