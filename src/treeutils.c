//
// FILENAME: bitutils.c | Shifting Stones Search
// DESCRIPTION: Utilities for working with board binary data
// CREATED: 2024-02-09 @ 10:38 PM
//

#include "treeutils.h"

// External definitions for inline functions
extern inline tile_t getTile(board_t board, int tile);
extern inline board_t* getBoard(void* tree, int parent, int child);

void* __buildTree(void* tree, board_t board, int height, int parent, bool atChild) {
	const int PARENT_INDEX = (CHILDREN_PER_PARENT + 1) * parent;

	//printf("Parent index: %d\n", PARENT_INDEX);

	if (height > 1) {
		return tree;
	}

	for (int i = atChild; i < CHILDREN_PER_PARENT + atChild; i++) {
		printf("Parent: %d Height: %d\n", parent, height);

		// Store the next board permutation.
		BOARD(tree, PARENT_INDEX, i - atChild) = __permuteBoard(board, i);

		printf("%d: %s\n", PARENT_INDEX + i - atChild, getBits(BOARD(tree, PARENT_INDEX, i - atChild), USABLE_BOARD));
		
		//
		// Continue building the tree recursively. Local root nodes cannot be used in recursive calls as
		// the tree cannot continue to be built until the local root has at least one child, at which point
		// that child's children (the local root's grandchildren) can be built.
		//
		if (i > 0) {
			//parent += (PARENT_INDEX + i - atChild) % 22 == 0;
			//height += (PARENT_INDEX + i - atChild) % 22 == 0;

			__buildTree(tree, BOARD(tree, PARENT_INDEX, i), height + 1, i, true);
		}
	}
}

/**
 * @brief Modify the board state
 * 
 * @param 	board 	The board to modify 
 * @param 	perm 	The permutation (state change) to apply 
 * @return 			A new board with the given state change 
 * 
 * @note
 * Permutations are as follows:
 * 	- 0:  No change
 * 	- 1:  Swap top left and top middle
 * 	- 2:  Swap top middle and top right
 * 	- 3:  Swap middle left and center
 * 	- 4:  Swap center and middle right
 * 	- 5:  Swap bottom left and bottom middle
 * 	- 6:  Swap bottom middle and bottom right
 * 	- 7:  Swap top left and middle left
 * 	- 8:  Swap middle left and bottom left
 * 	- 9:  Swap middle top and center
 * 	- 10: Swap center and bottom middle
 * 	- 11: Swap top right and middle right
 * 	- 12: Swap middle right and bottom right
 * 	- 13: Flip top right
 * 	- 14: Flip top middle
 * 	- 15: Flip top right
 * 	- 16: Flip middle left
 * 	- 17: Flip center
 * 	- 18: Flip middle right
 * 	- 19: Flip bottom left
 * 	- 20: Flip bottom middle
 * 	- 21: Flip bottom right
 */
board_t __permuteBoard(board_t board, int perm) {
	board_t* newBoard = &board;

	switch (perm) {
		case 1:
			swapTiles(newBoard, 0, 1);
			break;
		case 2:
			swapTiles(newBoard, 1, 2);
			break;
		case 3:
			swapTiles(newBoard, 3, 4);
			break;
		case 4:
			swapTiles(newBoard, 4, 5);
			break;
		case 5:
			swapTiles(newBoard, 6, 7);
			break;
		case 6:
			swapTiles(newBoard, 7, 8);
			break;
		case 7:
			swapTiles(newBoard, 0, 3);
			break;
		case 8:
			swapTiles(newBoard, 3, 6);
			break;
		case 9:
			swapTiles(newBoard, 1, 4);
			break;
		case 10:
			swapTiles(newBoard, 4, 7);
			break;
		case 11:
			swapTiles(newBoard, 2, 5);
			break;
		case 12:
			swapTiles(newBoard, 5, 8);
			break;
		case 13:
			flipTile(newBoard, 0);
			break;
		case 14:
			flipTile(newBoard, 1);
			break;
		case 15:
			flipTile(newBoard, 2);
			break;
		case 16:
			flipTile(newBoard, 3);
			break;
		case 17:
			flipTile(newBoard, 4);
			break;
		case 18:
			flipTile(newBoard, 5);
			break;
		case 19:
			flipTile(newBoard, 6);
			break;
		case 20:
			flipTile(newBoard, 7);
			break;
		case 21:
			flipTile(newBoard, 8);
			break;
		default:
			break;
	}

	return board;
}

