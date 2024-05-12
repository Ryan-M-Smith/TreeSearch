//
// FILENAME: main.cpp | Shifting Stones Search
// DESCRIPTION: Command line interface for interacting with the tree API
// CREATED: 2024-03-12 @ 9:38 PM
//

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <functional>

#include "faces.h"
#include "treeutils.hpp"
#include "successstates.hpp"
#include "treegraph.hpp"

board_t generateBoard() {
	board_t board = 0;
	srand(time(nullptr));

	// Store the possible tile choices. Flips are pre-calculated.
	const Faces TILE_CHOICES[] = {
		(rand() % 2)? Moon : Sun,
		(rand() % 2)? Bird : Fish,
		(rand() % 2)? Bird : Fish,
		(rand() % 2)? Tree : Seed,
		(rand() % 2)? Tree : Seed,
		(rand() % 2)? Tree : Seed,
		(rand() % 2)? Boat : Horse,
		(rand() % 2)? Boat : Horse,
		(rand() % 2)? Boat : Horse,
	};

	//
	// Storing visited board indices
	//
	// Rather than looping over an array of visited incides to help determine if an index can
	// be used or not, visited board indices can be stored in a bit field. Every time an index
	// is chosed to be loaded into the board, or "visited", a 1 gets loaded into the bit
	// corresponding to the index of the tile in the array. This way, we can check if an index
	// was visited by masking out the bit at any given index and seeing if it's set or not.
	//
	// The visited bit field is stored in 16 bits, but only the first 9 bits are used.
	//

	const size_t NUM_TILES = 9;
	uint16_t visited = 0;

    for (size_t i = 0; i < NUM_TILES; i++)  {
        uint8_t index;
		
		// Pick a new value from the possible tile choices. Each tile can only be chosen once.
		do {
			index = rand() % NUM_TILES;
		} while ((bool)((visited & (1 << index)) >> index)); // If the index was already used, choose another

		// Mark the index as visited
		visited |= 1 << index;

		//
		// Load the tile onto the board. Board tiles are stored with tile (0, 0) being at the highest
		// place values (bits 24 - 27) and tile (2, 2) being at the lowest place value (bits 1 - 3).
		// Thus, 
		//
		board |= (TILE_CHOICES[index] << (24 - i * 3));
    }

	return board;
}

int main() {
	//std::cout << success_states::getID(0b101100111001101110010001100) << "\n";

	board_t board = 0b100100101101011100100011000;//generateBoard();

	auto start = std::chrono::high_resolution_clock::now();
	//board_t targetBoard = treeutils::findSuccessState(board, "828878848");
	// auto [result, index] = treeutils::isValidBoardState(board);
	//auto [successState, moveSet] = treeutils::search()
	tree_t tree = treeutils::buildTree(board);
	std::cout << BOARD_IDX(tree, 13) << "\n";
	auto end = std::chrono::high_resolution_clock::now();

	auto duration = duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Generation completed in " << duration.count() << " ms\n";

	//std::cout << success_states::isSuccessState(93731096, "548888888") << "\n";
	//treeutils::storeTree(tree);

	auto&& [successState, moveSet] = treeutils::search(tree, "548888888");
	std::cout << treeutils::getBits(successState, 27) << "\n";

	//std::cout << "Result: " << treeutils::getBits(targetBoard, 27) << "\n";

	//auto [successState, moveSet] = treeutils::search(tree, "828878848");
	//std::cout << successState << "\n";
	//std::cout << "\nResult: " << successState << "\n";
	
	// //std::cout << "Generated a height " << TREE_GEN_HEIGHT << " tree in " << duration.count() << " ms\n";

	// TreeGraph<>* graph = new TreeGraph(tree);
	// graph->writeGVDOT("tree.gv");

	free(tree);
	// delete graph;
}