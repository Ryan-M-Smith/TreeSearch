//
// FILENAME: decl.h | Shifting Stones Search
// DESCRIPTION: 27-bit fixed-wdth integers
// CREATED: 2024-02-06 @ 9:47 AM
//

#ifndef SS_SEARCH_DECL_H
#define SS_SEARCH_DECL_H

#include <stdint.h>

/**
 * @brief A Shifting Stones board
 */
typedef uint32_t board_t;

/**
 * @brief A board tile
 */
typedef uint8_t tile_t;

/**
 * @brief The number of bits required to store a single board state
 */
#define SIZEOF_BOARD 32UL

/**
 * @brief The actual allocated board space in any given 32-bit board
 */
#define USABLE_BOARD 27UL

/**
 * @brief The maximum allocatable board value
 * @note  Equal to 2^27 - 1
 */
#define UINT27_MAX 134217727UL

/**
 * @brief The maximum allocatable board tile
 * @note  Equal to 2^8 - 1
 */
#define UINT3_MAX 7UL

/**
 * @brief The number of bits required to store a single tile
 */
#define SIZEOF_TILE 8UL

/**
 * @brief The actual allocated tile space in any given 8-bit tile
 */
#define USABLE_TILE 3UL

/**
 * @brief The maximum number of possible board states
 * 
 * @note  See https://grievous667.github.io/proofs/board-configs for calculations
 */
#define MAX_BOARD_STATES 2580480UL

/**
 * @brief The number of rows and columns in the board
 */
#define BOARD_LEN 3UL

/**
 * @brief The number of children each node in the tree has
 */
#define CHILDREN_PER_PARENT 21UL

/**
 * @brief The total number of possible board configs that can be generated from any given board
 */
#define POSSIBLE_CONFIGS 21UL

/**
 * @brief A function macro to get a usable board out of the tree buffer
 * 
 * @param 	__tree 		A pointer to the tree
 * @param 	__parent 	The index of the parent in the tree
 * @param 	__child 	The index of the child within that parent (1 - 21)
 * 
 * @note 				If 0 is passed for `__child`, the parent, or local root node, is returned
 */
#define BOARD(__tree, __parent, __child) *((board_t*)(__tree) + ((CHILDREN_PER_PARENT + 1) * __parent) + __child)

#endif // SS_SEARCH_DECL_H