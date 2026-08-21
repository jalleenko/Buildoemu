#include "TileExtra.h"
#include "items/ItemInfo.h"
#include "util/ResourceUtils.h"

int TileExtra::GetGrowth()
{
    // STUB.
    return 0;
}

void TileExtra::FinalizeGrowth()
{
    // STUB.
    return;
}

void TileExtra::Init(ItemType type)
{
    switch (type)
    {
        case ItemType::UserDoor:
        case ItemType::Door:
        {
            m_type = TYPE_DOOR;
            break;
        }
        case ItemType::Sign:
        {
            m_type = TYPE_SIGN;
            break;
        }
        case ItemType::Lock:
        {
            m_type = TYPE_LOCK;
            break;
        }
        case ItemType::Seed:
        {
            m_type = TYPE_TREE;
            //m_timer = GetTick();
            m_growth = 0;
            m_fruitsToSpawn = 3;
            break;
        }
        default:
            break;
    }
}

void TileExtra::Serialize(unsigned char *pMem, int *offsetInOut, bool bSave, World *pWorld, bool bIsForClient)
{
    MemorySerialize<unsigned char>(m_type, pMem, *offsetInOut, bSave);
    switch (m_type)
    {
        case TYPE_DOOR:
        {
            if (bIsForClient) {
                if (bSave) {
                    if (!m_name.size()) {
                        MemorySerialize(m_text, pMem, *offsetInOut, bSave);
                    } else {
                        MemorySerialize(m_name, pMem, *offsetInOut, bSave);
                    }
                } else {
                    MemorySerialize(m_text, pMem, *offsetInOut, bSave);
                }
            } else {
                MemorySerialize(m_name, pMem, *offsetInOut, bSave);
                MemorySerialize(m_text, pMem, *offsetInOut, bSave);
                MemorySerialize(m_id, pMem, *offsetInOut, bSave);
            }
            MemorySerialize<unsigned char>(m_flags, pMem, *offsetInOut, bSave);
            break;
        }
        case TYPE_SIGN:
        {
            MemorySerialize(m_text, pMem, *offsetInOut, bSave);
            MemorySerialize<int>(m_ownerID, pMem, *offsetInOut, bSave);
            break;
        }
        case TYPE_LOCK:
        {
            int accessed = m_playerList.size();
            MemorySerialize<unsigned char>(m_flags, pMem, *offsetInOut, bSave);
            MemorySerialize<int>(m_ownerID, pMem, *offsetInOut, bSave);
            MemorySerialize<int>(accessed, pMem, *offsetInOut, bSave);
            if (!bSave)
                m_playerList.resize(accessed);
            for (int i = 0; i < accessed; i++)
                MemorySerialize<int>(m_playerList[i], pMem, *offsetInOut, bSave);
            break;
        }
        case TYPE_TREE:
        {
            if (bSave) {
                FinalizeGrowth();
            }
            MemorySerialize<unsigned int>(m_growth, pMem, *offsetInOut, bSave);
            MemorySerialize<unsigned char>(m_fruitsToSpawn, pMem, *offsetInOut, bSave);
            break;
        }
    }
}