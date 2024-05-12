//
// FILENAME: treeutils.c | Shifting Stones Search
// DESCRIPTION: Utilities for creating and modifying trees
// CREATED: 2024-02-09 @ 10:38 PM
//

#include "treeutils.hpp"

namespace treeutils {
	/**
	 * @brief Modify the board state
	 * 
	 * @param 	board 	The board to modify 
	 * @param 	perm 	The permutation (state change) to apply 
	 * @return 			A new board with the given state change 
	 * 
	 * @note
	 * Permutations are as follows:
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
	 * @brief 
	 * 
	 * @param tree 
	 * @return std::vector<int> 
	 */
	std::vector<int> makeMoveSet(const tree_t tree, int startIndex) {
		std::vector<int> moveSet;
		moveSet.reserve(TREE_GEN_HEIGHT);
		moveSet.push_back(startIndex);

		while (startIndex != 0) {
			startIndex = (startIndex - 1) / CHILDREN_PER_PARENT;
			moveSet.push_back(startIndex);
		}

		return moveSet;
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
		*board ^= 1 << (24 - BOARD_LEN * tile);
	}

	/**
	 * @brief Store the tree's binary data in a file
	 * 
	 * @param 	tree 	The tree to store 
	 */
	void storeTree(const tree_t tree) {
		FILE* treefile = fopen("tree.bin", "wb");
		
		for (board_t i = 0, board = BOARD_IDX(tree, i); board != 0; board = BOARD_IDX(tree, ++i)) {
			fwrite(&board, sizeof(board), 1, treefile);
		}

		fclose(treefile);
	}

	/**
	 * @brief Determine if a given board configuration is valid
	 * 
	 * @param 	board 	The board to check
	 * @return 			The index of `board` if found, `-1` otherwise
	 * 
	 * @note 			The board is found using binary search
	 */
	int isValidBoardState(board_t board) {
		int min = 0, max = MAX_BOARD_STATES - 1, mid;

		while (min <= max) {
			mid = (min + max) / 2;

			if (BOARD_STATES[mid] < board) { 		// Guessed too low
				min = mid + 1;
			}
			else if (BOARD_STATES[mid] > board) { 	// Guessed too high
				max = mid - 1;
			}
			else {									// Guessed correctly
				return mid;
			}
		}

		return -1;
	}

	std::tuple<board_t, std::vector<int>> search(const tree_t tree, const std::string& target) {
		const int TREE_NODES = TREE_NODES_COUNT(CHILDREN_PER_PARENT, TREE_GEN_HEIGHT);
		//std::vector<__detail::__tree_node> nodes(TREE_NODES);
		//nodes[0] = {0, 0, 0}; // Initialize the root node

		//int current = 0; // Index of the current node being processed

		board_t board;

		for (int i = 0; i < TREE_NODES; i++) {
			board = BOARD_IDX(tree, i);

			int index = isValidBoardState(board);
			if (index == -1 || used[index]) {
				continue;
			}

			used[index] = true; // Mark the board as used
			
			std::cout << board << "\n";
			std::cout << success_states::isSuccessState(board, target) << "\n";

			//std::cout << "Valid: " << (index != -1) << "\n";

			if (success_states::isSuccessState(board, target)) {
				std::cout << "Found\n";
				return std::make_tuple(board, makeMoveSet(tree, index));
			}
		}

		return std::make_tuple(0, std::vector<int>{});

		// while (current < nodes.size()) {
		// 	auto [height, parent, child, _] = nodes[current];
		// 	const int PARENT_INDEX = CHILDREN_PER_PARENT * parent + child;

		// 	board_t board;
		// 	if (board = BOARD(tree, parent, child); board != 0 && success_states::isSuccessState(board, target)) {
				
		// 	}

			// for (int i = 1; i <= CHILDREN_PER_PARENT; i++) {
			// 	if (CHILDREN_PER_PARENT * current + i < nodes.size()) {
			// 		nodes[CHILDREN_PER_PARENT * current + i] = {height + 1, PARENT_INDEX, i};
			// 	}
			// }

		// 	current++;
		// }

		return std::make_tuple(0, std::vector<int>{});

		// std::queue<__detail::__tree_node> nodes;
		// nodes.push({0, 0, 0}); // Add the root node

		// while (!nodes.empty()) {
		// 	auto node = nodes.front();
		// 	auto [height, parent, child, _] = node;
		// 	nodes.pop();

		// 	board_t board;
		// 	if (board = BOARD(tree, parent, child); board != 0 && success_states::isSuccessState(board, target)) {
		// 		return std::make_tuple(board, node.makeMoveSet(tree));
		// 	}

		// 	const int PARENT_INDEX = CHILDREN_PER_PARENT * parent + child;

		// 	// Add that node's children to the queue
		// 	for (int i = 1; i <= CHILDREN_PER_PARENT; i++) {
		// 		nodes.push({height + 1, PARENT_INDEX, i});
		// 	}
		// }

		// return std::make_tuple(0, std::vector<int>{});
	}

