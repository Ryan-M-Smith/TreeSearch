#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <sys/mman.h>

#include "matrix.hpp"
#include "profiling.hpp"
#include "tile.hpp"
#include "masks.hpp"
#include "tree_node.hpp"

using namespace std::chrono;

// Board macros
#define __BOARD_BIT_SIZE 27UL
#define __MAX_BOARD_STATES 2580480UL

// Fixed-width 27-bit integets
#define int27_t _BitInt(__BOARD_BIT_SIZE)
#define board_t unsigned int27_t

int main() {}