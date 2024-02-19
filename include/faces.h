//
// FILENAME: faces.h | Shifting Stones Search
// DESCRIPTION: Represent each of the faces of the board tiles
// CREATED: 2024-02-06 @ 4:43 PM
//

#ifndef SS_SEARCH_FACES_H
#define SS_SEARCH_FACES_H

#ifdef __cplusplus
	#include "decl.h"

	enum class Faces: tile_t {
		Sun = 0, Moon, Fish, Bird,
		Horse, Boat, Seed, Tree
	};
#else
	enum Faces {
		Sun = 0, Moon, Fish, Bird,
		Horse, Boat, Seed, Tree
	};
#endif

#endif // SS_SEARCH_FACES_H