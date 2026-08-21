#include "WorldTileMap.h"
#include "util/MiscUtils.h"
#include "util/ResourceUtils.h"

int WorldTileMap::GetMemEstimate()
{
	return (int)(m_tiles.size()) * 16 + 102400;
}

void WorldTileMap::Serialize(uint8* buffer, int& offsetInOut, bool write)
{
    MemorySerialize(m_sizeX, buffer, offsetInOut, write);
    MemorySerialize(m_sizeY, buffer, offsetInOut, write);

    int tileCount = (int)m_tiles.size();
    LogMsg("tileCount=%d", tileCount);
    MemorySerialize(tileCount, buffer, offsetInOut, write);

    for (auto& tile : m_tiles)
    {
        tile.Serialize(buffer, offsetInOut, write);
    }
}

void WorldTileMap::CreateWorld()
{
    m_sizeX = 80;
    m_sizeY = 60;

    m_tiles.resize(4800);

    // World generation from v0.92 `GameLogicComponent::CreateDummyWorldForTesting`

    // Bedrock frame, V0.01 allows you to walk out of map bounds.
    FillBorder(8);

    // Dirt & cave layer
    CL_Rect terrainRect(1, m_sizeY / 2, m_sizeX - 1, m_sizeY - 1);
    FillRect(terrainRect, 2, 14);

    // V1 generation had bedrock generate alongside rock.
    terrainRect.bottom = m_sizeY - 6;
    terrainRect.top += 1;
    FillRectRandom(terrainRect, 8, -1, 10);
    FillRectRandom(terrainRect, 10, -1, 20);

    // Fill in the bedrock at bottom
    terrainRect.top = m_sizeY - 6;
    terrainRect.bottom = m_sizeY - 1;
    FillRect(terrainRect, 8, -1);

    // Pack in some lava
    terrainRect.top = m_sizeY - 10;
    terrainRect.bottom = m_sizeY - 8;
    FillRectRandom(terrainRect, 4, -1, 30);

    terrainRect.top = m_sizeY - 8;
    terrainRect.bottom = m_sizeY - 6;
    FillRectRandom(terrainRect, 4, -1, 130);

    terrainRect.top = (m_sizeY / 2) - 1;
    terrainRect.bottom = terrainRect.top;
    FillRectRandom(terrainRect, 6, -1, 1);
}

void WorldTileMap::FillBorder(int itemID)
{
    if (m_sizeX > 0)
    {
        int floorIdx = (m_sizeY - 1) * m_sizeX;
        for (int x = 0; x < m_sizeX; x++)
        {
            m_tiles[x].SetItemID(itemID);
            m_tiles[x + floorIdx].SetItemID(itemID);
        }
        for (int y = 0; y < m_sizeY; y++)
        {
            m_tiles[y * m_sizeX].SetItemID(itemID);
            m_tiles[(y * m_sizeX) + (m_sizeX - 1)].SetItemID(itemID);
        }
    }
}
void WorldTileMap::FillRect(CL_Rect r, int type, int bgType)
{
    if (r.top < 0)
        r.top = 0;
    if (r.left < 0)
        r.left = 0;
    for (int x = r.left; x < r.right; ++x)
    {
        for (int y = r.top; y < r.bottom; ++y)
        {
            if (x < m_sizeX && y < m_sizeY)
            {
                Tile* pTile = &m_tiles[x + (y * m_sizeX)];
                if (type != -1)
                    pTile->Set(type, 0, nullptr);
                if (bgType != -1)
                    pTile->SetBG(bgType);
            }
        }
    }
}
void WorldTileMap::FillRectRandom(CL_Rect r, int type, int bgType, int randomCount)
{
    while (randomCount-- > 0)
    {
        int x = RandomRange(r.left, r.right);
        int y = RandomRange(r.top, r.bottom);
        Tile* pTile = &m_tiles[x + (y * m_sizeX)];
        if (type != -1)
            pTile->Set(type, 0, nullptr);
        if (bgType != -1)
            pTile->SetBG(bgType);
    }
}