#pragma once
#include <iostream>
#include <vector>
#include "API/Core/Math/rect.h"
#include "Tile.h"
#include "PlatformSetup.h"

class WorldTileMap
{
public:
    int GetMemEstimate();
    void Serialize(uint8* buffer, int& offsetInOut, bool write);
    void CreateWorld();

    void FillBorder(int itemID);
    void FillRect(CL_Rect r, int type, int bgType);
    void FillRectRandom(CL_Rect r, int type, int bgType, int randomCount);

private:
    int m_sizeX = 100;
    int m_sizeY = 60;
    std::vector<Tile> m_tiles{};
};