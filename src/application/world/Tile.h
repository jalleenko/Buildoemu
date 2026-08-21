#pragma once
#include "PlatformSetup.h"

enum eTileFlags : int
{
    TILE_FLAG_HAS_EXTRA_DATA = 0x1,
    TILE_FLAG_HAS_PARENT = 0x2,
    TILE_FLAG_WAS_SPLICED = 0x4,
    TILE_FLAG_WILL_SPAWN_SEEDS_TOO = 0x8,
    TILE_FLAG_IS_SEEDLING = 0x10,
    TILE_FLAG_FLIPPED_X = 0x20,
    TILE_FLAG_IS_ON = 0x40
};

class World;
class TileExtra;
class Tile
{
public:
    void Serialize(uint8* buffer, int & offsetInOut, bool write);
    void SetItemID(int itemID) { m_itemID= (uint16)itemID;}
    void SetBG(int itemID) { m_itemBGID= (uint16)itemID;}

    void Set(short type, short visual, World* pWorld);

private:
	uint16 m_itemID = 0;
	uint16 m_itemBGID = 0;
	uint16 m_flags = 0;
    uint16 m_parent = 0;
    TileExtra* m_pTileExtra = 0;
};