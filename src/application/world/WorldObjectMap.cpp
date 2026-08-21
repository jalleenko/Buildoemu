#include "WorldObjectMap.h"
#include "util/ResourceUtils.h"

WorldObjectMap::WorldObjectMap()
{
    m_count = 0;
    // m_offset = 0; // it's being calculated in Serialize function anyways.
}

WorldObjectMap::~WorldObjectMap()
{
    //
}

void WorldObjectMap::Clear()
{
    m_objectsList.clear();
    m_count = 0;
}

void WorldObjectMap::ReorderObjects()
{
    int objOffset = 0;
    m_count = 0;
    for (WorldObject& obj : m_objectsList)
    {
        obj.m_objectID = ++objOffset;
    }
    m_count = objOffset;
}

int WorldObjectMap::GetMemEstimate()
{
    return (int)(sizeof(WorldObject) * m_objectsList.size());
}

void WorldObjectMap::Serialize(uint8* pMem, int& offsetInOut, bool bWriteToMem, bool bIgnoreExtra)
{
    int objectCount;
    if (m_objectsList.empty())
    {
        objectCount = 0;
    }
    else
    {
        objectCount = (int)m_objectsList.size();
        if (objectCount > 300000) // performance check
        {
            LogError("PERFNOTE: World is trying to load %d objects.  Rejecting load", objectCount);
            return;
        }
        if (objectCount > 100000) // performance check
        {
            LogError("PERFNOTE: World is trying to load %d objects", objectCount);
        }
    }

    if (bWriteToMem)
    {
        /*
        for (WorldObject& obj : m_objectsList)
        {
            if (obj.m_itemID != ITEM_ID_CARRIED_GAME_FLAG)
                continue;

            objectCount--;
        }
        */

        *(uint32*)(&pMem[offsetInOut]) = objectCount;
        offsetInOut += 4;
        *(uint32*)(&pMem[offsetInOut]) = m_count;
        offsetInOut += 4;

        for (WorldObject& obj : m_objectsList)
        {
            // if (obj.m_itemID == ITEM_ID_CARRIED_GAME_FLAG)
                // continue;

            obj.Serialize(pMem, offsetInOut, true);
        }
    }
    else
    {
        uint32 numObjects = *(uint32*)(&pMem[offsetInOut]);
        offsetInOut += 4;
        m_count = *(uint32*)(&pMem[offsetInOut]);
        offsetInOut += 4;

        for (uint32 i = 0; i < numObjects; i++)
        {
            WorldObject& obj = m_objectsList.emplace_back(WorldObject());
            obj.Serialize(pMem, offsetInOut, false);
        }

        if (!bIgnoreExtra)
            ReorderObjects();
    }
}