#include "WorldObject.h"
#include "ServerApplication.h"

WorldObject::WorldObject()
{
    //
}

WorldObject::~WorldObject()
{
    //
}

void WorldObject::Serialize(uint8* pMem, int& offsetInOut, bool bWriteToMem)
{
    if (bWriteToMem)
    {
        *(uint16*)(&pMem[offsetInOut]) = m_itemID;
        *(float*)(&pMem[offsetInOut + 2]) = m_pos.x;
        *(float*)(&pMem[offsetInOut + 6]) = m_pos.y;
        *(uint8*)(&pMem[offsetInOut + 10]) = m_count;
        *(uint8*)(&pMem[offsetInOut + 11]) = m_flags;
        *(uint32*)(&pMem[offsetInOut + 12]) = m_objectID;
    }
    else
    {
        m_itemID = *(uint16*)(&pMem[offsetInOut]);
        m_pos.x = *(float*)(&pMem[offsetInOut + 2]);
        m_pos.y = *(float*)(&pMem[offsetInOut + 6]);
        m_count = *(uint8*)(&pMem[offsetInOut + 10]);
        m_flags = *(uint8*)(&pMem[offsetInOut + 11]);
        m_objectID = *(uint32*)(&pMem[offsetInOut + 12]);

        if (m_itemID >= GetApp()->GetItemManager().GetItemCount())
            m_itemID = 2; // fallback for illegal itemID
    }
    
    offsetInOut += 16;
}

CL_Rectf WorldObject::GetWorldRect()
{
    return CL_Rectf
    (
        m_pos.x,
        m_pos.y,
        m_pos.x + 16.0f,
        m_pos.y + 16.0f
    );
}