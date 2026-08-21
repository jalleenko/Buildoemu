#pragma once
#include <vector>
#include "ItemInfo.h"

class ItemManager
{
public:
    struct PlayerItemData
    {
        uint32 hash = 0;
        uint32 size = 0;
        uint8* data = 0;
    };

    bool Load();
    void InitializePlayerItemData();
    int GetItemCount() { return (int)m_items.size(); }
    PlayerItemData& GetPlayerItemData() { return m_itemData; } 
    ItemInfo* GetItemByIDSafe(int itemID);

private:
    void CalculateRarityForAllItems();
    int GetRarityForItem(ItemInfo* pItemInfo, int parentRarity);
    void CreateSeedVersionOfLastItemAdded(int itemID);
    uint8* SaveToMem(uint32& sizeOut, uint16 itemDataVersion);

    std::vector<ItemInfo*> m_items{};

    PlayerItemData m_itemData{};
};