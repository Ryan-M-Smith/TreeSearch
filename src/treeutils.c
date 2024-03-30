//
// FILENAME: treeutils.c | Shifting Stones Search
// DESCRIPTION: Utilities for creating and modifying trees
// CREATED: 2024-02-09 @ 10:38 PM
//

#include <math.h>
#include <assert.h>

#include "treeutils.h"

#ifdef __cplusplus
namespace treeutils {
#endif

#ifndef __cplusplus
	// External definitions for inline functions
	extern inline tile_t getTile(board_t board, int tile);
	extern inline board_t getBoard(tree_t tree, int parent, int child);
#endif

/**
 * @brief Build
 * 
 * @param 	initialBoard 	The initial board state
 * @return 					The fully generated tree for that intitial board state
 */
tree_t buildTree(board_t initialBoard) {
	const int TREE_NODES = (pow(CHILDREN_PER_PARENT, TREE_GEN_HEIGHT + 1) - 1) / (CHILDREN_PER_PARENT - 1); // The number of nodes in the tree
	const size_t SIZEOF_TREE = TREE_NODES * SIZEOF_BOARD / 8; // The size of the tree in bytes
	tree_t tree = malloc(SIZEOF_TREE); // Create a tree

	search_results results = isValidBoardState(initialBoard);
	
	// The root must be able to be allocated, otherwise generation cannot continue
	assert(results.result && !used[results.index]);

	BOARD(tree, 0, 0) = initialBoard; // Store the root node
	
	int height = 1; // The starting tree height
	int parent = 0; // The index of the parent of the first recursively generated row

	__buildTree(tree, initialBoard, height, parent); // Recursively build the tree

	return tree;
}

/**
 * @brief Recursively build a tree of board states
 * 
 * @param 	tree 	A pointer to the tree memory 	
 * @param 	board 	The board to generate permutations of
 * @param 	height 	The current height nodes are being generated at
 * @param 	parent 	The index of the parent node for the set of nodes being generated
 * 
 * @note 			This function acts directly on `tree` rather than returning a result
 */
void __buildTree(tree_t tree, board_t board, int height, int parent) {
	static int count = 1; // The root has already been allocated, so we start at 1
	
	const int PARENT_INDEX = CHILDREN_PER_PARENT * parent;
	
	if (height > TREE_GEN_HEIGHT) {
		return;
	}

	for (int i = 1; i <= CHILDREN_PER_PARENT; i++) {
		//printf("Parent: %d | Height: %d | Count: %d\n", parent, height, count);
		//printf("Allocating: %lu(%d) + %d = %lu\n", CHILDREN_PER_PARENT, parent, i, CHILDREN_PER_PARENT * parent + i);

		// Store the next board permutation
		
		board_t nextBoard = __permuteBoard(board, i);

		search_results results = isValidBoardState(nextBoard);

		BOARD(tree, parent, i) = (results.result && !used[results.index])? nextBoard: 0;
		used[results.index] = true;
		count++;

		//printf("%d: %s\n", PARENT_INDEX + i, getBits(BOARD(tree, parent, i), USABLE_BOARD));

		// Continue building the tree recursively
		__buildTree(tree, BOARD(tree, parent, i), height + 1, PARENT_INDEX + i);
	}

	//printf("height: %d\n", height);
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
	tile_t boardTile = getTile(*board, tile); // The tile to be flipped
	int sign = (boardTile % 2 == 0)? 1 : -1;  // Subtract 1 from odd tiles, add one to even tiles

	// A board mask with the tile to flip zeroed out
	board_t mask = *board & ~(UINT3_MAX << (24 - BOARD_LEN * tile));

	*board = mask | ((boardTile + sign) << (24 - BOARD_LEN * tile));
}

/**
 * @brief Store the tree's binary data in a file
 * 
 * @param 	tree 	The tree to store 
 */
void storeTree(const void* tree) {
	FILE* treefile = fopen("tree.bin", "wb");
	
	for (board_t i = 0, board = BOARD_IDX(tree, i); board != 0; board = BOARD_IDX(tree, ++i)) {
		//printf("%s\n", getBits(board, 27));
		fwrite(&board, sizeof(board), 1, treefile);
	}

	fclose(treefile);
}

#ifdef __cplusplus
} // namespace treeutils
#endif