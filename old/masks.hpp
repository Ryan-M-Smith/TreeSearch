//
// FILENAME: masks.hpp | Shifting Stones Search
// DESCRIPTION: Objects that represent a change on the game board
//

#pragma once

#include <cstdlib>

enum class MaskType: uint8_t {Row, Column, Flip};

struct Mask {
	bool 	 	m_Mask[3];
	std::size_t m_Index;
	MaskType 	m_MaskType;
};

namespace masks {
	constexpr static int TOTAL_MASKS = 21;

	const static Mask LEFT_SWAP_TOP  = {{0, 0, 1}, 0, MaskType::Row};
	const static Mask RIGHT_SWAP_TOP = {{1, 0, 0}, 0, MaskType::Row};
	const static Mask LEFT_SWAP_MID  = {{0, 0, 1}, 1, MaskType::Row};
	const static Mask RIGHT_SWAP_MID = {{1, 0, 0}, 1, MaskType::Row};
	const static Mask LEFT_SWAP_BTM  = {{0, 0, 1}, 2, MaskType::Row};
	const static Mask RIGHT_SWAP_BTM = {{1, 0, 0}, 2, MaskType::Row};

	const static Mask TOP_SWAP_LEFT  = {{0, 0, 1}, 0, MaskType::Column};
	const static Mask BTM_SWAP_LEFT  = {{1, 0, 0}, 0, MaskType::Column};
	const static Mask TOP_SWAP_MID   = {{0, 0, 1}, 1, MaskType::Column};
	const static Mask BTM_SWAP_MID   = {{1, 0, 0}, 1, MaskType::Column};
	const static Mask TOP_SWAP_RIGHT = {{0, 0, 1}, 2, MaskType::Column};
	const static Mask BTM_SWAP_RIGHT = {{1, 0, 0}, 2, MaskType::Column};

	const static Mask LEFT_FLIP_TOP  = {{0, 1, 1}, 0, MaskType::Flip};
	const static Mask MID_FLIP_TOP   = {{1, 0, 1}, 0, MaskType::Flip};
	const static Mask RIGHT_FLIP_TOP = {{1, 1, 0}, 0, MaskType::Flip};

	const static Mask LEFT_FLIP_MID  = {{0, 1, 1}, 1, MaskType::Flip};
	const static Mask MID_FLIP_MID   = {{1, 0, 1}, 1, MaskType::Flip};
	const static Mask RIGHT_FLIP_MID = {{1, 1, 0}, 1, MaskType::Flip};

	const static Mask LEFT_FLIP_BTM  = {{0, 1, 1}, 2, MaskType::Flip};
	const static Mask MID_FLIP_BTM   = {{1, 0, 1}, 2, MaskType::Flip};
	const static Mask RIGHT_FLIP_BTM = {{1, 1, 0}, 2, MaskType::Flip};
}