//
// FILENAME: successstates.cpp | Shifting Stones Search
// DESCRIPTION: Determine if a given board is a success state for a given target card
// CREATED: 2024-03-30 @ 7:38 PM
//

#include "successstates.hpp"

#include <iostream>

namespace success_states {

	/**
	 * @brief 
	 * 
	 * @param board 
	 * @param target 
	 * @return true 
	 * @return false 
	 */
	bool isSuccessState(board_t board, const std::string& target) {
		std::vector<std::string> successStates;

		try {
			successStates = SUCCESS_STATES.at(target); // The set of possible success states for the given target card
		}
		catch (const std::out_of_range&) {
			return false;
		}

		for (const auto& state: successStates) {
			if (matchesSuccessState(board, state)) {
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief 
	 * 
	 * @param board 
	 * @param state 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool matchesSuccessState(board_t board, const std::string& state) {
		std::string boardID = getID(board); // The ID of the current board
		const char PLACEHOLDERS[] = {'8', '9'};
		bool match = true;
		
		//
		// Check if the board's ID number satisfies any of the success states for the target.
		//
		// The digits 8 and 9 are placeholders, and can be filled with any tile on the board.
		// These values are ignored when comparing.
		//
		for (std::size_t i = 0; i < state.size() && match; i++) {
			if (state[i] == PLACEHOLDERS[0] || state[i] == PLACEHOLDERS[1] || boardID[i] == state[i]) {
				continue;
			}

			match = false;
		}

		return match;
	}
}