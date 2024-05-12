//
// FILENAME: decl.h | Shifting Stones Search
// DESCRIPTION: Global declarations for the C API
// CREATED: 2024-02-06 @ 9:47 AM
//

#ifndef SS_SEARCH_DECL_H
#define SS_SEARCH_DECL_H

#include <stdint.h>
#include <math.h>

#if defined(__cplusplus) && defined(__USE_DECL_NAMESPACE)
namespace decl {
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A Shifting Stones board
 */
typedef uint32_t board_t;

/**
 * @brief A board tile
 */
typedef uint8_t tile_t;

/**
 * @brief A tree
 */
typedef void* tree_t;

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
 * @brief The maximum number of possible board states needed to completely fill a height 5 tree
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
 * @brief The maximum height the tree is allowed to generate to
 */
#define TREE_GEN_HEIGHT 6UL

/**
 * @brief A function macro to get a usable board out of the tree buffer given a parent and child
 * 
 * @param 	__tree 		A pointer to the tree
 * @param 	__parent 	The index of the parent in the tree (0 - 122879)
 * @param 	__child 	The index of the child within that parent (0 - 21)
 * 
 * @note 				0 should not be passed for `__child` unless the root node is being accessed.
 * 						Otherwise, the behavior is undefined.
 * 
 * @note 				The root node can be accessed with `BOARD(__tree, 0, 0)`
 */
#define BOARD(__tree, __parent, __child) *((board_t*)(__tree) + CHILDREN_PER_PARENT * __parent + __child)

/**
 * @brief A function macro to get a usable board out of the tree buffer given a memory offset
 * 
 * @param 	__tree 		A pointer to the tree
 * @param 	__index 	A memory offset to add to the tree pointer
 */
#define BOARD_IDX(__tree, __index) *((board_t*)(__tree) + __index)

#define TREE_NODES_COUNT(__children_per_parent, __tree_gen_height) ((unsigned)(pow(__children_per_parent, __tree_gen_height + 1) - 1) / (__children_per_parent - 1))

#define TREE_LEAVES_COUNT(__children_per_parent, __tree_gen_height) TREE_NODES_COUNT(__children_per_parent, __tree_gen_height) - TREE_NODES_COUNT(__children_per_parent, __tree_gen_height - 1)

#ifdef __cplusplus
} // extern "C"
#endif

#if defined(__cplusplus) && defined(__USE_DECL_NAMESPACE)
} // namespace decl
#endif

#endif // SS_SEARCH_DECL_H