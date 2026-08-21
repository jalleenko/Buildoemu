#pragma once
#include "world/WorldObject.h"
#include <list>
#include "PlatformSetup.h"

class WorldObjectMap
{
public:
    uint32 m_count;
    uint32 m_offset;
    std::list<WorldObject> m_objectsList;

public:
    WorldObjectMap();
    virtual ~WorldObjectMap();

    void Clear();
    void ReorderObjects();

    int GetMemEstimate();
    // @Note: bIgnoreExtra is set to false when loading from disk.
    void Serialize(uint8* pMem, int& offsetInOut, bool bWriteToMem, bool bIgnoreExtra);
};