void* buildTree(board_t initialBoard) {
	// Data points for recursion and board storage
	int height = 0, parent = 0;
	bool atChild = false;

	const int BYTES = MAX_BOARD_STATES * SIZEOF_BOARD / 8;
	void* tree = malloc(BYTES); // Create a tree

	__buildTree(tree, initialBoard, height, parent, atChild); 

	return tree;
}

/**
 * @brief Swap two board tiles
 * 
 * @param 	board 	The board to swap tiles on
 * @param 	tile1 	The index of the first tile (0 - 8)
 * @param 	tile2 	The index of the second tile (0 - 8)
 * 
 * @note 			The order of the tiles doesn't matter
 * @note 			This function acts directly on `board` rather than returning a result
 */
void swapTiles(board_t* board, int tile1, int tile2) {
	// The locations of the three bits of the first tile
	const uint8_t TOP_BIT_FIRST 	= (USABLE_BOARD - 1) - BOARD_LEN * tile1;
	const uint8_t MIDDLE_BIT_FIRST 	= (USABLE_BOARD - 2) - BOARD_LEN * tile1;
	const uint8_t BOTTOM_BIT_FIRST 	= (USABLE_BOARD - 3) - BOARD_LEN * tile1;

	// The locations of the three bits of the second tile
	const uint8_t TOP_BIT_SECOND 	= (USABLE_BOARD - 1) - BOARD_LEN * tile2;
	const uint8_t MIDDLE_BIT_SECOND = (USABLE_BOARD - 2) - BOARD_LEN * tile2;
	const uint8_t BOTTOM_BIT_SECOND = (USABLE_BOARD - 3) - BOARD_LEN * tile2;

	//
	// Swapping tiles
	//
	// Two bits can be swapped using xor operations when their values are different. If they're the same, the
	// swap is unnecessary.
	//
	// To swap any two given bits, we first check if they have different values. This is done by masking them out
	// and performing a bitwise xor. When an xor operation is done on two bits, the result is 1 if the bits are
	// different, 0 if they are the same.
	//
	// If the two bits are different, we can swap them. This is done by flipping each of the two bits individually.
	// Each bit to be swapped is xored with 1, thus flipping its value (1 ^ 1 = 0, 0 ^ 1 = 1). This process is
	// repeated for each bit in the sets of tiles.
	//

	// Swap the top two bits
	if (((*board & (1 << TOP_BIT_FIRST)) >> TOP_BIT_FIRST) ^ ((*board & (1 << TOP_BIT_SECOND)) >> TOP_BIT_SECOND)) {
		*board ^= 1 << TOP_BIT_FIRST;
		*board ^= 1 << TOP_BIT_SECOND;
	}

	// Swap the middle two bits
	if (((*board & (1 << MIDDLE_BIT_FIRST)) >> MIDDLE_BIT_FIRST) ^ ((*board & (1 << MIDDLE_BIT_SECOND)) >> MIDDLE_BIT_SECOND)) {
		*board ^= 1 << MIDDLE_BIT_FIRST;
		*board ^= 1 << MIDDLE_BIT_SECOND;
	}

	// Swap the bottom two bits
	if (((*board & (1 << BOTTOM_BIT_FIRST)) >> BOTTOM_BIT_FIRST) ^ ((*board & (1 << BOTTOM_BIT_SECOND)) >> BOTTOM_BIT_SECOND)) {
		*board ^= 1 << BOTTOM_BIT_FIRST;
		*board ^= 1 << BOTTOM_BIT_SECOND;
	}
}

/**
 * @brief Flip a tile on the board
 * 
 * @param 	board 	The board to filp a tile on
 * @param 	tile 	The index of the tile to flip (0 - 8) 
 */
void flipTile(board_t* board, int tile) {
	tile_t boardTile = getTile(*board, tile); // The tiler to be flipped
	int sign = (boardTile % 2 == 0)? 1 : -1;  // Subtract 1 from odd tiles, add one to even tiles

	// A board mask with the tile to flip zeroed out
	board_t mask = *board & (UINT27_MAX & ~(UINT3_MAX << (24 - BOARD_LEN * tile)));

	*board = mask | ((boardTile + sign) << (24 - BOARD_LEN * tile));
}