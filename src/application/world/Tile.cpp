#include "Tile.h"
#include "ServerApplication.h"
#include "TileExtra.h"
#include "util/ResourceUtils.h"

void Tile::Set(short type, short visual, World* pWorld)
{
    m_itemID = type;
    ItemInfo* pItem = GetApp()->GetItemManager().GetItemByIDSafe(m_itemID);
    if (!pItem)
        return;
    if (m_pTileExtra)
    {
        delete m_pTileExtra;
        m_pTileExtra = 0;
        m_flags &= ~TILE_FLAG_HAS_EXTRA_DATA;
    }
    switch (pItem->m_type)
    {
        case ItemType::UserDoor:
        case ItemType::Lock:
        case ItemType::Sign:
        case ItemType::Door:
        case ItemType::Seed:
        {
            m_pTileExtra = new TileExtra();
            m_pTileExtra->Init(pItem->m_type);
            m_flags |= TILE_FLAG_HAS_EXTRA_DATA;
            break;
        }
        default:
            break;
    }
}

void Tile::Serialize(uint8* buffer, int & offsetInOut, bool write)
{
    MemorySerialize(m_itemID, buffer, offsetInOut, write);
    MemorySerialize(m_itemBGID, buffer, offsetInOut, write);
    MemorySerialize(m_parent, buffer, offsetInOut, write);
    MemorySerialize(m_flags, buffer, offsetInOut, write);
    if (m_flags & TILE_FLAG_HAS_PARENT)
        MemorySerialize(m_parent, buffer, offsetInOut, write);

    if (m_flags & TILE_FLAG_HAS_EXTRA_DATA)
    {
        if (!m_pTileExtra)
            m_pTileExtra = new TileExtra();
        // everything is temporary right now, the function we're in right now should accomodate for bIsForClient later.
        m_pTileExtra->Serialize(buffer, &offsetInOut, write, nullptr, true);
    }
}