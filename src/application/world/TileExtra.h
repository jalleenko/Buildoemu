#pragma once
#include "PlatformSetup.h"
#include "items/ItemInfo.h"
#include "world/World.h"

class TileExtra
{
public:
    TileExtra(){};

    enum eType : int
    {
        TYPE_NONE,
        TYPE_DOOR,
        TYPE_SIGN,
        TYPE_LOCK,
        TYPE_TREE
    };
    void Serialize(unsigned char *pMem, int *offsetInOut, bool bSave, World* pWorld, bool bIsForClient);

    // STUBS
    void Init(ItemType ItemType);
    int GetIDOfPersonWithAccessToLock(int);
    void GivePersonAccessToLockID(int);
    bool HasKeyToLock(int);
    void ModGrowth(int, int);
    void RemovePersonAccessToLockID(int);
    void SetFruitCountToSpawn(byte);
    void SetGrowth(int);
    void SetLockOwner(int);
    int GetGrowth();
private:
    void FinalizeGrowth();

    vector<int> m_playerList;
    string m_name = "";
    string m_text = "";
    string m_id = "";
    unsigned int m_growth = 0;
    unsigned int m_timer = 0;
    int m_ownerID = 0;
    unsigned char m_type = 0;
    unsigned char m_flags = 0;
    unsigned char m_fruitsToSpawn = 0;
};