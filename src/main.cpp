//
// FILENAME: main.cpp | Shifting Stones Search
// DESCRIPTION: Command line interface for interacting with the tree API
// CREATED: 2024-03-12 @ 9:38 PM
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <functional>

#include "faces.h"
#include "treeutils.h"
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
	board_t board = generateBoard();
	tree_t tree = treeutils::buildTree(board);
	treeutils::storeTree(tree);

	TreeGraph<>* graph = new TreeGraph(tree);
	graph->writeGVDOT("tree.gv");

	free(tree);
	delete graph;
}