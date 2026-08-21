#pragma once
#include "API/Core/Math/vec2.h"
#include "PlatformSetup.h"
#include "enet/enet.h"

enum class NetMessageType : uint32
{
    Unknown = 0,
    ServerHello,
    GenericMessage,
    GameMessage,
    GamePacket
};

enum class GamePacketType : uint8
{
    State = 0,
    FunctionCall,
    SendMap = 4,
    SendInventoryState = 9,
    ItemChangeObject = 14,
    SendItemDatabaseData = 16
};

enum class GamePacketFlag : uint32
{
    Extended = (1 << 3),
    FacingLeft = (1 << 4),
};

class GameUpdatePacket
{
public:
    bool HasFlag(GamePacketFlag flag) { return flags & (uint32)flag; }
    void SetFlag(GamePacketFlag flag) { flags |= (uint32)flag; }
    void SetType(GamePacketType gpType) { type = (uint8)gpType; }

    uint8 type = 0;
    uint8 dataField0 = 0;
    uint8 dataField1 = 0;
    uint8 dataField3 = 0;

    union
    {
        int dataField4 = 0;
        int destinationNetID;
    };

    int dataField6 = 0;
    
    uint32 flags = 0;

    float dataField8 = 0;
    
    union
    {
        int dataField9 = 0;
        int delayMS;
    };

    CL_Vec2f dataField10{};
    CL_Vec2f dataField11{};
    float dataField12 = 0;
    int dataField13 = 0;
    int dataField14 = 0;

    uint32 extendedDataSize = 0;
};

namespace Protocol
{
    NetMessageType GetMessageType(ENetPacket* packet);
    char* GetMessage(ENetPacket* packet);
    bool GetStringFromText(const char* pText, const char* key, std::string& sOut, int count, bool bPicky);
    uint8* GetStructPointerFromTankPacket(ENetPacket* packet);
}