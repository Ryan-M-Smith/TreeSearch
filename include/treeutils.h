//
// FILENAME: treeutils.h | Shifting Stones Search
// DESCRIPTION: Utilities for creating and modifying trees
// CREATED: 2024-02-09 @ 10:38 PM
//

#ifndef SS_SEARCH_TREEUTILS_H
#define SS_SEARCH_TREEUTILS_H

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "decl.h"
#include "boardstates.h"

#ifdef __cplusplus
namespace treeutils {
extern "C" {
#endif

tree_t buildTree(board_t initialBoard);

void __buildTree(tree_t tree, board_t board, int height, int parent);
board_t __permuteBoard(board_t board, int perm);

void swapTiles(board_t* board, int tile1, int tile2);
void flipTile(board_t* board, int tile);

void storeTree(const void* tree);

bool isValidBoardState(board_t board);

/**
 * @brief Get a specific board from the tree
 * 
 * @param 	tree 	The tree 
 * @param 	parent 	The index of the board's parent node 
 * @param 	child 	The index of the child 
 * @return 			The board 
 */
inline board_t getBoard(tree_t tree, int parent, int child) {
	assert(tree && parent >= 0 && (child >= 0 && child <= CHILDREN_PER_PARENT));
	return *((board_t*)(tree) + CHILDREN_PER_PARENT * parent + child);
}

/**
 * @brief Get a specific tile from a board
 * 
 * @param 	board 	A board
 * @param 	tile 	The index of the tile to return (0 - 8)
 * @return 			The tile 
 */
inline tile_t getTile(board_t board, int tile) {
	return (tile_t)((board & (0b111 << (24 - BOARD_LEN * tile))) >> (24 - BOARD_LEN * tile));
}

/**
 * @brief Return the binary representation of a number
 * 
 * @param 	value 	An integer to turn into binary 
 * @param 	size 	The number of bits to convert.
 * @return 			A string of bits
 * 
 * @note  			If size is -1, the size of the passed in integer is used
 */
const static char* const getBits(unsigned long value, size_t size) {
	size = (size == -1)? sizeof(value) : size;
	const char CHAR_ZERO = '0';

	char* buffer = (char*)malloc(size + 1); // Allocate space for the bits
	buffer[size] = '\0'; // Null-terminate the string

	// Load each bit into the string buffer, starting with the most significant bit loaded at index 0. The masked
	// out integer is added to the ASCII value for 0 to convert it to a char.
	for (int i = size - 1; i >= 0; i--) {
		buffer[size - i - 1] = ((value & 1 << i) >> i) + CHAR_ZERO;
	}

	return buffer;
}

#ifdef __cplusplus
} // namespace treeutils
} // extern "C"
#endif

#endif // SS_SEARCH_TREEUTILS_H