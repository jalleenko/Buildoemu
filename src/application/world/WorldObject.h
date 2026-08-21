#pragma once
#include "PlatformSetup.h"
#include "API/Core/Math/vec2.h"
#include "API/Core/Math/rect.h"

enum eObjectChangeType
{
    OBJECT_CHANGE_TYPE_COLLECT      = 0,
    OBJECT_CHANGE_TYPE_SPAWN        = -1,
    OBJECT_CHANGE_TYPE_REMOVE       = -2,
    OBJECT_CHANGE_TYPE_UPDATE       = -3,
};

enum eWorldObjectFlags : uint8
{
    WORLDOBJECTFLAG_NONE            = 0,            // 0x00
    WORLDOBJECTFLAG_SHRINK          = (1 << 0),     // 0x01
    WORLDOBJECTFLAG_MERGED          = (1 << 1),     // 0x02
    WORLDOBJECTFLAG_SYSTEM          = (1 << 2),     // 0x04
    WORLDOBJECTFLAG_RADIATED        = (1 << 3),     // 0x08
    WORLDOBJECTFLAG_UNK16           = (1 << 4),     // 0x10
    WORLDOBJECTFLAG_UNK32           = (1 << 5),     // 0x20
    WORLDOBJECTFLAG_UNK64           = (1 << 6),     // 0x40
    WORLDOBJECTFLAG_UNK128          = (1 << 7),     // 0x80
};

class WorldObject
{
public:
    CL_Vec2f m_pos;
    uint16 m_itemID;
    uint8 m_count;
    uint8 m_flags;
    uint32 m_objectID;

public:
    WorldObject();
    virtual ~WorldObject();

    void Serialize(uint8* pMem, int& offsetInOut, bool bWriteToMem);

    CL_Rectf GetWorldRect();
};