	void buildTree(tree_t tree, board_t board, int height, int parent) {
		static int count = 1; // The root has already been allocated, so we start at 1
		const int PARENT_INDEX = CHILDREN_PER_PARENT * parent;
		
		if (height > TREE_GEN_HEIGHT) {
			return;
		}

		//parent -= (parent == height);

		for (int i = 1; i <= CHILDREN_PER_PARENT; i++) {
			//printf("Parent: %d | Height: %d | Count: %d\n", parent, height, count);
			//printf("Allocating: %lu(%d) + %d = %lu\n", CHILDREN_PER_PARENT, parent, i, CHILDREN_PER_PARENT * parent + i);

			// Store the next board permutation.
			BOARD(tree, parent, i) = __permuteBoard(board, i);
			count++;

			//printf("%d: %s\n", PARENT_INDEX + i, getBits(BOARD(tree, parent, i), USABLE_BOARD));

			// Continue building the tree recursively
			buildTree(tree, BOARD(tree, parent, i), height + 1, PARENT_INDEX + i);
		}
	}

	tree_t buildTree(board_t initialBoard) {
		// Create a tree
		const int TREE_NODES = TREE_NODES_COUNT(CHILDREN_PER_PARENT, TREE_GEN_HEIGHT);
		const size_t SIZEOF_TREE = TREE_NODES * sizeof(board_t);
		tree_t tree = malloc(SIZEOF_TREE);

		BOARD(tree, 0, 0) = initialBoard; // Store the root node
		//printf("Root: %s\n", getBits(initialBoard, 27));
		
		int height = 1; // The starting tree height
		int parent = 0; // The index of the parent of the first recursively generated row

		buildTree(tree, initialBoard, height, parent); // Recursively build the tree

		return tree;
	}

