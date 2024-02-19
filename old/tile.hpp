//
// FILENAME: tile.hpp | Shifting Stones Search
// DESCRIPTION: A basic representation of a game tile
//

#pragma once

enum class TileType {SunMoon, FishBird, HorseBoat, SeedTree};

struct Tile {
	TileType m_TileType;
	bool 	 m_IsFlipped;
};