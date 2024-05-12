//
// FILENAME: treeutils.h | Shifting Stones Search
// DESCRIPTION: Utilities for creating and modifying trees
// CREATED: 2024-02-09 @ 10:38 PM
//

#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>

#include "boardstates.h"
#include "decl.h"
#include "successstates.hpp"

namespace treeutils {
	namespace __detail {
		/**
		 * @struct __tree_node
		 * @brief A basic data-only tree node
		 */
		struct __tree_node {
			int 	height; 	// The height the node is stored at
			int 	parent; 	// The index of the node's parent
			int 	child; 		// The index of the node under its parent
			board_t board = 0;	// The board data stored in the node

			std::vector<int> makeMoveSet(const tree_t tree) const {
				int _height = height;
				int nextIndex = CHILDREN_PER_PARENT * parent + child;
				std::vector<int> moveSet;

				moveSet.reserve(height);
				do {
					moveSet.push_back(nextIndex % 21 + (height != 0));
					nextIndex /= CHILDREN_PER_PARENT;
				} while (--_height >= 0);
				return moveSet;
			}
		};
	}

	board_t __permuteBoard(board_t board, int perm);
	std::vector<int> makeMoveSet(const tree_t tree, int startIndex);

	void swapTiles(board_t* board, int tile1, int tile2);
	void flipTile(board_t* board, int tile);

	void storeTree(const tree_t tree);

	int isValidBoardState(board_t board);

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

	std::tuple<board_t, std::vector<int>> search(const tree_t tree, const std::string& target);
	
	board_t findSuccessState(board_t initialBoard, const std::string& successState);
	
	tree_t buildTree(board_t initialBoard);
	void buildTree(tree_t tree, board_t board, int height, int parent);

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
	// void buildTree(tree_t tree, board_t board, int height, int parent) {
	// 	static int count = 1; // The root has already been allocated, so we start at 1
	// 	static int numUsed = 0;
		
	// 	const int PARENT_INDEX = CHILDREN_PER_PARENT * parent;
		
	// 	if (height > TREE_GEN_HEIGHT /*|| count == MAX_BOARD_STATES*/) {
	// 		//printf("%d\n", numUsed);
	// 		return;
	// 	}

	// 	for (int i = 1; i <= CHILDREN_PER_PARENT; i++) {
	// 		//printf("Parent: %d | Height: %d | Count: %d\n", parent, height, count);
	// 		//printf("Allocating: %lu(%d) + %d = %lu\n", CHILDREN_PER_PARENT, parent, i, CHILDREN_PER_PARENT * parent + i);

	// 		// Store the next board permutation
			
	// 		board_t nextBoard = __permuteBoard(board, i);
	// 		int index = isValidBoardState(nextBoard);

	// 		if (index != -1 && !used[index]) {
	// 			printf("Allocated board: %d | Index: %d | Used: %d\n", nextBoard, index, used[index]);
	// 			BOARD(tree, parent, i) = nextBoard;
	// 			numUsed++;
	// 			used[index] = true;
	// 		}
	// 		else {
	// 			printf("Skipped board: %d | Index: %d | Used: %d\n", nextBoard, index, used[index]);
	// 			BOARD(tree, parent, i) = 0;
	// 			//exit(0);
	// 		}

	// 		count++;

	// 		//printf("%d: %s\n", PARENT_INDEX + i, getBits(BOARD(tree, parent, i), USABLE_BOARD));

	// 		// Continue building the tree recursively
	// 		assert(nextBoard != 0);
	// 		buildTree(tree, nextBoard, height + 1, PARENT_INDEX + i);
	// 	}

	// 	//printf("height: %d\n", height);
	// }
}