	/**
	 * @brief Build
	 * 
	 * @param 	initialBoard 	The initial board state
	 * @return 					The fully generated tree for that intitial board state
	 */
	board_t findSuccessState(board_t initialBoard, const std::string& successState) {
		// const int TREE_NODES = (pow(CHILDREN_PER_PARENT, TREE_GEN_HEIGHT + 1) - 1) / (CHILDREN_PER_PARENT - 1); // The number of nodes in the tree
		// const size_t SIZEOF_TREE = TREE_NODES * SIZEOF_BOARD / 8; // The size of the tree in bytes
		// tree_t tree = malloc(SIZEOF_TREE); // Create a tree
		// int count = 0, height = 0;

		// std::queue<__detail::__tree_node> nodes;
		// nodes.push({0, 0, 0, initialBoard}); // Store the root node
		// int nodesStored = 0;

		// // buildTree(tree, nodes);
		// // return tree;

		// while (!nodes.empty() && nodes.front().height <= TREE_GEN_HEIGHT) {
		// 	// Add the next node in the queue to the graph
		// 	auto [height, parent, child, board] = nodes.front();
		// 	nodes.pop();
		// 	const int PARENT_INDEX = CHILDREN_PER_PARENT * parent + child;

		// 	//std::cout << "Height: " << height << " | Parent: " << parent << " | Child: " << child << "\n";
		// 	int index = isValidBoardState(board);

		// 	if (index != -1 && !used[index]) {
		// 		//printf("Allocated board: %d | Index: %d | Used: %d\n", board, index, used[index]);
		// 		BOARD(tree, parent, child) = board;
		// 		used[index] = true;
		// 	}
		// 	else {
		// 		//printf("Skipped board: %d | Index: %d | Used: %d\n", board, index, used[index]);
		// 		BOARD(tree, parent, child) = 0;
		// 		//exit(0);
		// 	}

		// 	for (int i = 1; i <= CHILDREN_PER_PARENT; i++) {
		// 		nodes.push({height + 1, PARENT_INDEX, i, __permuteBoard(board, i)});
		// 		count++;
		// 	}

		const int TREE_NODES = TREE_NODES_COUNT(CHILDREN_PER_PARENT, TREE_GEN_HEIGHT);
		const size_t SIZEOF_TREE = TREE_NODES * SIZEOF_BOARD / 8;
		tree_t tree = malloc(SIZEOF_TREE);

		// Pre-allocate memory for the entire tree
		//std::vector<__detail::__tree_node> nodes(TREE_NODES);
		const std::size_t MAX_SIZE = TREE_LEAVES_COUNT(CHILDREN_PER_PARENT, TREE_GEN_HEIGHT);
		//std::cout << TREE_LEAVES_COUNT(CHILDREN_PER_PARENT, TREE_GEN_HEIGHT) << "\n";
		//std::vector<board_t> nodes(MAX_SIZE), oldNodes(MAX_SIZE);
		void* row = malloc(MAX_SIZE * SIZEOF_BOARD / 8);
		void* newRow = malloc(MAX_SIZE * SIZEOF_BOARD / 8);
		//memset(nodes, 0, sizeof(nodes));
	
		// for (board_t i = 0, board = BOARD_IDX(tree, i); board != 0; board = BOARD_IDX(tree, ++i)) {
		// 	fwrite(&board, sizeof(board), 1, treefile);
		// }

		// Initialize the root node
		BOARD_IDX(row, 0) = initialBoard; //{0, 0, 0, initialBoard};

		int rowSize = 1;
		int newRowSize = 0;

		board_t targetBoard = 0;
		int rowIndex = 0;

		while (rowIndex < TREE_GEN_HEIGHT) {
			std::cout << rowIndex << "\n";

			for (size_t i = 0; i < rowSize; i++) {
				board_t board = BOARD_IDX(row, i);
				//std::cout << board << "\n";
				if (success_states::isSuccessState(board, successState)) {
					std::cout << getBits(board, 27) << "\n";
					return board;
				}

				std::cout << "Loading\n";
				for (int j = 1; j <= CHILDREN_PER_PARENT; j++) {
					BOARD(newRow, i, j) = __permuteBoard(board, j);
					newRowSize++;
				}
			}

			rowIndex++;

			rowSize = newRowSize;
			newRowSize = 0;

			memcpy(row, newRow, sizeof(newRow));
			memset(newRow, 0, sizeof(newRow));
			

			//auto [height, parent, child, board] = nodes[current];
			// const int PARENT_INDEX = CHILDREN_PER_PARENT * parent + child;

			// if (success_states::isSuccessState(board, successState)) {
			// 	std::cout << getBits(board, 27) << "\n";
			// 	targetBoard = board;
			// 	continue;
			// }

			// int index = isValidBoardState(board);

			// if (index != -1 && !used[index]) {
			// 	BOARD(tree, parent, child) = board;
			// 	used[index] = true;
			// } else {
			// 	BOARD(tree, parent, child) = 0;
			// }

			// for (int i = 1; i <= CHILDREN_PER_PARENT; i++) {
			// 	if (current * CHILDREN_PER_PARENT + i < nodes.size()) {
			// 		nodes[current * CHILDREN_PER_PARENT + i] = {height + 1, PARENT_INDEX, i, __permuteBoard(board, i)};
			// 	}
			// }

			//current++;
		}

		free(tree);
		return targetBoard;
	}
}