//
// FILENAME: main.c | Shifting Stones Search
// DESCRIPTION: Main function
// CREATED: 2024-02-05 @ 8:30 PM
//

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "decl.h"
#include "faces.h"
#include "treeutils.h"

// unsigned int27_t* getByte(void* address, unsigned int index) {
// 	return (unsigned int27_t*)address + index;
// }

board_t generateBoard() {
	board_t board = 0;
	srand(time(NULL));

	// Store the possible tile choices. Flips are pre-calculated.
	const tile_t TILE_CHOICES[] = {
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
	// const int BYTES = MAX_BOARD_STATES * SIZEOF_BOARD / 8;
	// void* tree = malloc(BYTES);
	// uint64_t* realTree = (uint64_t*)tree;

	// struct timespec start, end;
	// clock_gettime(CLOCK_MONOTONIC, &start);
		
    // clock_gettime(CLOCK_MONOTONIC, &end);

    // // Calculate the elapsed time in nanoseconds
    // long long elapsed_time_ns = (end.tv_sec - start.tv_sec) * 1000000000LL; // Convert seconds to nanoseconds
    // elapsed_time_ns += end.tv_nsec - start.tv_nsec; // Add remaining nanoseconds
	// printf("%lld ns\n", elapsed_time_ns);

	board_t board = generateBoard();
	void* tree = buildTree(board);
	storeTree(tree);

	// for (int i = 0; i < 22; i++) {
	// 	printf("%d: %s\n", i, getBits(BOARD(0, i), 27));
	// }
	
	// board_t* b = getBoard(tree, 1, 0);
	// b = &board;

	//printf("Index 22: %s\n", getBits(*getBoard(tree, 1, 0), 32));
	//tree[4]
	//*((board_t*)addr) |= (uint64_t)(board) << 32;

	//printf("Tree: %s\n", getBits(*tree, 64));

	//board = getBoard(addr, 0, 0);

	// printf("Board: %s\n", getBits(board, 27));

	// flipTile(&board, 0);
	// printf("Flipped first tile: %s\n", getBits(board, 27));

	// printf("Value at top 3 bits: %u\n", (board & (0b111 << 24)) >> 24);

	// printf("Before:\n");
	// for (int i = 26; i >= 0; i--) {
	// 	printf("%d", (board & (1 << i)) >> i);
	// }
	// printf("\n");

	// swapTiles(&board, 0, 1);
	// //printf("Board: %u\n", board);

	// printf("After:\n");
	// for (int i = 26; i >= 0; i--) {
	// 	printf("%d", (board & (1 << i)) >> i);
	// }
	// printf("\n");

	// printf("Value at top 3 bits: %u\n", (board & (0b111 << 24)) >> 24);
	// printf("Value at next 3 bits: %u\n", (board & (0b111 << 21)) >> 21);

	free(tree);
}